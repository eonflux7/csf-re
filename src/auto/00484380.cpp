#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Hud_IsSuspended, 0x00495520, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_495550_CheckInnerObject, 0x00495550, (void* self, void*));
CSF_FUNC(int32_t, __fastcall, C_00495560_InnerTypeIs38, 0x00495560, (void* self, void*));

static int __fastcall CharActionFsm_GetMenuObjectState(void* self, void* /*edx*/)
{
    int menuState = 0;
    if (CSF_AT(void*, self, 0x9C))
    {
        void* menuObject = CSF_AT(void*, self, 0x9C);
        menuState =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, menuObject, 0)[0x4C / 4])(
                    menuObject, nullptr);
    }

    if (Hud_IsSuspended(CSF_AT(void*, self, 0x08), nullptr) &&
        CSF_AT(int, self, 0x0C) != 13)
    {
        return 0;
    }

    if (static_cast<uint8_t>(
            C_495550_CheckInnerObject(CSF_AT(void*, self, 0x08), nullptr)))
    {
        return 3;
    }

    const int currentState = CSF_AT(int, self, 0x0C);
    if ((currentState != 12 || CSF_AT(int, self, 0x10) == 12) &&
        menuState == 13)
    {
        return 1;
    }

    void* innerObject = CSF_AT(void*, self, 0x08);
    if ((CSF_AT(uint8_t, innerObject, 0x1F0) & 2) != 0 &&
        (menuState != 12 || currentState != 12 ||
         CSF_AT(int, self, 0x10) == 12))
    {
        return 2;
    }

    C_00495560_InnerTypeIs38(innerObject, nullptr);
    return -1;
}

HOOK(0x00484380, CharActionFsm_GetMenuObjectState, "auto");
