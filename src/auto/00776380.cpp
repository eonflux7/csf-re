#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_775D10: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(char, __fastcall, sub_775D10, 0x00775D10,
         (void* self, void* edx, int a1, unsigned int a3, int a2));

static std::uint32_t* __fastcall S_00776380_GetElementPointerForPosition(
    S_00776380* self, void* /*edx*/, std::uint32_t* result,
    int position, int extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 52 != 0) {
            const int offset = position - static_cast<int>(begin);
            index = offset / 52;
        }
    }

    sub_775D10(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 52u;
    return result;
}

HOOK(0x00776380, S_00776380_GetElementPointerForPosition, "auto");
