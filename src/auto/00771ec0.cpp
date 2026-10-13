#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, InsertRecord12At, 0x00771C00,
         (void* self, void* edx, char* a1, unsigned int a3, uint32_t* arg8));

static uint32_t* __fastcall S_00771b80_InsertRecord12(
    S_00771b80* self, void* /*edx*/, uint32_t* result,
    char* position, uint32_t* value)
{
    const uint32_t begin = self->field_0004;
    int32_t index = 0;

    if (begin != 0)
    {
        const int32_t sizeBytes =
            static_cast<int32_t>(self->field_0008 - begin);
        if (sizeBytes / 12 != 0)
        {
            const uint32_t positionAddress =
                static_cast<uint32_t>(
                    reinterpret_cast<uintptr_t>(position));
            index = static_cast<int32_t>(positionAddress - begin) / 12;
        }
    }

    InsertRecord12At(self, nullptr, position, 1u, value);

    *result = self->field_0004 + static_cast<uint32_t>(index) * 12u;
    return result;
}

HOOK(0x00771EC0, S_00771b80_InsertRecord12, "auto");
