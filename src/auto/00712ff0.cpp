#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall CInputDirectInput_CopyStateBlock(
    CInputDirectInput* self, void* /*edx*/, const void* sourceBlock)
{
    auto* destination = &self->field_0c30;
    auto* source = static_cast<const uint32_t*>(sourceBlock);

#pragma clang loop unroll(disable) vectorize(disable) interleave(disable)
    for (uint32_t remaining = 0x20; remaining != 0; remaining -= sizeof(uint32_t))
    {
        *destination = *source;
        ++destination;
        ++source;
    }

    self->field_0c3c = static_cast<float>(static_cast<int32_t>(self->field_0c44));
    self->field_0c40 = static_cast<float>(static_cast<int32_t>(self->field_0c48));
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x712FF0, CInputDirectInput_CopyStateBlock, "auto");
