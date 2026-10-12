#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, ApplySceneBinding, 0x00449440,
         (void* self, void* edx, uint32_t* a2));

static uint32_t* __fastcall S_00448a80_UpdateSceneBinding(
    S_00448a80* self, void* /*edx*/,
    S_00448a80* first, S_00448a80* second)
{
    alignas(8) uint8_t bindingBytes[0x18];
    auto* binding = reinterpret_cast<uint32_t*>(bindingBytes);

    const uint32_t firstValue = first->field_0004;
    binding[2] = first->field_0008;
    binding[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    binding[3] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    const uint32_t secondValue = second->field_0004;
    const uint32_t secondExtra = second->field_0008;
    binding[4] = secondValue;
    binding[1] = firstValue;
    binding[5] = secondExtra;

    return ApplySceneBinding(self, nullptr, binding);
}

HOOK(0x00449640, S_00448a80_UpdateSceneBinding, "auto");
