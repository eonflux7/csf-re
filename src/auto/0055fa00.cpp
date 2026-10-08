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

// Vtable addresses written into the object header during teardown.
static const uint32_t kVtblCMenuMpOnline = CSF_ADDR_VT(0x00845740);   // primary vtable of CMenuMpOnline
static constexpr uint32_t kVtblCMenuScreenChain = 0x008456F0u; // CMenuScreen-side intermediate vtable
static const uint32_t kVtblIntermediate830488 = CSF_ADDR_DATA(0x00830488);
static const uint32_t kVtblIntermediate830438 = CSF_ADDR_VT(0x00830438);
static const uint32_t kVtblFinal83042C = CSF_ADDR_VT(0x0083042C);

// game::Mem_Free (0x691110), __cdecl, one argument.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down a CMenuMpOnline: resets the vtable chain, releases the child object
// held at +0x14 (its slot-0 virtual is called with flag 1), and if bit 0 of flags
// is set, frees the object with Mem_Free. Returns this.
static void* __fastcall CMenuMpOnline_Destruct(CMenuMpOnline* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuMpOnline;
    self->field_0004 = kVtblCMenuScreenChain;

    void* child = reinterpret_cast<void*>(self->field_0014);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_0014 = 0;
    }

    self->field_0004 = kVtblIntermediate830488;
    self->field_0004 = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x0055FA00, CMenuMpOnline_Destruct, "auto");
