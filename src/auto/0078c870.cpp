#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_78C4C0: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(unsigned int, __fastcall, sub_78C4C0, 0x0078C4C0,
         (void* self, void* edx, char* a1, unsigned int a3, int* arg8));

static std::uint32_t* __fastcall S_0078c870_InsertAndGetRecordPointer(
    S_0078c870* self, void* /*edx*/, std::uint32_t* result,
    char* position, int* extraArg)
{
    std::uint32_t begin = self->field_0004;
    std::int32_t index = 0;

    // The original branches on the zero flag of (field_0008 - begin), not on the quotient.
    if (begin != 0 && (self->field_0008 - begin) != 0) {
        const std::int32_t offset =
            static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(position) - begin);
        index = offset / 164;
    }

    sub_78C4C0(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 164u;
    return result;
}

HOOK(0x0078C870, S_0078c870_InsertAndGetRecordPointer, "auto");
