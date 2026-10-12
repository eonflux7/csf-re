#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_440890: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(unsigned int, __fastcall, sub_440890, 0x00440890,
         (void* self, void* edx, char* a1, unsigned int a3, int arg8));

static std::uint32_t* __fastcall S_00440ee0_InsertAndGetRecordPointer(
    S_00440ee0* self, void* /*edx*/, std::uint32_t* result,
    char* position, int extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 196 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 196;
        }
    }

    sub_440890(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 196u;
    return result;
}

HOOK(0x00440EE0, S_00440ee0_InsertAndGetRecordPointer, "auto");
