/*
  NAME:
  gbj_appbutton

  DESCRIPTION:
  Library manages a general button, preferably the touch sensor TTP223.
  - It processes button presses with external interrupts.
  - It distinguishes short and long presses (touches), but no double touches.
  - It does not rely on pressing or realising the button, just on a change. So,
    the library manages active low or active high buttons equally.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
 */
#ifndef GBJ_APPBUTTON_H
#define GBJ_APPBUTTON_H

#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#elif defined(ESP8266) || defined(ESP32)
  #include <Arduino.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif
#include "gbj_appcore.h"
#include "gbj_serial_debug.h"

#undef SERIAL_PREFIX
#define SERIAL_PREFIX "gbj_appbutton"

class gbj_appbutton : gbj_appcore
{
public:
  static const String VERSION;

  typedef void Handler();

  struct Handlers
  {
    Handler *onButtonIdle;
    Handler *onButtonShort;
    Handler *onButtonLong;
  };

  /*
  Constructor.

  DESCRIPTION:
  Constructor creates the class instance object and registers handlers.

  PARAMETERS:
  handlers - A structure with pointers to various callback handler functions.
    - Data type: Handlers
    - Default value: structure with zeroed all handlers
    - Limited range: system address range

  RETURN: object
*/
  gbj_appbutton(Handlers handlers = Handlers()) { handlers_ = handlers; }

  /*
    Processing.

    DESCRIPTION:
    The method should be called in an application sketch loop.
    It processes main functionality and is control by the internal timer.

    PARAMETERS: None

    RETURN: none
  */
  void run();

  /*
    Interruption Service Routing.

    DESCRIPTION:
    The method should be called in a main sketch ISR attached to the touch
    button pin.

    PARAMETERS: None

    RETURN: none
  */
  void isr();

private:
  enum Timing : unsigned int
  {
    PERIOD_BOUNCE = 20,
    PERIOD_SHORT = 100,
    PERIOD_LONG = 2000,
    PERIOD_IDLE = 8500,
  };
  enum Button : byte
  {
    NONE,
    SHORT,
    LONG,
    IDLE,
  };
  struct Pulse
  {
    unsigned long tsEdge; // Timestamp of the recent edge event
    unsigned int length; // Length of the recent pulse
    bool flPress; // Flag about pressed button (starting edge of a pulse)
    bool flReady; // Flag about waiting for button action
  } pulse_;
  Handlers handlers_;
};

#endif
