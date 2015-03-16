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
#include "myfiledialog.h"
#include <QtGui>
#include "onyx/screen_proxy/screen_proxy.h"
#include "dialog.h"
using namespace onyx::screen;

MyFileDialog::MyFileDialog(QWidget *parent, const QString & caption, const QString & directory, const QString & filter)
    : QFileDialog(parent, caption, directory, filter)
{
}

MyFileDialog::~MyFileDialog()
{
}

bool MyFileDialog::event(QEvent* event)
{
    bool ret = QFileDialog::event(event);

    if (event->type() == QEvent::UpdateRequest)
    {
        refreshScreen();
        return true;
    }
    return ret;
}

void MyFileDialog::refreshScreen(Waveform w)
{
    onyx::screen::ScreenProxy::instance().flush(this, w);
}
