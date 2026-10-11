#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads one byte from the stream object through its vtable slot +0x8.
// Returns the callee's result (1 when a byte was read).
static int ReadOneByteFromStream(C_0085ec74* stream, uint8_t* outByte) {
    using ReadBytesFn = int(__fastcall*)(void* self, void* edx, uint8_t* dst, int count);
    ReadBytesFn readBytes = reinterpret_cast<ReadBytesFn>(CSF_AT(void**, stream, 0)[2]);
    return readBytes(stream, nullptr, outByte, 1);
}

// Reads characters from the stream into buffer until a newline, NUL byte,
// read failure, or until maxLength - 1 characters are stored. A trailing CR
// before the newline is dropped, and the buffer is NUL-terminated.
static char* __fastcall ReadTextLine(C_0085ec74* self, void* /*edx*/, char* buffer, int maxLength) {
    if (!buffer) {
        return nullptr;
    }

    int length = 0;
    uint8_t currentByte = 0;

    if (ReadOneByteFromStream(self, &currentByte) == 1) {
        for (;;) {
            if (currentByte == 0) {
                break;
            }
            if (currentByte == 10) {
                break;
            }
            if (length >= maxLength - 1) {
                break;
            }
            buffer[length] = static_cast<char>(currentByte);
            ++length;
            if (ReadOneByteFromStream(self, &currentByte) != 1) {
                break;
            }
        }
    }

    if (currentByte == 10) {
        if (length > 0 && buffer[length - 1] == 13) {
            --length;
        }
    }

    buffer[length] = 0;
    return buffer;
}

HOOK(0x006A2AE0, ReadTextLine, "auto");
