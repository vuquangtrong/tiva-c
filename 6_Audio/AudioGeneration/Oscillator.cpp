/*
 * Oscillator.cpp
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#include "AudioGeneration/Oscillator.h"
#include "AudioGeneration/NoteFrequencyTable.h"
#include <math.h>

// The default constructor just sets up default values for the oscillator
Oscillator::Oscillator() :
        waveformType_(Sawtooth), level_(10), cent_(0), semitone_(0)
{
}

Oscillator::Oscillator(enum WaveformType waveformType, uint8_t level,
                       int8_t cent, int8_t semitone) :
        waveformType_(waveformType), level_(level), cent_(cent), semitone_(
                semitone)
{
}

WaveformType Oscillator::GetWaveformType()
{
    return waveformType_;
}

uint8_t Oscillator::GetLevel()
{
    return level_;
}

int8_t Oscillator::GetCent()
{
    return cent_;
}

int8_t Oscillator::GetSemitone()
{
    return semitone_;
}

void Oscillator::SetWaveformType(WaveformType waveformType)
{
    waveformType_ = waveformType;
}

void Oscillator::SetLevel(uint8_t level)
{
    level_ = level;
}

void Oscillator::SetCent(int8_t cent)
{
    cent_ = cent;
}

void Oscillator::SetSemitone(int8_t semitone)
{
    semitone_ = semitone;
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

    if (waveformType_ == None)
    {
        return;
    }

    // 12-bit DAC
    peakLevel = 0x0FFF * ((float) level_ / 10.0f)
            / (float) totalOscillatorCount;
    samplesForOneCycle = GetSamplesPerCycle(noteIndex);

    for (i = 0; i < bufferSampleSize; ++i)
    {
        cyclePercent = fmodf(static_cast<float>(currentSample + i),
                             samplesForOneCycle) / samplesForOneCycle;

        if (waveformType_ == Square && cyclePercent > 0.5)
        {
            buffer[i] += static_cast<uint16_t>(peakLevel);
        }
        else if (waveformType_ == Sawtooth)
        {
            buffer[i] += static_cast<uint16_t>(peakLevel * cyclePercent);
        }
    }
}

float Oscillator::GetSamplesPerCycle(uint8_t noteIndex)
{
    float frequency;
    float centAdjustment = static_cast<float>(cent_ / 100.0);

    if (noteIndex == 0)
    {
        return 0.0;
    }

    uint8_t newNoteIndex = noteIndex + semitone_;
    frequency = GetFrequency(newNoteIndex);
    if (centAdjustment > 0.001)
    {
        frequency += (GetFrequency(newNoteIndex + 1)
                - GetFrequency(newNoteIndex)) * centAdjustment;
    }
    else if (centAdjustment < -0.001)
    {
        frequency += (GetFrequency(newNoteIndex)
                - GetFrequency(newNoteIndex - 1)) * centAdjustment;
    }

    return 44100.0f / frequency;
}

