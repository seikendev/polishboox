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
#ifndef MYFILEDIALOG_H
#define MYFILEDIALOG_H

#include <QtGui/QFileDialog>
#include "onyx/screen_proxy/screen_proxy.h"
using namespace onyx::screen;
class MyFileDialog : public QFileDialog
{
    Q_OBJECT

public:
    MyFileDialog(QWidget *parent = 0, const QString & caption = QString(), const QString & directory = QString(), const QString & filter = QString());
    ~MyFileDialog();
    bool event(QEvent* event);
    void refreshScreen(Waveform w=GU);
};

#endif // MYFILEDIALOG_H
