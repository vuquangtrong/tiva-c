//*****************************************************************************
//
// bl_ssi.c - Functions used to transfer data via the SSI port.
//
// Copyright (c) 2006-2017 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 2.1.4.178 of the Tiva Firmware Development Package.
//
//*****************************************************************************

#include <stdint.h>
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "bl_config.h"
#include "boot_loader/bl_ssi.h"

//*****************************************************************************
//
//! \addtogroup bl_ssi_api
//! @{
//
//*****************************************************************************
#if defined(SSI_ENABLE_UPDATE) || defined(DOXYGEN)

//*****************************************************************************
//
//! Sends data via the SSI port in slave mode.
//!
//! \param pui8Data is the location of the data to send through the SSI port.
//! \param ui32Size is the number of bytes of data to send.
//!
//! This function sends data through the SSI port in slave mode.  This function
//! will not return until all bytes are sent.
//!
//! \return None.
//
//*****************************************************************************
void
SSISend(const uint8_t *pui8Data, uint32_t ui32Size)
{
    //
    // Send the requested number of bytes over the SSI port.
    //
    while(ui32Size--)
    {
        //
        // Wait until there is space in the SSI FIFO.
        //
        while(!(HWREG(SSIx_BASE + SSI_O_SR) & SSI_SR_TNF))
        {
        }

        //
        // Write the next byte to the SSI port.
        //
        HWREG(SSIx_BASE + SSI_O_DR) = *pui8Data++;
    }

    //
    // Empty the receive FIFO.
    //
    while(HWREG(SSIx_BASE + SSI_O_SR) & SSI_SR_RNE)
    {
        HWREG(SSIx_BASE + SSI_O_DR);
    }
}

//*****************************************************************************
//
//! Waits until all data has been transmitted by the SSI port.
//!
//! This function waits until all data written to the SSI port has been read by
//! the master.
//!
//! \return None.
//
//*****************************************************************************
void
SSIFlush(void)
{
    //
    // Wait until the transmit FIFO is empty.
    //
    while(!(HWREG(SSIx_BASE + SSI_O_SR) & SSI_SR_TFE))
    {
    }

    //
    // Wait until the interface is not busy.
    //
    while(HWREG(SSIx_BASE + SSI_O_SR) & SSI_SR_BSY)
    {
    }
}

//*****************************************************************************
//
//! Receives data from the SSI port in slave mode.
//!
//! \param pui8Data is the location to store the data received from the SSI
//! port.
//! \param ui32Size is the number of bytes of data to receive.
//!
//! This function receives data from the SSI port in slave mode.  The function
//! will not return until \e ui32Size number of bytes have been received.
//!
//! \return None.
//
//*****************************************************************************
void
SSIReceive(uint8_t *pui8Data, uint32_t ui32Size)
{
    //
    // Ensure that we are sending out zeros so that we don't confuse the host.
    //
    HWREG(SSIx_BASE + SSI_O_DR) = 0;

    //
    // Wait for the requested number of bytes.
    //
    while(ui32Size--)
    {
        //
        // Wait until there is data in the FIFO.
        //
        while(!(HWREG(SSIx_BASE + SSI_O_SR) & SSI_SR_RNE))
        {
        }

        //
        // Read the next byte from the FIFO.
        //
        *pui8Data++ = HWREG(SSIx_BASE + SSI_O_DR);
        HWREG(SSIx_BASE + SSI_O_DR) = 0;
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
#endif
