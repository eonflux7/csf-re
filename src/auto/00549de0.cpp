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
static const uint32_t kVtblCMenuVideoOptions = CSF_ADDR_VT(0x00844148); // CMenuVideoOptions primary vtable
static constexpr uint32_t kVtblSecondary8440F8 = 0x008440F8u;   // CMenuScreen-chain secondary vtable
static const uint32_t kVtblIntermediate830488 = CSF_ADDR_DATA(0x00830488);
static const uint32_t kVtblIntermediate830438 = CSF_ADDR_VT(0x00830438);
static const uint32_t kVtblFinal83042C = CSF_ADDR_VT(0x0083042C);

// Mem_Free (0x691110): game heap free, called only when bit 0 of flags is set.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the video options menu: resets the vtable chain, releases the child
// object at +0x1C (virtual slot 0 called with flag 1), and frees the object
// itself when bit 0 of flags is set. Returns self in eax.
static void* __fastcall CMenuVideoOptions_Destruct(CMenuVideoOptions* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuVideoOptions;
    self->field_0004 = kVtblSecondary8440F8;

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

HOOK(0x00549DE0, CMenuVideoOptions_Destruct, "auto");
