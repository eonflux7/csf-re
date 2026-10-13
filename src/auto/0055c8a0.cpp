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

// Embedded member at +0x34 is torn down by 0x518890 (__thiscall, no stack args).
CSF_FUNC(int, __fastcall, CMenuAccountEdit_ResetInnerObject, 0x518890, (void* self, void* edx));

// Game heap free (__cdecl, one stack argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* block));

// CMenuScreen base vtable global, bound by address.
static CMenuAccountEdit* __fastcall CMenuAccountEdit_DestroyAndMaybeFree(CMenuAccountEdit* self, void* /*edx*/, char freeFlag)
{
    // Install CMenuAccountEdit primary vtable and secondary vtable.
    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x845438);
    self->field_0004 = 0x8453E8;

    // Release the child object at +0x284 through its slot-0 (deleting) destructor, then clear it.
    void** child = reinterpret_cast<void**>(self->field_0284);
    if (child != nullptr)
    {
        void** childVtable = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtable[0])(child, nullptr, 1);
        self->field_0284 = 0;
    }

    // Destroy the embedded member at +0x34.
    CMenuAccountEdit_ResetInnerObject(reinterpret_cast<uint8_t*>(self) + 0x34, nullptr);

    // Restore base-class vtables.
    self->field_0004 = CSF_ADDR_DATA(0x830488);
    self->field_0004 = CSF_ADDR_VT(0x830438);
    CSF_AT(uint32_t, self, 0) = reinterpret_cast<uint32_t>(CSF_VTBL(0x83042C));

    // Free the object storage when the low bit of the flag is set.
    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x0055C8A0, CMenuAccountEdit_DestroyAndMaybeFree, "auto");
