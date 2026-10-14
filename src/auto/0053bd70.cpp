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

// Vtable addresses written into the object header (primary and secondary bases).
static const uint32_t kVtblCMenuProfiles = CSF_ADDR_VT(0x00842908);     // CMenuProfiles primary vtable
static const uint32_t kVtblCMenuScreenSecondary = CSF_ADDR_VT(0x008428B8);
static const uint32_t kVtblIntermediate830488 = CSF_ADDR_DATA(0x00830488);
static const uint32_t kVtblIntermediate830438 = CSF_ADDR_VT(0x00830438);
static const uint32_t kVtblFinal83042C = CSF_ADDR_VT(0x0083042C);

// game::Mem_Free
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Byte offset of the held child object pointer inside CMenuProfiles.
static constexpr size_t kChildObjectOffset = 0x24;

// Tears down the profiles menu: releases the held child (slot 0 virtual, flag 1),
// resets the vtable chain, and frees the object if bit 0 of flags is set.
static void* __fastcall CMenuProfiles_Destruct(void* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuProfiles;
    words[1] = kVtblCMenuScreenSecondary;

    void* child = CSF_AT(void*, self, kChildObjectOffset);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        CSF_AT(uint32_t, self, kChildObjectOffset) = 0;
    }

    words[1] = kVtblIntermediate830488;
    words[1] = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x0053BD70, CMenuProfiles_Destruct, "auto");

