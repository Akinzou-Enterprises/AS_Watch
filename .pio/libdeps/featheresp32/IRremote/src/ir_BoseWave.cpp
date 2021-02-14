/*
 * ir_BoseWave.cpp
 *
 *  Contains functions for receiving and sending Bose IR Protocol
 *
 *  This file is part of Arduino-IRremote https://github.com/z3t0/Arduino-IRremote.
 *
 */

//#define DEBUG // Activate this  for lots of lovely debug output.
#include "IRremote.h"

//==============================================================================
//                           BBBB    OOO    SSSS  EEEEE
//                           B   B  O   O  S      E
//                           BB B   O   O   SSS   EEEE
//                           B   B  O   O      S  E
//                           BBBB    OOO   SSSS   EEEEE
//==============================================================================
// see http://lirc.sourceforge.net/remotes/bose/WAVERADIO
//
// Support for Bose Wave Radio CD initially provided by https://github.com/uvotguy.
//
// As seen on my trusty oscilloscope, there is no repeat code.  Instead, when I
// press and hold a button on my remote, it sends a command, makes a 51.2ms space,
// and resends the command, etc, etc.

// LSB first, 1 start bit + 8 bit data + 8 bit inverted data + 1 stop bit.
#define BOSEWAVE_BITS             16 // Command and inverted command

#define BOSEWAVE_HEADER_MARK    1060
#define BOSEWAVE_HEADER_SPACE   1450
#define BOSEWAVE_BIT_MARK        534
#define BOSEWAVE_ONE_SPACE       468
#define BOSEWAVE_ZERO_SPACE     1447

#define BOSEWAVE_REPEAT_SPACE  52000

//+=============================================================================

void IRsend::sendBoseWaveStandard(uint8_t aCommand, uint8_t aNumberOfRepeats) {
    // Set IR carrier frequency
    enableIROut(38);

    uint8_t tNumberOfCommands = aNumberOfRepeats + 1;
    while (tNumberOfCommands > 0) {
        // Header
        mark(BOSEWAVE_HEADER_MARK);
        space(BOSEWAVE_HEADER_SPACE);
        // send 8 command bits and then 8 inverted command bits LSB first
        uint16_t tData = ((~aCommand) << 8) | aCommand;

        sendPulseDistanceWidthData(BOSEWAVE_BIT_MARK, BOSEWAVE_ONE_SPACE, BOSEWAVE_BIT_MARK, BOSEWAVE_ZERO_SPACE, tData,
        BOSEWAVE_BITS, false);

        mark(BOSEWAVE_BIT_MARK); // Stop bit
        space(0);  // Always end with the LED off

        tNumberOfCommands--;
        // skip last delay!
        if (tNumberOfCommands > 0) {
            // send repeated command with a fixed space gap
            delay( BOSEWAVE_REPEAT_SPACE / 1000);
        }
    }
}

//+=============================================================================
bool IRrecv::decodeBoseWave() {
    uint8_t tOffset = 1;  // Index in to results; Skip first space.

    // Check header "mark"
    if (!MATCH_MARK(results.rawbuf[tOffset], BOSEWAVE_HEADER_MARK)) {
        // no debug output, since this check is mainly to determine the received protocol
        return false;
    }
    tOffset++;

    // Check we have enough data +4 for initial gap, start bit mark and space + stop bit mark
    if (results.rawlen != (2 * BOSEWAVE_BITS) + 4) {
        DBG_PRINT("Bose: ");
        DBG_PRINT("Data length=");
        DBG_PRINT(results.rawlen);
        DBG_PRINTLN(" is not 36");
        return false;
    }
    // Check header "space"
    if (!MATCH_SPACE(results.rawbuf[tOffset], BOSEWAVE_HEADER_SPACE)) {
        DBG_PRINT("Bose: ");
        DBG_PRINTLN("Header space length is wrong");
        return false;
    }
    tOffset++;

    if (!decodePulseDistanceData(BOSEWAVE_BITS, tOffset, BOSEWAVE_BIT_MARK, BOSEWAVE_ONE_SPACE, BOSEWAVE_ZERO_SPACE, false)) {
        DBG_PRINT("Bose: ");
        DBG_PRINTLN("Decode failed");
        return false;
    }

    // Stop bit
    if (!MATCH_MARK(results.rawbuf[tOffset + (2 * BOSEWAVE_BITS)], BOSEWAVE_BIT_MARK)) {
        DBG_PRINT("Bose: ");
        DBG_PRINTLN("Stop bit verify failed");
        return false;
    }

    // Success
    uint16_t tDecodedValue = results.value;
    uint8_t tCommandNotInverted = tDecodedValue & 0xFF;
    uint8_t tCommandInverted = tDecodedValue >> 8;
    // parity check for command. Use this variant to avoid compiler warning "comparison of promoted ~unsigned with unsigned [-Wsign-compare]"
        if ((tCommandNotInverted ^ tCommandInverted) != 0xFF) {
        DBG_PRINT("Bose: ");
        DBG_PRINT("Command and inverted command check failed");
        return false;
    }

    // check for repeat
    if (results.rawbuf[0] < ((BOSEWAVE_REPEAT_SPACE + (BOSEWAVE_REPEAT_SPACE / 4)) / MICROS_PER_TICK)) {
        decodedIRData.flags = IRDATA_FLAGS_IS_REPEAT;
    }

    decodedIRData.command = tCommandNotInverted;
    decodedIRData.protocol = BOSEWAVE;
    decodedIRData.numberOfBits = BOSEWAVE_BITS;

    return true;
}
