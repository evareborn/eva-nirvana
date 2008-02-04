/***************************************************************************
 *   Copyright (C) 2004 by nwillis                                                   *
 *   neowillis@gmail.com
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "evaidt.h"

#include <qdatetime.h>
#include <qmessagebox.h>
#include <qtimer.h>

#ifdef HAVE_LIBXSS
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>
#endif // HAVE_LIBXSS


IdleTimeDetector::IdleTimeDetector(int maxIdle)
{
  _maxIdle = maxIdle;

#ifdef HAVE_LIBXSS
  int event_base, error_base;
  if(XScreenSaverQueryExtension(qt_xdisplay(), &event_base, &error_base)) {
    _idleDetectionPossible = true;
  }
  else {
    _idleDetectionPossible = false;
  }

  _timer = new QTimer(this);
  connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
#else
  _idleDetectionPossible = false;
#endif // HAVE_LIBXSS

}

bool IdleTimeDetector::isIdleDetectionPossible()
{
  return _idleDetectionPossible;
}

void IdleTimeDetector::check()
{
#ifdef HAVE_LIBXSS
  XScreenSaverInfo *_mit_info;

  if (_idleDetectionPossible)
  {
    _mit_info = XScreenSaverAllocInfo ();
    XScreenSaverQueryInfo(qt_xdisplay(), qt_xrootwin(), _mit_info);
    int idleMinutes = (_mit_info->idle/1000)/secsPerMinute;
    if (idleMinutes >= _maxIdle)
      informOverrun();
    else
      informBack();
  }
#endif // HAVE_LIBXSS
}

void IdleTimeDetector::setMaxIdle(int maxIdle)
{
  _maxIdle = maxIdle;
}

#ifdef HAVE_LIBXSS
void IdleTimeDetector::informOverrun()
{
  if (!_overAllIdleDetect)
    return; // In the preferences the user has indicated that he do not
            // want idle detection.
  
  _timer->stop();

  emit idleTimeUp();
  _timer->start(testInterval);
}
#endif // HAVE_LIBXSS

#ifdef HAVE_LIBXSS
void IdleTimeDetector::informBack()
{
  if (!_overAllIdleDetect)
    return; // In the preferences the user has indicated that he do not
            // want idle detection.
  
  _timer->stop();

  emit idleBack();
  _timer->start(testInterval);
}
#endif // HAVE_LIBXSS


void IdleTimeDetector::startIdleDetection()
{
#ifdef HAVE_LIBXSS
  if (!_timer->isActive())
    _timer->start(testInterval);
#endif //HAVE_LIBXSS
}

void IdleTimeDetector::stopIdleDetection()
{
#ifdef HAVE_LIBXSS
  if (_timer->isActive())
    _timer->stop();
#endif // HAVE_LIBXSS
}
void IdleTimeDetector::toggleOverAllIdleDetection(bool on)
{
  _overAllIdleDetect = on;
}

