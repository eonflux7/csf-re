#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WeaponFirearms_UpdateState, 0x00479B10,
         (void* self, void* edx, int a2));

static int __fastcall C_008318c8_UpdateState(C_008318c8* self, void* /*edx*/, int a2)
{
    bool initialFlag = false;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* object = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char result =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, object, 0)[0x80 / 4])(object, nullptr, 0, a2);
        initialFlag = result == 0;
    }

    int savedState = CSF_AT(int, a2, 0x1C);
    CSF_AT(uint8_t, a2, 0) = initialFlag ? 1 : 0;
    WeaponFirearms_UpdateState(self, nullptr, a2);

    bool flagClear = CSF_AT(uint8_t, a2, 0) == 0;
    CSF_AT(int, a2, 0x1C) = savedState;

    if (flagClear)
    {
        void* object = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char result =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x7C / 4])(object, nullptr, a2);
        if (result != 0)
        {
            CSF_AT(uint8_t, a2, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x0047A240, C_008318c8_UpdateState, "auto");
