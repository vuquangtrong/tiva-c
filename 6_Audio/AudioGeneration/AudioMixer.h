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
    AudioMixer();

    Oscillator& GetOscillator1();
    Oscillator& GetOscillator2();
    Oscillator& GetOscillator3();

    void SetMIDINote(uint8_t midiNoteIndex);
    void GetAudioData(uint16_t buffer[], uint32_t bufferSampleSize);

private:
    void SetupDefaultOscillatorValues();
    uint8_t GetActiveOscillatorCount();

    Oscillator oscillator1_;
    Oscillator oscillator2_;
    Oscillator oscillator3_;

    uint8_t midiNoteIndex_;
    static const uint8_t MIDI_NOTE_COUNT_ = 128;
    static const uint8_t NO_MIDI_NOTE_ = 0;
    //float noteFrequencyTable_[MIDI_NOTE_COUNT_];

    uint32_t sampleCount_;
};
