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
static constexpr uint32_t kVtblCMenuSoundOptions = 0x00842AD0u;
static constexpr uint32_t kVtblCMenuScreenSecondary = 0x00842A80u;
static constexpr uint32_t kVtblIntermediate830488 = 0x00830488u;
static constexpr uint32_t kVtblIntermediate830438 = 0x00830438u;
static constexpr uint32_t kVtblFinal83042C = 0x0083042Cu;

// Mem_Free (__cdecl, one argument, caller pops)
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Tears down a CMenuSoundOptions object: resets the vtable chain, releases the
// held child object (slot 0 virtual, called with flag 1), and if bit 0 of flags
// is set, frees the object itself.
static void* __fastcall CMenuSoundOptions_Destruct(CMenuSoundOptions* self, void* /*edx*/, char flags)
{
    uint32_t* words = reinterpret_cast<uint32_t*>(self);

    words[0] = kVtblCMenuSoundOptions;
    words[1] = kVtblCMenuScreenSecondary;

    void* soundChild = reinterpret_cast<void*>(self->field_001c);
    if (soundChild)
    {
        void** childVtbl = CSF_AT(void**, soundChild, 0);
        ((void(__fastcall*)(void*, void*, int))childVtbl[0])(soundChild, nullptr, 1);
        self->field_001c = 0;
    }

    words[1] = kVtblIntermediate830488;
    words[1] = kVtblIntermediate830438;
    words[0] = kVtblFinal83042C;

    if (flags & 1)
        Mem_Free(self);

    return self;
}

HOOK(0x0053CF60, CMenuSoundOptions_Destruct, "auto");
