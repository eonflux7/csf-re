#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, WeaponDefs_IsFireModeSpecial, 0x007612C0, (int a1));

static int __fastcall C_008318c8_GetFireModeValue(C_008318c8* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0084);
    if (!result)
    {
        void* provider = reinterpret_cast<void*>(self->field_0078);
        if (provider)
        {
            int index = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, provider, 0)[0x98 / 4])(provider, nullptr, 100);
            if (index == -1)
            {
                void* value = reinterpret_cast<void*>(
                    Handle_GetValuePointer(self, nullptr));
                result = WeaponDefs_IsFireModeSpecial(CSF_AT(int, value, 0x74));
                if (static_cast<uint8_t>(result))
                    return Handle_GetValuePointer(self, nullptr);
            }
            else
            {
                result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, provider, 0)[0x8C / 4])(provider, nullptr, index);
                self->field_0084 = static_cast<uint32_t>(result);
            }
        }
    }
    return result;
}

HOOK(0x0047A2E0, C_008318c8_GetFireModeValue, "auto");
