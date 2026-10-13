#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_771390: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(int, __fastcall, sub_771390, 0x00771390,
         (void* self, void* edx, int* a1, unsigned int a3, int arg8));

static std::uint32_t* __fastcall S_00771690_InsertAndGetRecordPointer(
    S_00771690* self, void* /*edx*/, std::uint32_t* result,
    int* position, int extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 104 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 104;
        }
    }

    sub_771390(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 104u;
    return result;
}

HOOK(0x00771690, S_00771690_InsertAndGetRecordPointer, "auto");
