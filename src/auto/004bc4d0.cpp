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

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

// Vtable of S_0041e5f0, referenced by the two grow call sites in the original.
[[maybe_unused]] static const auto kSignedArrayVtableRef = CSF_VTBL(0x0041E5F0);

static int __fastcall AiCoverAttackTasks_AppendSignedParameters(S_0041e5f0* self, void* /*edx*/, float a2, float a3, float a4)
{
    // The original pushes 100, calls GetGlobalState once (its eax result becomes GetStateValue's this),
    // then calls GetStateValue, which pops the pushed 100.
    void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalState_50D190()));
    int stateValuePlusOne = GetStateValue(globalState, nullptr, 100) + 1;

    // Signed-to-float conversion with the unsigned bias fix-up for negative results.
    float unsignedCount = static_cast<float>(stateValuePlusOne);
    if (stateValuePlusOne < 0)
        unsignedCount += g_UnsignedIntegerBias;

    float a3a = ((unsignedCount * (a4 - a3)) * g_ProgressScale + a3) * a2;

    // First array: opcode 23.
    if (self->field_0008 >= self->field_0004)
        S_0041e5f0_GrowArrayCapacity(self, nullptr, static_cast<int>(self->field_000c));

    uint32_t opcodeIndex = self->field_0008;
    uint32_t* opcodeSlot = reinterpret_cast<uint32_t*>(self->field_0000) + opcodeIndex;
    self->field_0008 = opcodeIndex + 1;
    *opcodeSlot = 23;

    // Second array (embedded at +0x10): the computed float.
    S_0041e5f0* secondArray = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x10);
    if (secondArray->field_0008 >= secondArray->field_0004)
        S_0041e5f0_GrowArrayCapacity(secondArray, nullptr, static_cast<int>(secondArray->field_000c));

    uint32_t valueIndex = secondArray->field_0008;
    float* valueSlot = reinterpret_cast<float*>(secondArray->field_0000) + valueIndex;
    secondArray->field_0008 = valueIndex + 1;
    *valueSlot = a3a;

    uint32_t resultBits = 0;
    std::memcpy(&resultBits, &a3a, sizeof(resultBits));
    return static_cast<int>(resultBits);
}

HOOK(0x004BC4D0, AiCoverAttackTasks_AppendSignedParameters, "auto");
