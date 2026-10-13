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

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Constructor-style reset: installs the C_0086c1e0 vtable, allocates a 16-byte
// block (zeroing words 1..3) and stores it in field_0004, or nullptr on failure.
// Returns this.
static C_0086c1e0* __fastcall C_0086c1e0_ResetBaseTransitionState(C_0086c1e0* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x0086C1E0);

    void* block = Mem_Alloc(16);
    if (block != nullptr) {
        auto* words = static_cast<uint32_t*>(block);
        words[1] = 0;
        words[2] = 0;
        words[3] = 0;
        self->field_0004 = static_cast<uint32_t>(reinterpret_cast<std::uintptr_t>(block));
    }
    else {
        self->field_0004 = 0;
    }
    return self;
}

HOOK(0x75B5A0, C_0086c1e0_ResetBaseTransitionState, "auto");
