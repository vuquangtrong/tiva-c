/*
 * AudioOutput.h
 *
 *  Created on: Jul 14, 2019
 *      Author: vqtrong
 */

#ifndef AUDIOOUTPUT_AUDIOOUTPUT_H_
#define AUDIOOUTPUT_AUDIOOUTPUT_H_

#pragma once

#include <stdint.h>
#include "BuildConfig.h"
#include "MCP4921.h"
#include "AudioGeneration/AudioMixer.h"

class AudioOutput
{
public:
    static AudioOutput& getInstance()
    {
        static AudioOutput __instance;
        return __instance;
    }

    AudioMixer* getAudioMixer();
    void playSampleAudio();

private:
    MCP4921* _mcp4921;
    AudioMixer _audioMixer;

    AudioOutput();
};

#endif /* AUDIOOUTPUT_AUDIOOUTPUT_H_ */
