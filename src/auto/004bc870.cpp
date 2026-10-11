#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_0041e5f0_BeginAppendSequence, 0x004BC810, (void* self, void* edx, int a2, int a3));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// Appends two int values (a4, then a5) to the dynamic int array embedded at this+0x10.
// Array header layout: +0x10 data pointer, +0x14 capacity, +0x18 count, +0x1C grow step.
static int __fastcall S_0041e5f0_AppendPair(S_0041e5f0* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    S_0041e5f0_BeginAppendSequence(self, nullptr, a2, a3);

    S_0041e5f0* arrayHeader = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x10);

    // First append: grow when count >= capacity (unsigned compare).
    if (self->field_0018 >= self->field_0014)
        S_0041e5f0_GrowArrayCapacity(arrayHeader, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x1C)));

    uint32_t index = self->field_0018;
    int* slot = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x10) + 4 * index);
    self->field_0018 = index + 1;
    *slot = a4;

    // Second append: grow when count >= capacity, re-reading count after the first store.
    if (self->field_0018 >= self->field_0014)
        S_0041e5f0_GrowArrayCapacity(arrayHeader, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x1C)));

    uint32_t index2 = self->field_0018;
    int* slot2 = reinterpret_cast<int*>(CSF_AT(uint32_t, self, 0x10) + 4 * index2);
    self->field_0018 = index2 + 1;
    *slot2 = a5;

    return a5;
}

HOOK(0x4BC870, S_0041e5f0_AppendPair, "auto");
