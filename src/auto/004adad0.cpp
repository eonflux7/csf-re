#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns a pointer to one of two int buffers embedded in the object, selected by
// the signed 32-bit selector stored at +0x54: a negative selector selects the
// buffer at +0x34, otherwise the buffer at +0x4C is returned.
static int* __fastcall C_Object_SelectBuffer(int* self, void* /*edx*/)
{
    if (CSF_AT(int32_t, self, 0x54) < 0)
        return reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x34);
    return reinterpret_cast<int*>(reinterpret_cast<uint8_t*>(self) + 0x4C);
}

HOOK(0x4ADAD0, C_Object_SelectBuffer, "auto");
