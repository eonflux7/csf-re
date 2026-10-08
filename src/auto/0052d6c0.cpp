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
static const uint32_t kVtblCMenuProgressBar = CSF_ADDR_VT(0x00841598); // CMenuProgressBar primary vtable
static constexpr uint32_t kVtblCMenuScreenSecondary = 0x00841548u;
static const uint32_t kVtblIntermediate830488 = CSF_ADDR_DATA(0x00830488);
static const uint32_t kVtblIntermediate830438 = CSF_ADDR_VT(0x00830438);
static const uint32_t kVtblFinal83042C = CSF_ADDR_VT(0x0083042C);

// game::Mem_Free
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the held progress-bar child (slot 0 virtual, called with flag 1), clears
// the child pointer, resets the vtable chain, and frees the object when bit 0 of
// flags is set. Returns the object pointer.
static void* __fastcall CMenuProgressBar_Destruct(CMenuProgressBar* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuProgressBar;
    self->field_0004 = kVtblCMenuScreenSecondary;

    void* child = reinterpret_cast<void*>(self->field_0020);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_0020 = 0;
    }

    self->field_0004 = kVtblIntermediate830488;
    self->field_0004 = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x0052D6C0, CMenuProgressBar_Destruct, "auto");
