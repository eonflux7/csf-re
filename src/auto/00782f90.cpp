#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_782C70: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(void, __fastcall, sub_782C70, 0x00782C70,
         (void* self, void* edx, void* a1, unsigned int a3, void* a2));

static std::uint32_t* __fastcall S_00782f90_GetSlotPointerForPosition(
    S_00782f90* self, void* /*edx*/, std::uint32_t* result,
    C_0082d410* position, C_0082d410* extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 348 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 348;
        }
    }

    sub_782C70(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 348u;
    return result;
}

HOOK(0x00782F90, S_00782f90_GetSlotPointerForPosition, "auto");
