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

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Releases the object held at +0x20 (through its virtual dtor slot 0 when
// its header flag is set, otherwise straight to the core heap), clears the
// slot, re-stamps the vtable, and frees `this` when bit 0 of a2 is set.
static C_0082d410* __fastcall C_0082d410_ResetAndMaybeDelete(C_0082d410* self, void* /*edx*/, char a2)
{
    void* heldObject = CSF_AT(void*, self, 0x20);
    if (heldObject)
    {
        // Header dword sits just before the object.
        void* heldBlock = reinterpret_cast<uint8_t*>(heldObject) - 4;
        if (CSF_AT(uint32_t, heldBlock, 0))
        {
            // Virtual scalar-deleting destructor, slot 0, flag 3.
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, heldObject, 0)[0])(heldObject, nullptr, 3);
        }
        else
        {
            CoreMemory_Free(heldBlock);
        }
        CSF_AT(void*, self, 0x20) = nullptr;
    }

    C_0082d410_SetVtable(self, nullptr);

    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }
    return self;
}

HOOK(0x4AE920, C_0082d410_ResetAndMaybeDelete, "auto");
