#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, InsertStatusIconRecord, 0x004595A0,
         (void* self, void* edx, char* a1, unsigned int a3, char* arg8));

static uint32_t* __fastcall S_00459e50_InsertStatusIconAt(
    S_00459e50* self, void* /*edx*/, uint32_t* result,
    char* position, char* value)
{
    const uint32_t begin = self->field_0004;
    int32_t index = 0;

    if (begin != 0)
    {
        const int32_t sizeBytes =
            static_cast<int32_t>(self->field_0008 - begin);
        if (sizeBytes / 40 != 0)
        {
            const uint32_t positionAddress =
                static_cast<uint32_t>(
                    reinterpret_cast<uintptr_t>(position));
            index = static_cast<int32_t>(positionAddress - begin) / 40;
        }
    }

    InsertStatusIconRecord(self, nullptr, position, 1u, value);

    *result = self->field_0004 + static_cast<uint32_t>(index) * 40u;
    return result;
}

HOOK(0x00459E50, S_00459e50_InsertStatusIconAt, "auto");
