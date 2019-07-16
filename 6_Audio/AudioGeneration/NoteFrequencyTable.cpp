/*
 * NoteFrequencyTable.cpp
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#include "AudioGeneration/NoteFrequencyTable.h"
#include <math.h>

// For more info about the following values please see https://en.wikipedia.org/wiki/MIDI_tuning_standard
float twoToTheOne12th_ = 1.059463094f;
float a440Hz_ = 440.0;
int a440Index_ = 57;

// See MIDI standard, it seems to be generally agreed that MIDI notes range in index from 
// 0-to-127 with middle C at 60.
const int indexCount = 128;

float noteFrequency = 0.0;
float noteFrequencyTable_[indexCount];

void InitializeNoteFrequencyTable()
{
    volatile int i = 0;

    for(; i < indexCount; ++i)
    {
        noteFrequencyTable_[i] = a440Hz_ * powf(twoToTheOne12th_, static_cast<float>(i - a440Index_));
    }
}

float GetFrequency(uint8_t noteIndex)
{
    if(noteIndex > indexCount)
    {
        return 0.0;
    }

    return noteFrequencyTable_[noteIndex];
}
