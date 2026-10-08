#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// Stream vtable layout as used here:
//   slot 2 (+0x08): Read(buffer, byteCount) -> int (bytes read), thiscall
//   slot 6 (+0x18): status query, returns a byte; 0 means "ok", thiscall

static int MenuExtras_StreamReadBytes(void* stream, void* buffer, int byteCount)
{
    return ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, stream, 0)[2])(stream, nullptr, buffer, byteCount);
}

static uint8_t MenuExtras_StreamStatusByte(void* stream)
{
    return ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, stream, 0)[6])(stream, nullptr);
}

// Reads one line of 16-bit characters from the stream into outWords (at most maxWords kept).
// The output region is zeroed first, using the original's 32-bit byte count (2 * maxWords,
// wrapping), so a negative maxWords clears the same (huge) region the original does.
// Leading whitespace (space, tab, CR, LF) is dropped; a CR LF pair terminates the line once
// some characters have been stored.
// Returns false if the first read fails; otherwise returns true when the stream status byte is 0.
static bool __cdecl MenuExtras_ReadWideTextLine(int streamPtr, uint16_t* outWords, int maxWords)
{
    void* stream = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(streamPtr)));

    // Original: ecx = maxWords; edx = ecx * 2 (32-bit wrap); rep stosd (edx >> 2); rep stosb (edx & 3).
    // Together these zero exactly edx bytes starting at outWords.
    const uint32_t clearByteCount = static_cast<uint32_t>(maxWords) * 2u;
    uint8_t* clearBytes = reinterpret_cast<uint8_t*>(outWords);
    for (uint32_t i = 0; i < clearByteCount; ++i) {
        clearBytes[i] = 0;
    }

    uint16_t prevChar = 0;
    if (MenuExtras_StreamReadBytes(stream, &prevChar, 2) != 2) {
        return false;
    }

    int wordCount = 0;
    uint16_t nextChar = 0;
    while (MenuExtras_StreamReadBytes(stream, &nextChar, 2) == 2) {
        if (prevChar == 13 && nextChar == 10) {
            if (wordCount != 0) {
                return MenuExtras_StreamStatusByte(stream) == 0;
            }
            prevChar = 10;
        } else {
            bool isSkippableWhitespace = prevChar == 32 || prevChar == 9 || prevChar == 13 || prevChar == 10;
            if ((wordCount != 0 || !isSkippableWhitespace) && wordCount < maxWords) {
                outWords[wordCount++] = prevChar;
            }
            prevChar = nextChar;
        }
    }

    return MenuExtras_StreamStatusByte(stream) == 0;
}

HOOK(0x545E00, MenuExtras_ReadWideTextLine, "auto");
