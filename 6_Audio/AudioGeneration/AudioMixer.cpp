/*
 * AudioMixer.cpp
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#include "AudioGeneration/AudioMixer.h"
#include "AudioGeneration/Oscillator.h"
#include "AudioGeneration/NoteFrequencyTable.h"
#include <math.h>
#include <stddef.h>

AudioMixer::AudioMixer() :
        sampleCount_(0), midiNoteIndex_(NO_MIDI_NOTE_)
{
    InitializeNoteFrequencyTable();
    SetupDefaultOscillatorValues();
}

void AudioMixer::SetupDefaultOscillatorValues()
{
    oscillator2_.SetWaveformType(WaveformType::None);
    oscillator2_.SetCent(0);

    oscillator3_.SetWaveformType(WaveformType::None);
    oscillator3_.SetCent(0);
}

Oscillator& AudioMixer::GetOscillator1()
{
    return oscillator1_;
}

Oscillator& AudioMixer::GetOscillator2()
{
    return oscillator2_;
}

Oscillator& AudioMixer::GetOscillator3()
{
    return oscillator3_;
}

void AudioMixer::SetMIDINote(uint8_t midiNoteIndex)
{
    if (midiNoteIndex < MIDI_NOTE_COUNT_)
    {
        midiNoteIndex_ = midiNoteIndex;
    }
    else
    {
        midiNoteIndex_ = MIDI_NOTE_COUNT_ - 1;
    }
}

uint8_t AudioMixer::GetActiveOscillatorCount()
{
    int oscillatorCount = 0;

    if (oscillator1_.GetWaveformType() != None)
    {
        ++oscillatorCount;
    }
    if (oscillator2_.GetWaveformType() != None)
    {
        ++oscillatorCount;
    }
    if (oscillator3_.GetWaveformType() != None)
    {
        ++oscillatorCount;
    }

    return oscillatorCount;
}

void AudioMixer::GetAudioData(uint16_t buffer[], uint32_t bufferSampleSize)
{
    // Silent the audio buffer before adding audio to it through the oscillators
    for (unsigned int i = 0; i < bufferSampleSize; ++i)
    {
        buffer[i] = 0;
    }

    // If no note is being played, no audio needed
    if (midiNoteIndex_ == NO_MIDI_NOTE_)
    {
        return;
    }

    uint8_t totalOscillatorCount = GetActiveOscillatorCount();

    oscillator1_.MixInOscillatorAudio(buffer, bufferSampleSize,
                                      totalOscillatorCount, midiNoteIndex_,
                                      sampleCount_);
    oscillator2_.MixInOscillatorAudio(buffer, bufferSampleSize,
                                      totalOscillatorCount, midiNoteIndex_,
                                      sampleCount_);
    oscillator3_.MixInOscillatorAudio(buffer, bufferSampleSize,
                                      totalOscillatorCount, midiNoteIndex_,
                                      sampleCount_);

    // 12bit DAC
    for (unsigned int i = 0; i < bufferSampleSize; ++i)
    {
        buffer[i] = (0x7000 | (buffer[i] >> 4));
    }

    sampleCount_ += bufferSampleSize;
}
