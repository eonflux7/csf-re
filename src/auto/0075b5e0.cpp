#include <cstdint>
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

// sub_75B290: __thiscall with 3 stack args (ret 12). Bound as __fastcall form (self, edx, stack args...).
CSF_FUNC(int*, __fastcall, sub_75B290, 0x0075B290,
         (void* self, void* edx, char* a1, unsigned int a3, int* arg8));

static std::uint32_t* __fastcall S_0075b5e0_InsertAndGetSoundRecordPointer(
    S_0075b5e0* self, void* /*edx*/, std::uint32_t* result,
    char* position, int* extraArg)
{
    std::uint32_t begin = self->field_0004;
    int index = 0;

    if (begin != 0) {
        const int byteCount = static_cast<int>(self->field_0008 - begin);

        if (byteCount / 68 != 0) {
            const int offset = static_cast<int>(reinterpret_cast<std::uintptr_t>(position) - begin);
            index = offset / 68;
        }
    }

    sub_75B290(self, nullptr, position, 1u, extraArg);

    *result = self->field_0004
            + static_cast<std::uint32_t>(index) * 68u;
    return result;
}

HOOK(0x0075B5E0, S_0075b5e0_InsertAndGetSoundRecordPointer, "auto");
