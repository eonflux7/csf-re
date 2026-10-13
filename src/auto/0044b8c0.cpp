#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, InsertRecord12At, 0x0044B2D0,
         (void* self, void* edx, int a1, unsigned int a3, int arg8));

static uint32_t* __fastcall S_0044b8c0_InsertElementAndGetPointer(
    S_0044b8c0* self, void* /*edx*/, uint32_t* result,
    int position, int value)
{
    const uint32_t begin = self->field_0004;
    int32_t index = 0;

    if (begin != 0)
    {
        const int32_t sizeBytes =
            static_cast<int32_t>(self->field_0008 - begin);
        if (sizeBytes / 12 != 0)
        {
            index = (position - static_cast<int32_t>(begin)) / 12;
        }
    }

    InsertRecord12At(self, nullptr, position, 1u, value);

    *result = self->field_0004 + static_cast<uint32_t>(index) * 12u;
    return result;
}

HOOK(0x0044B8C0, S_0044b8c0_InsertElementAndGetPointer, "auto");
