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
#include <QtGui/QApplication>
#include "dialog.h"
#include "onyx/screen_proxy/screen_proxy.h"
#include "onyx/sys/sys_status.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sys::SysStatus::instance().setSystemBusy(false);
    Dialog w;
    w.showFullScreen();
    w.refreshScreen();
    return a.exec();
}