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

// Game heap allocator (0x6910F0, __cdecl, pops nothing).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

// Copy-init helper for the base state of an attack task (0x47F310, __thiscall, ret 4).
CSF_FUNC(int*, __fastcall, C_00832f18_InitFromSource, 0x0047F310, (void* self, void* edx, int a2));

// Copies an embedded record of type S_0047ea40 at +0x30 (0x47EC10, __thiscall, ret 4).
CSF_FUNC(int*, __fastcall, S_0047ea40_CopyEmbeddedRecord, 0x0047EC10, (void* self, void* edx, int a5));

// Allocates a 64-byte C_00832f18 task object, zero-initializes it, copies the
// source task's state into it, and returns the new object (as an int, per the original).
static int __fastcall C_00832f18_CloneTask(C_00832f18* self, void* /*edx*/)
{
    C_00832f18* copy = nullptr;

    void* memory = Mem_Alloc(64);
    if (memory) {
        copy = reinterpret_cast<C_00832f18*>(memory);

        CSF_AT(uint32_t, copy, 0x04) = 1;
        copy->field_000c = 0;
        copy->field_0010 = 0;
        copy->field_0014 = 0;
        copy->field_001c = 0;
        copy->field_0020 = 0;
        copy->field_0024 = 0;
        copy->field_0028 = 0;
        copy->vtbl = CSF_VTBL(0x00832F18);
        copy->field_0034 = 0;
        copy->field_0038 = 0;
        copy->field_003c = 0;
        copy->field_002c = 0;
    }

    C_00832f18_InitFromSource(copy, nullptr, reinterpret_cast<int>(self));

    // Re-read the source flag after the init call, as the original does.
    uint8_t sourceFlag = self->field_002c;
    copy->field_002c = sourceFlag;

    S_0047ea40_CopyEmbeddedRecord(
        reinterpret_cast<uint8_t*>(copy) + 0x30,
        nullptr,
        reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x30));

    return reinterpret_cast<int>(copy);
}

HOOK(0x0047F2A0, C_00832f18_CloneTask, "auto");
