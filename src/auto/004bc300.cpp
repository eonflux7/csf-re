#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Grows the backing array of S_0041e5f0 (thiscall, one stack arg, ret 4).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0,
         (S_0041e5f0* self, void* edx, int additionalCapacity));

// Appends a vector-like source to the same container (thiscall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_AppendVector, 0x004B7150,
         (void* self, void* edx, uint32_t* a2));

static int __fastcall AiCoverAttackTasks_AppendDestinationCommands(S_0041e5f0* self, void* /*edx*/, uint32_t* a2)
{
    // Referenced vtable of S_0041e5f0 (no store is performed by the original at this site).
    (void)CSF_VTBL(0x0041E5F0);

    // Grow when the used count has reached the element count (unsigned compare, jb in the original).
    if (self->field_0008 >= self->field_0004) {
        S_0041e5f0_GrowArrayCapacity(self, nullptr, static_cast<int>(self->field_000c));
    }

    // Re-read the base pointer and used count after a possible grow.
    uint32_t index = self->field_0008;
    uint32_t* slot = reinterpret_cast<uint32_t*>(self->field_0000 + 4u * index);
    self->field_0008 = index + 1;
    *slot = 2;

    return AiCoverAttackTasks_AppendVector(self, nullptr, a2);
}

HOOK(0x004BC300, AiCoverAttackTasks_AppendDestinationCommands, "auto");
