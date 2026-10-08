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
static constexpr uint32_t kVtblCMenuMpNetworkType = 0x00845BF0u;  // primary vtable
static constexpr uint32_t kVtblCMenuScreenSecondary = 0x00845BA0u; // secondary base vtable
static constexpr uint32_t kVtblIntermediate830488 = 0x00830488u;
static constexpr uint32_t kVtblIntermediate830438 = 0x00830438u;
static constexpr uint32_t kVtblFinal83042C = 0x0083042Cu;

// game::Mem_Free
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the held child object (slot 0 virtual, called with flag 1) and resets
// the vtable chain. If bit 0 of flags is set, frees the object with Mem_Free.
static void* __fastcall CMenuMpNetworkType_DestructAndMaybeFree(CMenuMpNetworkType* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuMpNetworkType;
    self->field_0004 = kVtblCMenuScreenSecondary;

    void* child = reinterpret_cast<void*>(self->field_0038);
    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_0038 = 0;
    }

    self->field_0004 = kVtblIntermediate830488;
    self->field_0004 = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x005658E0, CMenuMpNetworkType_DestructAndMaybeFree, "auto");
