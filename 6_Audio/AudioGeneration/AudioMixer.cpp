/*
 * AudioMixer.cpp
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include "driverlib/sysctl.h"
#include "BuildConfig.h"
#include "AudioGeneration/AudioMixer.h"
#include "AudioGeneration/Oscillator.h"
#include "AudioGeneration/NoteFrequencyTable.h"
#include "Logger/Logger.h"

AudioMixer::AudioMixer() :
        _sampleCount(0), _midiNoteIndex(NO_MIDI_NOTE)
{
    initializeNoteFrequencyTable();
    setupDefaultOscillatorValues();
}

void AudioMixer::setupDefaultOscillatorValues()
{
    _oscillator1.setWaveformType(WaveformType::Sine);
    _oscillator1.setCent(0);

    _oscillator2.setWaveformType(WaveformType::None);
    _oscillator2.setCent(0);

    _oscillator3.setWaveformType(WaveformType::None);
    _oscillator3.setCent(0);
}

Oscillator& AudioMixer::getOscillator1()
{
    return _oscillator1;
}

Oscillator& AudioMixer::getOscillator2()
{
    return _oscillator2;
}

Oscillator& AudioMixer::getOscillator3()
{
    return _oscillator3;
}

void AudioMixer::setMIDINote(uint8_t midiNoteIndex)
{
    if (midiNoteIndex < MIDI_NOTE_COUNT)
    {
        _midiNoteIndex = midiNoteIndex;
    }
    else
    {
        _midiNoteIndex = MIDI_NOTE_COUNT - 1;
    }
}

uint8_t AudioMixer::getActiveOscillatorCount()
{
    int oscillatorCount = 0;

    if (_oscillator1.getWaveformType() != None)
    {
        ++oscillatorCount;
    }
    if (_oscillator2.getWaveformType() != None)
    {
        ++oscillatorCount;
    }
    if (_oscillator3.getWaveformType() != None)
    {
        ++oscillatorCount;
    }

    return oscillatorCount;
}

void AudioMixer::getAudioData(uint16_t buffer[], uint32_t bufferSampleSize)
{
    volatile uint32_t i = 0;

    // Silent the audio buffer before adding audio to it through the oscillators
    for (i = 0; i < bufferSampleSize; ++i)
    {
#if PROBE_AUDIO_WAVE
        buffer[i] = 0;
#else
        buffer[i] = 0x7000; /* ACTIVE */
#endif
    }

    // If no note is being played, no audio needed
    if (_midiNoteIndex == NO_MIDI_NOTE)
    {
        return;
    }

    uint8_t totalOscillatorCount = getActiveOscillatorCount();

    _oscillator1.MixInOscillatorAudio(buffer, bufferSampleSize,
                                      totalOscillatorCount, _midiNoteIndex,
                                      _sampleCount);
    _oscillator2.MixInOscillatorAudio(buffer, bufferSampleSize,
                                      totalOscillatorCount, _midiNoteIndex,
                                      _sampleCount);
    _oscillator3.MixInOscillatorAudio(buffer, bufferSampleSize,
                                      totalOscillatorCount, _midiNoteIndex,
                                      _sampleCount);

#if PROBE_AUDIO_WAVE
    for (i = 0; i < bufferSampleSize; ++i)
    {
        Logger::getInstance().printf("%d\n\r", buffer[i]);
        SysCtlDelay(200000);
    }
#endif
    _sampleCount += bufferSampleSize;
}
