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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, UpdateGameDataRecordLinks, 0x007754B0, (int a1, int a2, int a3, int a4));

// Resets the two owned buffers of a table record: the string buffer at +0x3C
// (capacity +0x50) and the link/data buffer at +0x24 (sub-object at +0x20),
// then the small buffer at +0x08 (capacity +0x1C). Returns the last value of
// the original's result register.
static int __fastcall Tables_ClearRecordBuffers(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x50) >= 0x10u && CSF_AT(void*, self, 0x3C) != nullptr)
        Mem_FreeDebug(CSF_AT(void*, self, 0x3C));

    CSF_AT(uint32_t, self, 0x50) = 15;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint8_t, self, 0x3C) = 0;

    int result = 0;
    if (CSF_AT(int, self, 0x24) != 0) {
        UpdateGameDataRecordLinks(CSF_AT(int, self, 0x24),
                                  CSF_AT(int, self, 0x28),
                                  static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + 0x20)),
                                  0);
        result = CSF_AT(int, self, 0x24);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
    }

    CSF_AT(int, self, 0x24) = 0;
    CSF_AT(int, self, 0x28) = 0;
    CSF_AT(int, self, 0x2C) = 0;

    if (CSF_AT(uint32_t, self, 0x1C) >= 0x10u) {
        result = CSF_AT(int, self, 0x08);
        if (result != 0)
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(result)));
    }

    CSF_AT(int, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 15;
    CSF_AT(uint8_t, self, 0x08) = 0;

    return result;
}

HOOK(0x0051ABD0, Tables_ClearRecordBuffers, "auto");
