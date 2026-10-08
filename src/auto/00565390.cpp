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

// Vtable addresses written into the object header (primary and intermediate bases).
static constexpr uint32_t kVtblCMenuAccounts = 0x00845AF8u;   // CMenuAccounts primary vtable
static constexpr uint32_t kVtblCMenuScreenSec = 0x00845AA8u;  // CMenuScreen secondary vtable
static constexpr uint32_t kVtblIntermediate830488 = 0x00830488u;
static constexpr uint32_t kVtblIntermediate830438 = 0x00830438u;
static constexpr uint32_t kVtblFinal83042C = 0x0083042Cu;

// game::Mem_Free (menu allocations are released with the game heap)
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the held child object (slot 0 virtual, called with flag 1), clears the
// child pointer, resets the vtable chain, and frees the object with Mem_Free when
// bit 0 of flags is set. Returns self.
static void* __fastcall CMenuAccounts_Destruct(CMenuAccounts* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuAccounts;
    self->field_0004 = kVtblCMenuScreenSec;

    void* child = reinterpret_cast<void*>(self->field_003c);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_003c = 0;
    }

    self->field_0004 = kVtblIntermediate830488;
    self->field_0004 = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x00565390, CMenuAccounts_Destruct, "auto");

