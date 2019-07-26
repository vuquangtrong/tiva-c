/*
 * Oscillator.h
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#pragma once

#include <stdint.h>
#include "AudioGeneration/WaveformTypes.h"

class Oscillator
{
public:
    Oscillator();
    Oscillator(WaveformType waveformType, uint8_t level, int8_t cent,
               int8_t semitone);

    void MixInOscillatorAudio(uint16_t buffer[], uint32_t bufferSampleSize,
                              uint8_t totalOscillatorCount, uint8_t noteIndex,
                              uint32_t currentSample);

    WaveformType getWaveformType();
    uint8_t getLevel();
    int8_t getCent();
    int8_t getSemitone();

    void setWaveformType(WaveformType waveformType);
    void setLevel(uint8_t level);
    void setCent(int8_t cent);
    void setSemitone(int8_t semitone);

private:
    float getSamplesPerCycle(uint8_t noteIndex);

    WaveformType _waveformType;
    uint8_t _level;
    int8_t _cent;
    int8_t _semitone;
};
