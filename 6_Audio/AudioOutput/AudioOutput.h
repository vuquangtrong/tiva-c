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
    static AudioOutput& getIntance()
    {
        static AudioOutput __intance;
        return __intance;
    }

    AudioMixer* getAudioMixer();
    void output(uint16_t voltage);
    void test();

private:
    AudioOutput();
    MCP4921* _mcp4921;
    AudioMixer _audioMixer;

};

#endif /* AUDIOOUTPUT_AUDIOOUTPUT_H_ */
