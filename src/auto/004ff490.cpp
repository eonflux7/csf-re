#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resets a scene-loader record: releases the optional element array at +0x48
// (element destructor called with delete flag 3, or raw Mem_Free when the
// array has no element-count header), points the three vtable slots at
// 0x82D410, optionally frees the block at +0x04 (when capacity >= 0x10),
// and resets capacity to 15, length to 0 and the first byte to 0.
static int __fastcall Scene_ResetSectorTables(void* self, void* /*edx*/)
{
    // Element array at +0x48: element count header lives 4 bytes before the base.
    void* elementArray = CSF_AT(void*, self, 0x48);
    if (elementArray)
    {
        uint8_t* arrayHeader = reinterpret_cast<uint8_t*>(elementArray) - 4;
        if (CSF_AT(uint32_t, arrayHeader, 0))
        {
            // Element object: first dword is its vtable, slot 0 is the destructor.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, elementArray, 0)[0])(elementArray, nullptr, 3);
        }
        else
        {
            Mem_Free(arrayHeader);
        }
        CSF_AT(void*, self, 0x48) = nullptr;
    }

    void* emptyTable = CSF_VTBL(0x0082D410);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(emptyTable));
    CSF_AT(void*, self, 0x3C) = emptyTable;
    CSF_AT(void*, self, 0x30) = emptyTable;
    CSF_AT(void*, self, 0x24) = emptyTable;

    if (CSF_AT(uint32_t, self, 0x18) >= 0x10u)
    {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(CSF_AT(void*, self, 0x04)));
        if (result)
        {
            result = Mem_FreeDebug(CSF_AT(void*, self, 0x04));
        }
    }

    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x04) = 0;
    return result;
}

HOOK(0x4FF490, Scene_ResetSectorTables, "auto");
