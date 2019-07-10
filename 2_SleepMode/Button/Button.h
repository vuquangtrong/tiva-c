/*
 * Button.h
 *
 *  Created on: Jul 8, 2019
 *      Author: vqtrong
 *
 * This class is modified version of https://github.com/poelstra/arduino-multi-button
 *
 * MIT License
 */

#ifndef BUTTON_BUTTON_H_
#define BUTTON_BUTTON_H_

#pragma once

#include <stdint.h>
#include "ButtonHandler.h"

// below definition will be use to calculate the number of SYSTICK
#define DEBOUNCE_DELAY      20 // milliseconds
#define SINGLECLICK_DELAY   200 // milliseconds
#define LONGCLICK_DELAY     600 // milliseconds

class Button
{
    public:
        enum
        {
            ACTIVE_LOW, // press down makes input = 0
            ACTIVE_HIGH // press down makes input = Vcc
        };

        Button();
        void checkState();
        void setActiveLevel(uint8_t level);
        void setGPIO(uint32_t port, uint8_t pin);
        void setButtonHandler(ButtonHandler* buttonHandler);

    private:

        /**
         * Note:
         * - SingleClick is only emitted when it's definitely not going to be a double or long click
         * - Click is emitted when either Pressed or DoubleClick happens (so it's emitted whenever any
         *   click happens, but earlier than the SingleClick or LongClick is detected)
         *
         * Several example patterns and resulting states.
         * Delays aren't at scale. State transitions when input
         * changes are based on that input, transitions when
         * input didn't change are due to timeouts.
         * Start state is Idle.
         *
         * Clean single click:
         * _______########___________________
         *        |   |   ||      |Idle
         *        |   |   ||      SingleClick
         *        |   |   |ClickIdle
         *        |   |   ClickUp
         *        |   Pressed
         *        Debounce
         *
         * Clean double click:
         * _______########______########_____
         *        |   |   ||    |   |   |Idle
         *        |   |   ||    |   |   OtherUp
         *        |   |   ||    |   DoubleClick
         *        |   |   ||    DoubleClickDebounce
         *        |   |   |ClickIdle
         *        |   |   ClickUp
         *        |   Pressed
         *        Debounce
         *
         * Clean long click:
         * _______####################_______
         *        |   |         |     |Idle
         *        |   |         |     OtherUp
         *        |   |         LongClick
         *        |   Pressed
         *        Debounce
         *
         * Single click with bouncing:
         * ___#_#_########__#_#_______________
         *    |||||   |   ||||||      |Idle
         *    |||||   |   ||||||      SingleClick
         *    |||||   |   |||||ClickIdle
         *    |||||   |   ||||DoubleClickDebounce
         *    |||||   |   |||ClickIdle
         *    |||||   |   ||DoubleClickDebounce
         *    |||||   |   |ClickIdle
         *    |||||   |   ClickUp
         *    |||||   Pressed
         *    ||||Debounce
         *    |||Idle
         *    ||Debounce
         *    |Idle
         *    Debounce
         */

        enum State
        {
            StateIdle,
            StateDebounce,
            StatePressed,
            StateClickUp,
            StateClickIdle,
            StateSingleClick,
            StateDoubleClickDebounce,
            StateDoubleClick,
            StateLongClick,
            StateOtherUp,
        };

        uint32_t _port;
        uint8_t _pin;
        uint8_t _active_level;
        ButtonHandler* _buttonHandler;

        uint32_t _debounce_delay;
        uint32_t _singleClick_delay;
        uint32_t _longClick_delay;
        uint32_t _lastTransition;
        uint32_t _tick;
        State _state;
        bool _new;

        State _checkIdle(bool pressed, uint32_t diff);
        State _checkDebounce(bool pressed, uint32_t diff);
        State _checkPressed(bool pressed, uint32_t diff);
        State _checkClickUp(bool pressed, uint32_t diff);
        State _checkClickIdle(bool pressed, uint32_t diff);
        State _checkSingleClick(bool pressed, uint32_t diff);
        State _checkDoubleClickDebounce(bool pressed, uint32_t diff);
        State _checkDoubleClick(bool pressed, uint32_t diff);
        State _checkLongClick(bool pressed, uint32_t diff);
        State _checkOtherUp(bool pressed, uint32_t diff);
};
#endif /* BUTTON_BUTTON_H_ */
