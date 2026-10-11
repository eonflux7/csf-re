#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static char __fastcall C_008310d8_Destroy(C_008310d8* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0004);
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008310D8);
    if (result)
        reinterpret_cast<uint8_t*>(&self->field_0004)[0] = 0;

    void* object = reinterpret_cast<void*>(self->field_0054);
    if (object)
    {
        void* allocation = static_cast<uint8_t*>(object) - 4;
        if (CSF_AT(uint32_t, allocation, 0))
        {
            result = static_cast<char>(
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0])(object, nullptr, 3));
        }
        else
        {
            result = static_cast<char>(Mem_Free(allocation));
        }
        self->field_0054 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x004451E0, C_008310d8_Destroy, "auto");
