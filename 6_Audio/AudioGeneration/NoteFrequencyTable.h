/*
 * NoteFrequencyTable.h
 *
 *  Created on: Jul 13, 2019
 *      Author: vqtrong
 */

#pragma once

#include <stdint.h>

void InitializeNoteFrequencyTable();
float GetFrequency(uint8_t noteIndex);
