#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, StateNode_Update, 0x00491950, (void* self, void* edx, int a2));

static int __fastcall C_00833a48_UpdateState(C_00833a48* self, void* /*edx*/, int a2)
{
    bool initialFlag = false;
    if (CSF_AT(uint8_t, a2, 0) == 0)
    {
        void* object = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        initialFlag =
            !reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, object, 0)[0x80 / 4])(object, nullptr, 0, a2);
    }

    int savedField = CSF_AT(int, a2, 0x1c);
    CSF_AT(uint8_t, a2, 0) = initialFlag ? 1 : 0;
    StateNode_Update(self, nullptr, a2);

    bool flagClear = CSF_AT(uint8_t, a2, 0) == 0;
    CSF_AT(int, a2, 0x1c) = savedField;

    if (flagClear)
    {
        void* object = reinterpret_cast<void*>(CSF_AT(uint32_t, a2, 4));
        unsigned char finishResult =
            reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0x7c / 4])(object, nullptr, a2);
        if (finishResult != 0)
        {
            CSF_AT(uint8_t, a2, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x00492030, C_00833a48_UpdateState, "auto");
