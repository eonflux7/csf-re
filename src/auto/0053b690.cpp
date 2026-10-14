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
static const uint32_t kVtblCMenuGameOptions = CSF_ADDR_VT(0x00842788);  // CMenuGameOptions primary vtable
static const uint32_t kVtblCMenuScreenSub   = CSF_ADDR_VT(0x00842738);  // CMenuScreen sub-object vtable
static const uint32_t kVtblIntermediate830488 = CSF_ADDR_DATA(0x00830488);
static const uint32_t kVtblIntermediate830438 = CSF_ADDR_VT(0x00830438);
static const uint32_t kVtblFinal83042C        = CSF_ADDR_VT(0x0083042C);

// game::Mem_Free
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down the options menu: resets the vtable chain, releases the held child
// object (slot 0 virtual, called with flag 1, field cleared), and frees the
// object with Mem_Free when bit 0 of flags is set.
static void* __fastcall CMenuGameOptions_Destruct(CMenuGameOptions* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuGameOptions;
    self->field_0004 = kVtblCMenuScreenSub;

    void* child = reinterpret_cast<void*>(self->field_001c);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_001c = 0;
    }

    self->field_0004 = kVtblIntermediate830488;
    self->field_0004 = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x0053B690, CMenuGameOptions_Destruct, "auto");
