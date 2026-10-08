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

// Embedded member at +0x20 is destroyed by 0x508430 (__thiscall, no stack args).
CSF_FUNC(unsigned int, __fastcall, CMenuCredits_DestroyEmbeddedMember, 0x508430, (void* self, void* edx));

// Game heap free (__cdecl, one stack argument).
CSF_FUNC(int, __cdecl, Mem_Free, 0x691110, (void* block));

// Vtables and the CMenuScreen base vtable global, bound by address.
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x83042C);

static CMenuCredits* __fastcall CMenuCredits_DestroyAndMaybeFree(CMenuCredits* self, void* /*edx*/, char freeFlag)
{
    // Install CMenuCredits primary vtable and secondary vtable.
    CSF_AT(uint32_t, self, 0) = CSF_ADDR_VT(0x8418B8);
    self->field_0004 = 0x841868;

    // Release the child object at +0x18 through its slot-0 (deleting) destructor, then clear it.
    void** child = reinterpret_cast<void**>(self->field_0018);
    if (child != nullptr)
    {
        void** childVtable = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtable[0])(child, nullptr, 1);
        self->field_0018 = 0;
    }

    // Destroy the embedded member at +0x20.
    CMenuCredits_DestroyEmbeddedMember(reinterpret_cast<uint8_t*>(self) + 0x20, nullptr);

    // Restore base-class vtables.
    self->field_0004 = CSF_ADDR_DATA(0x830488);
    self->field_0004 = CSF_ADDR_VT(0x830438);
    CSF_AT(uint32_t, self, 0) = reinterpret_cast<uint32_t>(&g_VtblCMenuScreenBase);

    // Free the object storage when the low bit of the flag is set.
    if ((freeFlag & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x0052FA20, CMenuCredits_DestroyAndMaybeFree, "auto");
