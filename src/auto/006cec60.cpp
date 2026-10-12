#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_6CE710: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(unsigned int, __fastcall, sub_6CE710, 0x006CE710,
         (void* self, void* edx, char* a1, unsigned int a3, int arg8));

static std::uint32_t* __fastcall S_006cec60_InsertAndGetRecordPointer(
    S_006cec60* self, void* /*edx*/, std::uint32_t* result,
    char* position, int extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 100 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 100;
        }
    }

    sub_6CE710(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 100u;
    return result;
}

HOOK(0x006CEC60, S_006cec60_InsertAndGetRecordPointer, "auto");
