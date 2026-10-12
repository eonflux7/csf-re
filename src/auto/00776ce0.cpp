#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_776420: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(char, __fastcall, sub_776420, 0x00776420,
         (void* self, void* edx, void* a1, unsigned int a3, int arg8));

static std::uint32_t* __fastcall S_00776ce0_InsertElementAndGetPointer(
    S_00776ce0* self, void* /*edx*/, std::uint32_t* result,
    char* position, int extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 168 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 168;
        }
    }

    sub_776420(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 168u;
    return result;
}

HOOK(0x00776CE0, S_00776ce0_InsertElementAndGetPointer, "auto");
