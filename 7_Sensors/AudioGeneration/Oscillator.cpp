/*
 * Oscillator.cpp
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#include <math.h>
#include "AudioGeneration/Oscillator.h"
#include "AudioGeneration/NoteFrequencyTable.h"

// The default constructor just sets up default values for the oscillator
Oscillator::Oscillator() :
        _waveformType(Sawtooth), _level(10), _cent(0), _semitone(0)
{
}

Oscillator::Oscillator(enum WaveformType waveformType, uint8_t level,
                       int8_t cent, int8_t semitone) :
        _waveformType(waveformType), _level(level), _cent(cent), _semitone(
                semitone)
{
}

WaveformType Oscillator::getWaveformType()
{
    return _waveformType;
}

uint8_t Oscillator::getLevel()
{
    return _level;
}

int8_t Oscillator::getCent()
{
    return _cent;
}

int8_t Oscillator::getSemitone()
{
    return _semitone;
}

void Oscillator::setWaveformType(WaveformType waveformType)
{
    _waveformType = waveformType;
}

void Oscillator::setLevel(uint8_t level)
{
    _level = level;
}

void Oscillator::setCent(int8_t cent)
{
    _cent = cent;
}

void Oscillator::setSemitone(int8_t semitone)
{
    _semitone = semitone;
}

void Oscillator::MixInOscillatorAudio(uint16_t buffer[],
                                      uint32_t bufferSampleSize,
                                      uint8_t totalOscillatorCount,
                                      uint8_t noteIndex, uint32_t currentSample)
{
    float samplesForOneCycle;
    float cyclePercent;
    float peakLevel;
    volatile uint32_t i = 0;

    if (_waveformType == None)
    {
        return;
    }

    // 12-bit DAC, max value = 0x0FFF
    peakLevel = 0x0FFF * ((float) _level / 10.0f)
            / (float) totalOscillatorCount;
    samplesForOneCycle = getSamplesPerCycle(noteIndex);

    for (i = 0; i < bufferSampleSize; ++i)
    {
        cyclePercent = fmodf(static_cast<float>(currentSample + i),
                             samplesForOneCycle) / samplesForOneCycle;

        if (_waveformType == Square && cyclePercent > 0.5)
        {
            buffer[i] += static_cast<uint16_t>(peakLevel);
        }
        else if (_waveformType == Sawtooth)
        {
            buffer[i] += static_cast<uint16_t>(peakLevel * cyclePercent);
        }
        else if (_waveformType == Sine)
        {
            buffer[i] += static_cast<uint16_t>(peakLevel * 0.5f
                    * (1 + sin(cyclePercent * 6.28f))); /* 2*pi = 6.28 */
        }
    }
}

float Oscillator::getSamplesPerCycle(uint8_t noteIndex)
{
    float frequency;
    float centAdjustment = static_cast<float>(_cent / 100.0);

    if (noteIndex == 0)
    {
        return 0.0;
    }

    uint8_t newNoteIndex = noteIndex + _semitone;
    frequency = getFrequency(newNoteIndex);
    if (centAdjustment > 0.001)
    {
        frequency += (getFrequency(newNoteIndex + 1)
                - getFrequency(newNoteIndex)) * centAdjustment;
    }
    else if (centAdjustment < -0.001)
    {
        frequency += (getFrequency(newNoteIndex)
                - getFrequency(newNoteIndex - 1)) * centAdjustment;
    }

    /* we should use 2 * 44100, but our system has some delay, we have to use a oscilloscope to tune the frequency
     * 1.85f * 44100.0f / frequency
     */
    return 81585.0f / frequency;
}

