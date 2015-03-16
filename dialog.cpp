/***************************************************************************
* Copyright (C) 2009 The BooxPowerPack Project
* All rights reserved.
* Contact: woshiu (woshiu@protonmail.ch)
* 
* This file is part of BooxPowerPack.
*
* BooxPowerPack is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* BooxPowerPack is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with BooxPowerPack.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#include "dialog.h"
#include <QtGui>
#include "myfiledialog.h"
#include "onyx/screen_proxy/screen_proxy.h"
#include "onyx/sys/sys_status.h"
using namespace onyx::screen;

void Dialog::refreshScreen(Waveform w)
{
    onyx::screen::ScreenProxy::instance().flush(this, w);
}

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    QLabel *screenSaversLabel = new QLabel(QString::fromUtf8("Wygaszacze ekranów"), this);
    
    listWidget = new QListWidget(this);
    listWidget->setViewMode(QListWidget::IconMode);
    listWidget->setResizeMode(QListWidget::Adjust);
    listWidget->setUniformItemSizes(true);
    listWidget->setDragEnabled(false);
    listWidget->setIconSize(QSize(235, 235));
    QDir dir("/usr/share/system_manager/images/");
    foreach (QFileInfo qfi, dir.entryInfoList(QDir::NoFilter, QDir::Time | QDir::Reversed)) {
        if (qfi.fileName() == "." || qfi.fileName() == "..") continue;
        QIcon icon(qfi.absoluteFilePath());
        QListWidgetItem *item = new QListWidgetItem(listWidget);
        item->setText(qfi.fileName());
        item->setIcon(icon);
        listWidget->addItem(item);
    }
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(screenSaversLabel);
    layout->addWidget(listWidget);
    
    
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    addButton = new QPushButton(tr("Dodaj nowe..."), this);
    //addButton->setFlat(true);
    deleteButton = new QPushButton(QString::fromUtf8("Usuń wybrane"), this);
    //deleteButton->setFlat(true);
    exitButton = new QPushButton(QString::fromUtf8("Wyjście"), this);
    //exitButton->setFlat(true);
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(exitButton);

    
    connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonClicked()));
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteButtonClicked()));
    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitButtonClicked()));
    
    connect(listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(on_currentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    
    layout->addLayout(buttonsLayout);
    QLabel *info = new QLabel(tr("<b>Screensaver Manager v0.1PL build20091107</b>, of <b><em><a href='http://sites.google.com/site/booxpowerpack'>BooxPowerPack</a></em></b>"), this);
    info->setAlignment(Qt::AlignRight);
    layout->addWidget(info);
    this->setLayout(layout);
}

Dialog::~Dialog()
{
}

void Dialog::on_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString fileName = current->text();
    if (fileName == "screen_saver0.png" || fileName == "screen_saver1.png" || fileName == "screen_saver2.png")
    {
        deleteButton->setEnabled(false);
    } else {
        deleteButton->setEnabled(true);
    }
}

void Dialog::addButtonClicked()
{
    MyFileDialog fileDialog(this);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.setFilter(tr("Obrazy PNG (*.png)"));
    fileDialog.setViewMode(QFileDialog::Detail);
    fileDialog.setDirectory("/media/sd/");
    fileDialog.resize(500, 300);
    QStringList fileNames;
    if (fileDialog.exec()) fileNames = fileDialog.selectedFiles();

    if (fileNames.isEmpty()) return;

    QString fileName = fileNames.at(0);
    if (fileName != 0) {
        QImage image(fileName);
        if (image.width() != 600 || image.height() != 800) {
            QMessageBox::information(this, QString::fromUtf8("Zły obraz"),
                                     QString::fromUtf8("Szerokość obrazu powinna wynosić 600 pixeli, a wysokość powinna wynosić 800 pixeli."));

        } else {
            QString copyDest = "screen_saver";
            copyDest.append(QString::number(listWidget->count())).append(".png");
            QFile::remove("/usr/share/system_manager/images/" + copyDest);

            if (QFile::copy(fileName,  "/usr/share/system_manager/images/" + copyDest)) {
                QListWidgetItem *item = new QListWidgetItem(listWidget);
                QIcon icon(fileName);
                item->setIcon(icon);
                item->setText(copyDest);
                listWidget->addItem(item);
            } else {
                QMessageBox::information(this, QString::fromUtf8("Kopiowanie obrazu nie powiodło się"),
                                        QString::fromUtf8("Kopiowanie obrazu nie powiodło się."));

            }
        }
    }
    refreshScreen(GC);
}
        
void Dialog::deleteButtonClicked()
{
    int curIndex = listWidget->currentRow();
    int ret = QMessageBox::question(this, QString::fromUtf8("Potwierdź usunięcie..."), QString::fromUtf8("Czy na pewno usunąć ten wygaszacz ekranu ?"), QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::No) {
        return;
    }
    QFile::remove("/usr/share/system_manager/images/" + listWidget->currentItem()->text());
    listWidget->takeItem(curIndex);
    listWidget->removeItemWidget(listWidget->currentItem());
    for (int i = curIndex + 1; i <= listWidget->count(); i++)
    {
        QFile::rename("/usr/share/system_manager/images/screen_saver" + QString::number(i) + ".png",
                      "/usr/share/system_manager/images/screen_saver" + QString::number(i - 1) + ".png");
        listWidget->item(i - 1)->setText("screen_saver" + QString::number(i - 1) + ".png");
    }

}
void Dialog::exitButtonClicked()
{
    int ret = QMessageBox::question(this, QString::fromUtf8("Potwierdź zamknięcie..."), QString::fromUtf8("Czy na pewno zamknąć program?"), QMessageBox::Yes, QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        QApplication::exit();
    }
}

bool Dialog::event(QEvent* event)
{
    bool ret = QDialog::event(event);
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = (QKeyEvent*)event;
        
        if (keyEvent->key() ==Qt::Key_Escape)
        {
            QApplication::exit();
            return true;
        }        
        else if (keyEvent->key() == Qt::Key_Menu)
        {
            refreshScreen(GC);
            return true;
        }
    }else if (event->type() == QEvent::UpdateRequest)
    {
        refreshScreen();
        return true;
    }
    return ret;
}
