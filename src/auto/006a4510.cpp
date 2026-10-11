#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Stream context accessor: takes the context in eax and returns it in eax.
CSF_FUNC(int, __cdecl, LogStream_GetContext, 0x6A44A0, (int context));

// Writes a zlib stored-block header (LEN, NLEN, little-endian) into the log stream
// buffer, then copies `length` raw bytes from `data`. The header is written only
// when `writeHeader` is nonzero.
static int* __cdecl LogStream_WriteStoredBlock(int context, uint8_t* data, uint32_t length, int writeHeader)
{
    int* stream = reinterpret_cast<int*>(LogStream_GetContext(context));
    CSF_AT(uint32_t, stream, 0x16AC) = 8;

    if (writeHeader) {
        uint8_t* buffer = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stream, 0x8));
        uint32_t index = CSF_AT(uint32_t, stream, 0x14);
        buffer[index] = static_cast<uint8_t>(length);
        ++index;
        CSF_AT(uint32_t, stream, 0x14) = index;

        buffer = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stream, 0x8));
        buffer[index] = static_cast<uint8_t>(length >> 8);
        ++index;
        CSF_AT(uint32_t, stream, 0x14) = index;

        buffer = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stream, 0x8));
        buffer[index] = static_cast<uint8_t>(~length);
        ++index;
        CSF_AT(uint32_t, stream, 0x14) = index;

        buffer = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stream, 0x8));
        buffer[index] = static_cast<uint8_t>(~length >> 8);
        ++index;
        CSF_AT(uint32_t, stream, 0x14) = index;
    }

    uint32_t remaining = length;
    while (remaining != 0) {
        uint8_t* buffer = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stream, 0x8));
        uint32_t index = CSF_AT(uint32_t, stream, 0x14);
        buffer[index] = *data;
        ++data;
        --remaining;
        CSF_AT(uint32_t, stream, 0x14) = index + 1;
    }

    return stream;
}

HOOK(0x006A4510, LogStream_WriteStoredBlock, "auto");
