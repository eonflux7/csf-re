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

CSF_FUNC(int, __fastcall, sub_51BF70, 0x0051BF70, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00840e38_ResetBuffers(C_00840e38* self, void* /*edx*/)
{
    // Release the previously owned entry block, if any.
    uint32_t oldEntry = self->field_0004;
    if (oldEntry)
    {
        sub_51BF70(reinterpret_cast<void*>(static_cast<uintptr_t>(oldEntry)), nullptr);
        Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(oldEntry)));
        self->field_0004 = 0;
    }

    // Allocate a fresh 16-byte entry block and zero its three dwords.
    void* entry = Mem_Alloc(16);
    if (entry)
    {
        CSF_AT(uint32_t, entry, 0x4) = 0;
        CSF_AT(uint32_t, entry, 0x8) = 0;
        CSF_AT(uint32_t, entry, 0xC) = 0;
    }
    else
    {
        entry = nullptr;
    }
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entry));

    // Free the secondary buffer, keeping its debug-free return value.
    int result = static_cast<int>(self->field_000c);
    if (result)
    {
        result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_000c)));
    }
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    return result;
}

HOOK(0x0051C000, C_00840e38_ResetBuffers, "auto");
