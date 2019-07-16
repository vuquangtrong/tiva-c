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

    WaveformType GetWaveformType();
    uint8_t GetLevel();
    int8_t GetCent();
    int8_t GetSemitone();

    void SetWaveformType(WaveformType waveformType);
    void SetLevel(uint8_t level);
    void SetCent(int8_t cent);
    void SetSemitone(int8_t semitone);

private:
    float GetSamplesPerCycle(uint8_t noteIndex);

    WaveformType waveformType_;
    uint8_t level_;
    int8_t cent_;
    int8_t semitone_;
};
