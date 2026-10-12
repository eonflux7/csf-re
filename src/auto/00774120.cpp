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

// Constructor: sets the vtable, allocates a 16-byte block (zeroing words 1..3)
// and stores it in field_0004. Returns this.
static C_00871a18* __fastcall C_00871a18_ConstructMenuEventSource(C_00871a18* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00871A18);

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

HOOK(0x774120, C_00871a18_ConstructMenuEventSource, "auto");
