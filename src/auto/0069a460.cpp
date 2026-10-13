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

// Copies up to requestedCount bytes from the internal buffer (field_0004) starting at the
// read cursor (field_0008), clamped to the bytes remaining before field_000c, advances the
// cursor by the number of bytes copied, and returns that count.
static unsigned int __fastcall C_0085e7b8_ReadBytesFromBuffer(C_0085e7b8* self, void* /*edx*/, void* destination, int requestedCount)
{
    uint32_t readCursor = self->field_0008;
    int bytesRemaining = (int)(self->field_000c - readCursor);

    unsigned int bytesToCopy = (unsigned int)requestedCount;
    if (requestedCount > bytesRemaining)
        bytesToCopy = (unsigned int)bytesRemaining;

    const uint8_t* source = reinterpret_cast<const uint8_t*>(self->field_0004) + readCursor;
    std::memcpy(destination, source, bytesToCopy);

    self->field_0008 = self->field_0008 + bytesToCopy;
    return bytesToCopy;
}

HOOK(0x0069A460, C_0085e7b8_ReadBytesFromBuffer, "auto");
