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


#ifndef EVA_IDLE_TIME_DETECTOR_H
#define EVA_IDLE_TIME_DETECTOR_H

#include <qobject.h>
#include "config.h"

class QTimer;


// Seconds per minutes - useful for speeding debugging up!
const int secsPerMinute = 60;

// Minutes between each idle overrun test.
const int testInterval= 1000;

/**
 * Keep track of how long the computer has been idle.
 */

class IdleTimeDetector :public QObject
{
Q_OBJECT

public:
  /**
     Initializes and idle test timer
     @param maxIdle minutes before the idle timer will go off.
  **/
  IdleTimeDetector(int maxIdle);

  /**
     Returns true if it is possible to do idle detection.
     Idle detection relys on a feature in the X server, which might not
     always be present.
  **/
  bool isIdleDetectionPossible();

signals:
  /**
     Tells the listener to extract time from current timing.
     The time to extract is due to the idle time since the dialog wass
     shown, and until the user answers the dialog.
     @param minutes Minutes to extract.
  **/
  void extractTime(int minutes);

  /**
      Tells the listener to stop timing
   **/
  void stopAllTimers();

  /**
      Tells EVA to change user's status
   **/
  void idleTimeUp();

  void idleBack();

public slots:
  /**
     Sets the maximum allowed idle.
     @param maxIdle Maximum allowed idle time in minutes
  **/
  void setMaxIdle(int maxIdle);

  /**
     Starts detecting idle time
  **/
  void startIdleDetection();

  /**
      Stops detecting idle time.
  **/
  void stopIdleDetection();

  /**
     Sets whether idle detection should be done at all
     @param on If true idle detection is done based on @see
     startIdleDetection and @ref stopIdleDetection
  **/
  void toggleOverAllIdleDetection(bool on);


protected:
#ifdef HAVE_LIBXSS
  void informOverrun();
  void informBack();
#endif // HAVE_LIBXSS

protected slots:
  void check();

private:
  bool _idleDetectionPossible;
  bool _overAllIdleDetect; // Based on preferences.
  int _maxIdle;
  QTimer *_timer;
};

#endif // EVA_IDLE_TIME_DETECTOR_H
