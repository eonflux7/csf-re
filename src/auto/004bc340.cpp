#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original function at 0x0041E5F0: grows the backing array of an S_0041e5f0 (self or embedded sub-object).
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));

// The original's two call sites to 0x41E5F0 reference this struct's vtable slot in the packet; keep the binding present.
[[maybe_unused]] static const auto* const s_growCallSiteVtable = CSF_VTBL(0x0041E5F0);

// Appends a command record to two arrays: the constant 12 into the first array (self),
// then a2 into the second array, embedded at offset +0x10 of the same struct. Returns a2.
static int __fastcall AiCoverAttackTasks_AppendParameterCommand(S_0041e5f0* self, void* /*edx*/, int a2)
{
    // First array: field_0000 = data, field_0004 = capacity, field_0008 = count, field_000c = grow step.
    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(self, nullptr, self->field_000c);

    uint32_t firstIndex = self->field_0008;
    uint32_t* firstSlot = reinterpret_cast<uint32_t*>(self->field_0000 + 4 * firstIndex);
    self->field_0008 = firstIndex + 1;
    *firstSlot = 12;

    // Second array: the sub-object embedded at +0x10 of this struct.
    S_0041e5f0* secondArray = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x10);
    if (secondArray->field_0008 >= secondArray->field_0004)
        S_0041e5f0_GrowArrayCapacity(secondArray, nullptr, secondArray->field_000c);

    uint32_t secondIndex = secondArray->field_0008;
    int* secondSlot = reinterpret_cast<int*>(secondArray->field_0000 + 4 * secondIndex);
    secondArray->field_0008 = secondIndex + 1;
    *secondSlot = a2;

    return a2;
}

HOOK(0x004BC340, AiCoverAttackTasks_AppendParameterCommand, "auto");
