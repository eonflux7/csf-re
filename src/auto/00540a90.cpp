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
static constexpr uint32_t kVtblCMenuMpScoreBar = 0x008430F8u;
static constexpr uint32_t kVtblCMenuScreenSecondary = 0x008430A8u;
static constexpr uint32_t kVtblIntermediate830488 = 0x00830488u;
static constexpr uint32_t kVtblIntermediate830438 = 0x00830438u;
static constexpr uint32_t kVtblFinal83042C = 0x0083042Cu;

// game::Mem_Free
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor of CMenuMpScoreBar. Releases the held child object (slot 0 virtual,
// called with flag 1), resets the vtable chain, and if bit 0 of flags is set,
// frees the object with Mem_Free. Returns self.
static void* __fastcall CMenuMpScoreBar_Destruct(CMenuMpScoreBar* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    void* child = reinterpret_cast<void*>(self->field_0030);

    words[0] = kVtblCMenuMpScoreBar;
    self->field_0004 = kVtblCMenuScreenSecondary;

    if (child)
    {
        void** childVtbl = CSF_AT(void**, child, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(child, nullptr, 1);
        self->field_0030 = 0;
    }

    self->field_0004 = kVtblIntermediate830488;
    self->field_0004 = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x00540A90, CMenuMpScoreBar_Destruct, "auto");
