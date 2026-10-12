#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_7729E0: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(char, __fastcall, sub_7729E0, 0x007729E0,
         (void* self, void* edx, void* a1, unsigned int a3, void* arg8));

static std::uint32_t* __fastcall S_00773060_InsertAndGetRecordPointer(
    S_00773060* self, void* /*edx*/, std::uint32_t* result,
    char* position, C_0082d410* extra)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 44 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 44;
        }
    }

    sub_7729E0(self, nullptr, position, 1u, extra);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 44u;
    return result;
}

HOOK(0x00773060, S_00773060_InsertAndGetRecordPointer, "auto");
