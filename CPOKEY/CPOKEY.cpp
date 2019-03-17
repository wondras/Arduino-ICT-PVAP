//
// Copyright (c) 2019 Warren Ondras
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
// TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
#include "CPOKEY.h"
#include "types.h"

// POKEY has 16 addresses, read and write functions are different

// WRITE REGISTERS
static const UINT8 POKEY_W_AUDF1     = 0x0;
static const UINT8 POKEY_W_AUDC1     = 0x1;
static const UINT8 POKEY_W_AUDF2     = 0x2;
static const UINT8 POKEY_W_AUDC2     = 0x3;
static const UINT8 POKEY_W_AUDF3     = 0x4;
static const UINT8 POKEY_W_AUDC3     = 0x5;
static const UINT8 POKEY_W_AUDF4     = 0x6;
static const UINT8 POKEY_W_AUDC4     = 0x7;
static const UINT8 POKEY_W_AUDCTL    = 0x8;
static const UINT8 POKEY_W_STIMER    = 0x9;
static const UINT8 POKEY_W_SKRES     = 0xA;
static const UINT8 POKEY_W_POTGO     = 0xB;
// Write 0xC not used
static const UINT8 POKEY_W_SEROUT    = 0xD;
static const UINT8 POKEY_W_IRQEN     = 0xE;
static const UINT8 POKEY_W_SKCTL     = 0xF;

// READ REGISTERS
static const UINT8 POKEY_R_POT0      = 0x0;
static const UINT8 POKEY_R_POT1      = 0x1;
static const UINT8 POKEY_R_POT2      = 0x2;
static const UINT8 POKEY_R_POT3      = 0x3;
static const UINT8 POKEY_R_POT4      = 0x4;
static const UINT8 POKEY_R_POT5      = 0x5;
static const UINT8 POKEY_R_POT6      = 0x6;
static const UINT8 POKEY_R_POT7      = 0x7;
static const UINT8 POKEY_R_ALLPOT    = 0x8;
static const UINT8 POKEY_R_KBCODE    = 0x9;
static const UINT8 POKEY_R_RANDOM    = 0xA;
// Read 0xB not used
// Read 0xC not used
static const UINT8 POKEY_R_SERIN     = 0xD;
static const UINT8 POKEY_R_IRQSTAT   = 0xE;
static const UINT8 POKEY_R_NMIST     = 0xF;

// clock divider values for test tones
static const UINT8 POKEY_FREQ_LOW_C      = 0xF3;
static const UINT8 POKEY_FREQ_MID_C      = 0x79;
static const UINT8 POKEY_FREQ_HIGH_C     = 0x3C;
static const UINT8 POKEY_FREQ_HIGHER_C  = 0x1D;

CPOKEY::CPOKEY(
    ICpu   *cpu,
    UINT32 baseAddress
) : m_cpu(cpu),
    m_baseAddress(baseAddress)
{
}


CPOKEY::~CPOKEY(
)
{
}


//
// Return to a reasonable default state. (POKEY has no reset state, so this is not exact)
//
PERROR
CPOKEY::idle(
)
{
    PERROR error = errorSuccess;
    
    // 0x0-0x7 AUDF1-4 and AUDC1-4 to 0x00
    // 0x8 AUDCTL      0x00
    // 0x9 STIMER      any value
    // 0xA SKRES       any value
    // 0xB POTGO       NO WRITE
    // 0xC undefined   NO WRITE
    // 0xD SEROUT      0x00 (ensures serial line goes high)
    // 0xE IRQEN       0x00
    // 0xF SKCTL       0x00

    for (UINT8 reg = 0 ; reg < 16 ; reg++)
    {
        if ( (reg != 0xB) && (reg != 0xC) ) // skip the 0xB and 0xC registers
        {
            error = write(reg, 0x00);

            if (FAILED(error))
            {
                break;
            }
        }
    }
    return error;
}


//
// Simple audio test
//
PERROR
CPOKEY::soundCheck(
)
{
    PERROR error = errorSuccess;
    static const UINT32 DURATION = 1000; // millliseconds for test tone

    error = playTone(POKEY_W_AUDF1, POKEY_W_AUDC1, POKEY_FREQ_LOW_C, DURATION);
    if (SUCCESS(error)) {
        delay(DURATION/2);
        error = playTone(POKEY_W_AUDF2, POKEY_W_AUDC2, POKEY_FREQ_MID_C, DURATION);
        if (SUCCESS(error)) {
            delay(DURATION/2);
            error = playTone(POKEY_W_AUDF3, POKEY_W_AUDC3, POKEY_FREQ_HIGH_C, DURATION);
            if (SUCCESS(error)) {
                delay(DURATION/2);
                error = playTone(POKEY_W_AUDF4, POKEY_W_AUDC4, POKEY_FREQ_HIGHER_C, DURATION);
            }
        }
    }
    return error;
}

//
// Read switches on POT0-POT7 inputs
//
PERROR
CPOKEY::readSwitches(
)
{
    UINT8 switchValues;
    PERROR error = errorSuccess;
    //   set D2 of POKEY_W_SKCTL
    error = write(POKEY_W_SKCTL, 0x04);
    if SUCCESS(error)
    {
        //   strobe POKEY_W_POTGO (write any value)
        error = write(POKEY_W_POTGO, 0x00);
        if SUCCESS(error)
        {
            //   wait at least 4 clocks
            delayMicroseconds(10);
            //   read POKEY_R_ALLPOT
            error = read(POKEY_R_ALLPOT, &switchValues);
        }
    }
    if SUCCESS(error)
    {
        error = errorCustom;
        error->code = ERROR_SUCCESS;
        error->description = "OK:";
        STRING_UINT8_HEX(error->description, switchValues);
    }

    return error;
}

//
// Read a value from the random number generator
//
PERROR
CPOKEY::readRandom(
)
{
    static const int readDelayMicros = 100;
    UINT8 randomValue1, randomValue2, randomValue3;
    PERROR error = errorSuccess;

    //  set timer / polynomial params?

    // read POKEY_R_RANDOM three times
    error = read(POKEY_R_RANDOM, &randomValue1);
    if SUCCESS(error)
    {
        error = read(POKEY_R_RANDOM, &randomValue2);
        delay(readDelayMicros);
        if SUCCESS(error)
        {
            delay(readDelayMicros);
            error = read(POKEY_R_RANDOM, &randomValue3);
        }
    }
    if SUCCESS(error)
    {
        error = errorCustom;
        // if all reads are the same, something is wrong
        if ((randomValue1 == randomValue2) && (randomValue1 == randomValue3))
        {
            error->code = ERROR_FAILED;
            error->description = "E:";
        }
        else
        {
            error->code = ERROR_SUCCESS;
            error->description = "OK:";
        }
        // show the "random" bytes we read
        STRING_UINT8_HEX(error->description, randomValue1);
        STRING_UINT8_HEX(error->description, randomValue2);
        STRING_UINT8_HEX(error->description, randomValue3);
    }
    return error;
}


PERROR
CPOKEY::playTone(
    UINT8 freqReg,
    UINT8 ctrlReg,
    UINT8 freqency,
    UINT32 duration
)
{
    PERROR error = errorSuccess;
    error = write(freqReg, freqency);
    if (SUCCESS(error))
    {
  //      error = write(ctrlReg, 0x00);  // probably not needed
        delay(duration);
        error = write(freqReg, 0x00);
    }
    return error;
}

// NOT DOING (these are not used in arcade games AFAIK)
//  POT0-POT7
//  keyboard scanning
//  serial I/O
//  Timer
//  IRQ
//   NMI



PERROR
CPOKEY::read(
    UINT8 reg,
    UINT8 *data
)
{
    PERROR error = errorSuccess;
    UINT16 data16 = 0;
    
    error = m_cpu->memoryRead(m_baseAddress + reg, &data16);
    *data = (UINT8) data16;

    return error;
}


PERROR
CPOKEY::write(
    UINT8 reg,
    UINT8 data
)
{
    PERROR error = errorSuccess;

    error = m_cpu->memoryWrite(m_baseAddress + reg, (UINT16) data);

    return error;
}


