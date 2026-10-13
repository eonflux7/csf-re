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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, S_0041e990_ReleaseAllEntries, 0x0041E990, (S_0041e990* self, void*));

// Clears the buffer bookkeeping fields, frees the buffer if one is held,
// then releases all entries of the object stored at +0x1C (tail call).
static int __fastcall ResetBufferAndReleaseEntries(void* self, void* /*edx*/)
{
    uint32_t bufferPtr = CSF_AT(uint32_t, self, 0x0C);
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    if (bufferPtr != 0)
    {
        Mem_Free(reinterpret_cast<void*>(bufferPtr));
        CSF_AT(uint32_t, self, 0x0C) = 0;
    }

    S_0041e990* entries = reinterpret_cast<S_0041e990*>(CSF_AT(uint32_t, self, 0x1C));
    (void)CSF_VTBL(0x0041E990);
    return S_0041e990_ReleaseAllEntries(entries, nullptr);
}

HOOK(0x0044DE50, ResetBufferAndReleaseEntries, "auto");
