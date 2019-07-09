/*
 * Button.c
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * MIT License
 */

#include <stdint.h>
#include "BuildConfig.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "Button.h"
#include "Logger/Logger.h"

Button::Button() :
        _active_level(ACTIVE_LOW), _tick(0), _state(StateIdle), _port(0), _pin(0), _buttonHandler(0)
{
    // calculate delay as a number of of SYSTICK
    _debounce_delay = (DEBOUNCE_DELAY * SYSTICK_PER_SECOND) / 1000;
    _singleClick_delay = (SINGLECLICK_DELAY * SYSTICK_PER_SECOND) / 1000;
    _longClick_delay = (LONGCLICK_DELAY * SYSTICK_PER_SECOND) / 1000;
    Logger::getIntance().printf("Button detection settings: %d %d %d\n\r", _debounce_delay, _singleClick_delay, _longClick_delay);
}

void Button::checkState()
{
    uint8_t value = GPIOPinRead(_port, _pin) & _pin;
    bool pressed = (_active_level == ACTIVE_HIGH ? (value > 0) : (value == 0));

    _new = false;

    // Optimization for the most common case: nothing happening.
    if (!pressed && _state == StateIdle)
    {
        _tick = 0;
        return;
    }

    _tick++;

    // Only take the low word, works as long as update() is called
    // at least every ~32s, and typical use-case is <50ms.
    // Also note that wrapping during StateIdle doesn't really matter,
    // as the time isn't used then.
    uint32_t diff = _tick - _lastTransition;

    State next = StateIdle;
    switch (_state)
    {
        case StateIdle:
            next = _checkIdle(pressed, diff);
            break;
        case StateDebounce:
            next = _checkDebounce(pressed, diff);
            break;
        case StatePressed:
            next = _checkPressed(pressed, diff);
            break;
        case StateClickUp:
            next = _checkClickUp(pressed, diff);
            break;
        case StateClickIdle:
            next = _checkClickIdle(pressed, diff);
            break;
        case StateSingleClick:
            next = _checkSingleClick(pressed, diff);
            break;
        case StateDoubleClickDebounce:
            next = _checkDoubleClickDebounce(pressed, diff);
            break;
        case StateDoubleClick:
            next = _checkDoubleClick(pressed, diff);
            break;
        case StateLongClick:
            next = _checkLongClick(pressed, diff);
            break;
        case StateOtherUp:
            next = _checkOtherUp(pressed, diff);
            break;
    }

    if (next != _state)
    {
        // Mark current state transition to make 'diff' meaningful
        _lastTransition = _tick;
        // Enter next state
        _state = next;
        // Only mark this state 'new' for one iteration, to cause
        // e.g. isClick() to return true until the next call to update()
        _new = true;
    }

    // Call handling function

    // True when any kind of click is detected (single, double or long).
    // A Click is detected at the same time as a Double or Long click, but
    // earlier than a Single click.
    if (_new && (_state == StatePressed || _state == StateDoubleClick))
    {
    }

    // True when a Single click is detected
    if (_new && _state == StateSingleClick && _buttonHandler != 0)
    {
        _buttonHandler->onClick();
    }

    // True when a Double click is detected.
    if (_new && _state == StateDoubleClick && _buttonHandler != 0)
    {
        _buttonHandler->onDoubleClick();
    }

    // True when a Long click is detected.
    if (_new && _state == StateLongClick && _buttonHandler != 0)
    {
        _buttonHandler->onHold();
    }

    // True once the button is released after Click, Long click or Double click.
    // Note: there is no release event after a Single click, because that is a
    // 'synthetic' event that happens after a normal click.
    if (_new && (_state == StateSingleClick /* StateClickUp */|| _state == StateOtherUp) && _buttonHandler != 0)
    {
        _buttonHandler->onRelease();
    }
}
Button::State Button::_checkIdle(bool pressed, uint32_t diff)
{
    (void) diff;
    // Wait for a key press
    return pressed ? StateDebounce : StateIdle;
}

Button::State Button::_checkDebounce(bool pressed, uint32_t diff)
{
    // If released in this state: it was a glitch
    if (!pressed)
    {
        return StateIdle;
    }
    if (diff >= _debounce_delay)
    {
        // Still pressed after debounce delay: real 'press'
        return StatePressed;
    }
    return StateDebounce;
}

Button::State Button::_checkPressed(bool pressed, uint32_t diff)
{
    // If released, go wait for either a double-click, or
    // to generate the actual SingleClick event,
    // but first mark that the button is released.
    if (!pressed)
    {
        return StateClickUp;
    }
    // If pressed, keep waiting to see if it's a long click
    if (diff >= _longClick_delay)
    {
        return StateLongClick;
    }
    return StatePressed;
}

Button::State Button::_checkClickUp(bool pressed, uint32_t diff)
{
    (void) pressed;
    (void) diff;
    return StateClickIdle;
}

Button::State Button::_checkClickIdle(bool pressed, uint32_t diff)
{
    if (pressed)
    {
        return StateDoubleClickDebounce;
    }
    if (diff >= _singleClick_delay)
    {
        return StateSingleClick;
    }
    return StateClickIdle;
}

Button::State Button::_checkSingleClick(bool pressed, uint32_t diff)
{
    (void) pressed;
    (void) diff;
    return StateIdle;
}

Button::State Button::_checkDoubleClickDebounce(bool pressed, uint32_t diff)
{
    if (!pressed)
    {
        return StateClickIdle;
    }
    if (diff >= _debounce_delay)
    {
        return StateDoubleClick;
    }
    return StateDoubleClickDebounce;
}

Button::State Button::_checkDoubleClick(bool pressed, uint32_t diff)
{
    (void) diff;
    if (!pressed)
    {
        return StateOtherUp;
    }
    return StateDoubleClick;
}

Button::State Button::_checkLongClick(bool pressed, uint32_t diff)
{
    (void) diff;
    if (!pressed)
    {
        return StateOtherUp;
    }
    return StateLongClick;
}

Button::State Button::_checkOtherUp(bool pressed, uint32_t diff)
{
    (void) pressed;
    (void) diff;
    return StateIdle;
}

void Button::setGPIO(uint32_t port, uint8_t pin)
{
    _port = port;
    _pin = pin;
}

void Button::setButtonHandler(ButtonHandler *buttonHandler)
{
    _buttonHandler = buttonHandler;
}
