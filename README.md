<a id="library"></a>

# gbj\_appbutton
This is an application library, which is used usually as a project library for particular PlatformIO project. It encapsulates the functionality of a `tactile or touch switch`. The encapsulation provides following advantages:

* Functionality is hidden from the main sketch.
* The library follows the principle `separation of concern`.
* The library is reusable for various projects without need to code the switch management.
* Update in library is valid for all involved projects.
* It specifies (inherits from) the parent application library `gbj_appcore`.
* It utilizes funcionality and error handling from the parent class.


## Fundamental functionality

* The functionality is represented within this library by a class method called by an `Interuption Service Routine` (referred to as "ISR") usually in the main sketch at each change of a GPIO pin of a microcontroller sensing a tactile or touch switch.
* The library processes the switch changes for detecting and evaluating `short and long presses`.
* **The library does not recognize double presses.**
* The library assumes subsequent toggling of pressing and releasing of a switch at state changes.


<a id="internals"></a>

## Internal parameters
Internal parameters are hard-coded in the library usually as enumerations and have neither setters nor getters associated.

* **Time period for reset** (8500 ms): It is a minimal period of a switch keeping pressed to reset a switch press, i.e., to ignore it. Its value has been chosen for reset period of the touch sensor `TTP223`.
* **Time period for long press** (2000 ms): It is a minimal period of a switch keeping pressed, but not longer than period for switch reset, to recognize a long switch press.
* **Time period for short press** (100 ms): It is a minimal period of a switch keeping pressed, but not longer than period for long press, to recognize a short switch press.
* **Time period for debouncing** (50 ms): This parameter determines time period within which subsequent switch state change is ignored.


<a id="dependency"></a>

## Dependency

* **gbj\_appcore**: Parent library loaded from the file `gbj_appcore.h`.
* **gbj\_serial\_debug**: Auxilliary library for debug serial output loaded from the file `gbj_serial_debug.h`. It enables to exclude serial outputs from final (production) compilation.

#### Arduino platform
* **Arduino.h**: Main include file for the Arduino SDK.
* **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.

#### Espressif ESP8266 platform
* **Arduino.h**: Main include file for the Arduino platform.

#### Espressif ESP32 platform
* **Arduino.h**: Main include file for the Arduino platform.

#### Particle platform
* **Particle.h**: Includes alternative (C++) data type definitions.


<a id="constants"></a>

## Constants

* **gbj\_appbutton::VERSION**: Name and semantic version of the library.

Other constants and enumerations are inherited from the parent library.


<a id="interface"></a>

## Custom data types
* [Handler](#handler)
* [Handlers](#handlers)

## Interface
* [gbj_appbutton()](#gbj_appbutton)
* [run()](#run)
* [isr()](#isr)


<a id="handler"></a>

## Handler

#### Description
The template or the signature of a callback function, which is called at particular event in the processing. It can be utilized for instant communicating with other modules of the application (project).
* A handler is just a function with no arguments and returning nothing.
* A handler can be declared just as `void` type in the main sketch.

#### Syntax
    typedef void Handler()

#### Parameters
None

#### Returns
None

#### See also
[Handlers](#handlers)

[Back to interface](#interface)


<a id="handlers"></a>

## Handlers

#### Description
The structure of pointers to handlers each for particular event in processing.
* Individual or all handlers do not need to be defined in the main sketch, just those that are useful.

#### Syntax
    struct Handlers
    {
        Handler *onButtonIdle;
        Handler *onButtonShort;
        Handler *onButtonLong;
    }

#### Parameters

* **onButtonIdle**: Pointer to a callback function, which is call at resetting the switch due to too long keeping it pressed.
  * *Valid values*: system address range
  * *Default value*: none


* **onButtonShort**: Pointer to a callback function, which is call at detecting a short press of the switch.
  * *Data type*: Handler
  * *Default value*: none


* **onButtonLong**: Pointer to a callback function, which is call at detecting a long press of the switch.
  * *Data type*: Handler
  * *Default value*: none

#### Example
```cpp
void onButtonIdle()
{
  ...
}
void onButtonShort()
{
  ...
}
void onButtonLong()
{
  ...
}
gbj_appbutton::Handlers handlersButton = { .onButtonIdle = onButtonIdle,
                                           .onButtonShort = onButtonShort,
                                           .onButtonLong = onButtonLong };
gbj_appbutton button = gbj_appbutton(handlersButton);
```

#### See also
[Handler](#handler)

[gbj_appbutton](#gbj_appbutton)

[Back to interface](#interface)


<a id="gbj_appbutton"></a>

## gbj_appbutton()

#### Description
Constructor creates the class instance object and initiates internal resources and parameters.

#### Syntax
    gbj_appbutton(Handlers handlers)

#### Parameters
* **handlers**: Pointer to a structure of callback functions. This structure as well as handlers should be defined in the main sketch.
  * *Data type*: Handlers
  * *Default value*: empty structure

#### Returns
Object performing tactile or touch switch processing.

#### See also
[Handler](#handler)

[Handlers](#handlers)

[Back to interface](#interface)


<a id="run"></a>

## run()

#### Description
The execution method, which should be called frequently, usually in the loop function of a sketch.
* The method evaluates switch presses from data caught by ISR and calls appropriate handlers.

#### Syntax
	void run()

#### Parameters
None

#### Returns
None

[Back to interface](#interface)


<a id="isr"></a>

## isr()

#### Description
The execution method that should be called by _Interruption Service Routing_ in the main project sketch.
* The method registers presses and releases of the switch and provides debouncing.

#### Syntax
    void isr()

#### Parameters
None

#### Returns
None

#### Example
```cpp
gbj_appbutton button = gbj_appbutton(...);
IRAM_ATTR void isrButton()
{
  button.isr();
}
void setup()
{
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BUTTON), isrButton, CHANGE);
}
```

[Back to interface](#interface)
