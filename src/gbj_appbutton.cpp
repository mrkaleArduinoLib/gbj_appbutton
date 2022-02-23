#include "gbj_appbutton.h"

void gbj_appbutton::run()
{
  // Categorize button event
  Button type = Button::NONE;
  if (pulse_.flPress) // Pressed button
  {
    if (millis() - pulse_.tsEdge >= Timing::PERIOD_LONG)
    {
      type = Button::LONG;
      // Prevent detecting idle if holding button after initiating setup mode.
      // Autorelease of the touch sensor TTP223 is after ~8.5 seconds, so that
      // holding button leads to idle state in either case.
      pulse_.tsEdge = millis();
    }
  }
  else // Released button
  {
    if (millis() - pulse_.tsEdge > Timing::PERIOD_IDLE)
    {
      type = Button::IDLE;
      pulse_.flReady = true;
    }
    else if (pulse_.length > Timing::PERIOD_SHORT)
    {
      type = Button::SHORT;
    }
  }
  // No button action if previous has been finished
  if (!pulse_.flReady)
  {
    return;
  }
  // Perform button action
  switch (type)
  {
    case Button::IDLE:
      if (handlers_.onButtonIdle)
      {
        handlers_.onButtonIdle();
      }
      pulse_.flReady = false;
      pulse_.tsEdge = millis();
      break;

    case Button::SHORT:
      if (handlers_.onButtonShort)
      {
        handlers_.onButtonShort();
      }
      pulse_.flReady = false;
      break;

    case Button::LONG:
      if (handlers_.onButtonLong)
      {
        handlers_.onButtonLong();
      }
      pulse_.flReady = false;
      pulse_.tsEdge = millis();
      break;

    case Button::NONE:
    default:
      break;
  }
}

void gbj_appbutton::isr()
{
  if (millis() - pulse_.tsEdge < Timing::PERIOD_BOUNCE)
  {
    return;
  }
  // Button pressed
  if (pulse_.flPress ^= 1)
  {
    pulse_.flReady = true;
    pulse_.length = 0;
  }
  // Button released
  else
  {
    pulse_.length = millis() - pulse_.tsEdge;
  }
  pulse_.tsEdge = millis();
}
