#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, C_00840f48_IsActive, 0x0051C0B0, (void* self, void* edx));
static CSF_FUNC(unsigned int, __fastcall, C_00840f48_Stop, 0x0051E640, (void* self, void* edx));
static CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00840f48_Destroy(C_00840f48* self, void* /*edx*/)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00840F48);

    if (C_00840f48_IsActive(self, nullptr))
        C_00840f48_Stop(self, nullptr);

    int result = static_cast<int>(self->field_001c);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_001c));
        self->field_001c = 0;
    }

    const uint32_t elements = self->field_000c;
    if (elements)
    {
        const uint32_t count =
            CSF_AT(uint32_t, reinterpret_cast<void*>(elements - 4), 0);
        uint32_t cursor = elements + 16 * count;
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            cursor += 4;
            uint32_t remaining = count;
            do
            {
                cursor -= 0x10;
                --remaining;
                CSF_AT(void**, reinterpret_cast<void*>(cursor), 0) =
                    CSF_VTBL(0x0082D410);
            } while (remaining);
        }
        result = Mem_Free(reinterpret_cast<void*>(elements - 4));
        self->field_000c = 0;
    }

    CSF_AT(void**, self, 0) = CSF_VTBL(0x00840E68);
    return result;
}

HOOK(0x0051E890, C_00840f48_Destroy, "auto");
