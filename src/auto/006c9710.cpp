#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, InsertRecord36At, 0x006C9290,
         (void* self, void* edx, int a1, unsigned int a3, int arg8));

static uint32_t* __fastcall S_006c9710_InsertElement(
    S_006c9710* self, void* /*edx*/, uint32_t* result,
    int position, int value)
{
    const uint32_t begin = self->field_0004;
    int32_t index = 0;

    if (begin != 0)
    {
        const int32_t sizeBytes =
            static_cast<int32_t>(self->field_0008 - begin);
        if (sizeBytes / 36 != 0)
        {
            index = (static_cast<int32_t>(static_cast<uint32_t>(position) - begin)) / 36;
        }
    }

    InsertRecord36At(self, nullptr, position, 1u, value);

    *result = self->field_0004 + static_cast<uint32_t>(index) * 36u;
    return result;
}

HOOK(0x006C9710, S_006c9710_InsertElement, "auto");
