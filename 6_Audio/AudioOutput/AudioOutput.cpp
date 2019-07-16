/*
 * AudioOutput.cpp
 *
 *  Created on: Jul 14, 2019
 *      Author: vqtrong
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_ssi.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/ssi.h"
#include "driverlib/udma.h"
#include "driverlib/interrupt.h"
#include "AudioOutput.h"
#include "AudioSample.h"
#include "Logger/Logger.h"

#pragma DATA_ALIGN(1024)
uint8_t __controlTable[1024];

#define AUDIO_BUFFER_SIZE    256
uint16_t __primaryAudioBuffer[AUDIO_BUFFER_SIZE];
uint16_t __alternateAudioBuffer[AUDIO_BUFFER_SIZE];

void SSI1IntHandler(void)
{
    // clear interrupt
    uint32_t status;
    status = SSIIntStatus(SSI1_BASE, 1);
    SSIIntClear(SSI1_BASE, status);

    // check completed DMA channel
    uint32_t mode;
    mode = uDMAChannelModeGet(UDMA_CHANNEL_SSI1TX | UDMA_PRI_SELECT);
    if (mode == UDMA_MODE_STOP)
    {
        // refill buffer
        AudioOutput::getIntance().getAudioMixer()->GetAudioData(
                __primaryAudioBuffer, AUDIO_BUFFER_SIZE);
        // re-start dma
        uDMAChannelTransferSet(
        UDMA_CHANNEL_SSI1TX | UDMA_PRI_SELECT,
                               UDMA_MODE_PINGPONG, __primaryAudioBuffer,
                               (void *) (SSI1_BASE + SSI_O_DR),
                               AUDIO_BUFFER_SIZE);
        uDMAChannelEnable(UDMA_CHANNEL_SSI1TX);
    }

    mode = uDMAChannelModeGet(UDMA_CHANNEL_SSI1TX | UDMA_ALT_SELECT);
    if (mode == UDMA_MODE_STOP)
    {
        // refill buffer
        AudioOutput::getIntance().getAudioMixer()->GetAudioData(
                __alternateAudioBuffer, AUDIO_BUFFER_SIZE);
        // re-start dma
        uDMAChannelTransferSet(
        UDMA_CHANNEL_SSI1TX | UDMA_ALT_SELECT,
                               UDMA_MODE_PINGPONG, __alternateAudioBuffer,
                               (void *) (SSI1_BASE + SSI_O_DR),
                               AUDIO_BUFFER_SIZE);
        uDMAChannelEnable(UDMA_CHANNEL_SSI1TX);
    }
}

AudioOutput::AudioOutput()
{
    // turn on DAC chip
    _mcp4921 = &MCP4921::getIntance();
    _audioMixer.SetMIDINote(69);

#ifndef TEST_AUDIO
    // turn on DMA
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_UDMA);
    uDMAEnable();
    uDMAControlBaseSet(__controlTable);

    // turn on DMA of SSI1
    SSIDMAEnable(SSI1_BASE, SSI_DMA_TX);

    // setup DMA on SSI1
    uDMAChannelAttributeDisable(
    UDMA_CHANNEL_SSI1TX,
                                UDMA_ATTR_HIGH_PRIORITY | UDMA_ATTR_REQMASK);

    uDMAChannelAttributeEnable(UDMA_CHANNEL_SSI1TX,
    UDMA_ATTR_ALTSELECT | UDMA_ATTR_USEBURST);

    uDMAChannelControlSet(
            UDMA_CHANNEL_SSI1TX | UDMA_PRI_SELECT,
            UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_4);

    uDMAChannelTransferSet(UDMA_CHANNEL_SSI1TX | UDMA_PRI_SELECT,
    UDMA_MODE_PINGPONG,
                           __primaryAudioBuffer,
                           (void *) (SSI1_BASE + SSI_O_DR),
                           AUDIO_BUFFER_SIZE);

    uDMAChannelControlSet(
            UDMA_CHANNEL_SSI1TX | UDMA_ALT_SELECT,
            UDMA_SIZE_16 | UDMA_SRC_INC_16 | UDMA_DST_INC_NONE | UDMA_ARB_4);

    uDMAChannelTransferSet(UDMA_CHANNEL_SSI1TX | UDMA_ALT_SELECT,
    UDMA_MODE_PINGPONG,
                           __alternateAudioBuffer,
                           (void *) (SSI1_BASE + SSI_O_DR),
                           AUDIO_BUFFER_SIZE);

    // enable SSI1's DMA
    uDMAChannelEnable(UDMA_CHANNEL_SSI1TX);

    // enable SSI1's interrupt
    SSIIntRegister(SSI1_BASE, SSI1IntHandler);
    SSIIntEnable(SSI1_BASE, SSI_DMATX);
    IntEnable(INT_SSI1);
#endif
}

AudioMixer* AudioOutput::getAudioMixer()
{
    return &_audioMixer;
}

void AudioOutput::output(uint16_t voltage)
{
    _mcp4921->output(voltage);
}

void AudioOutput::test()
{
#ifdef TEST_AUDIO
    uint16_t i = 0;
    while (true)
    {
        for (i = 0; i < SAMPLE_ELEMENTS; i++)
        {
            output(__audioSamples[i]);
        }
        Logger::getIntance().print(">");
    }
#endif
}
