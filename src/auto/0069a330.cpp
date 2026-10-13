#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_0085e760_TryAccumulateAndConfirm(
    C_0085e760* self, void* /*edx*/, int request, int /*unused*/)
{
    alignas(8) uint8_t deltaBytes[0x4];
    int& delta = *reinterpret_cast<int*>(deltaBytes);

    // The original reserves this local with push ecx, retaining this value
    // if the query reports success without writing its output.
    delta = static_cast<int>(reinterpret_cast<uintptr_t>(self));

    int result = self->vSlot20(
        static_cast<int>(reinterpret_cast<uintptr_t>(&delta)));
    if ((result & 0xFF) == 0)
        return result;

    self->field_0008 += static_cast<uint32_t>(delta);

    void* inner = reinterpret_cast<void*>(self->field_0004);
    int acknowledged =
        ((int(__fastcall*)(void*, void*, int, int))
            CSF_AT(void**, inner, 0)[0x8 / 4])(
                inner, nullptr, request, delta);

    return delta == acknowledged ? 1 : 0;
}

HOOK(0x0069A330, C_0085e760_TryAccumulateAndConfirm, "auto");
