#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ParticleManager_IsActive, 0x0051C0B0,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, ParticleManager_ReleaseActiveState, 0x0051EDB0,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall C_00840f6c_Destroy(C_00840f6c* self, void* /*edx*/)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00840F6C);

    if (ParticleManager_IsActive(self, nullptr))
        ParticleManager_ReleaseActiveState(self, nullptr);

    int result = static_cast<int>(self->field_003c);
    if (result)
    {
        result = Mem_Free(reinterpret_cast<void*>(self->field_003c));
        self->field_003c = 0;
    }

    CSF_AT(void**, self, 0x24) = CSF_VTBL(0x0082D410);

    const uint32_t elements = self->field_0010;
    if (elements)
    {
        const uint32_t count =
            CSF_AT(uint32_t, reinterpret_cast<void*>(elements - 4), 0);
        uint32_t cursor = elements + 28 * count;
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            cursor += 4;
            uint32_t remaining = count;
            do
            {
                cursor -= 28;
                --remaining;
                CSF_AT(void**, reinterpret_cast<void*>(cursor), 0) =
                    CSF_VTBL(0x0082D410);
            } while (remaining);
        }

        result = Mem_Free(reinterpret_cast<void*>(elements - 4));
        self->field_0010 = 0;
    }

    CSF_AT(void**, self, 0) = CSF_VTBL(0x00840E68);
    return result;
}

HOOK(0x0051F160, C_00840f6c_Destroy, "auto");
