/*
 * AudioMixer.h
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#pragma once

#include <stdint.h>
#include <AudioGeneration/Oscillator.h>

class Oscillator;

class AudioMixer
{
public:
    enum Settings
    {
        NO_MIDI_NOTE = 0, MIDI_NOTE_COUNT = 128
    };

    AudioMixer();
    Oscillator& getOscillator1();
    Oscillator& getOscillator2();
    Oscillator& getOscillator3();
    void setMIDINote(uint8_t midiNoteIndex);
    void getAudioData(uint16_t buffer[], uint32_t bufferSampleSize);

private:
    Oscillator _oscillator1;
    Oscillator _oscillator2;
    Oscillator _oscillator3;
    uint32_t _sampleCount;
    uint8_t _midiNoteIndex;

    void setupDefaultOscillatorValues();
    uint8_t getActiveOscillatorCount();
};
