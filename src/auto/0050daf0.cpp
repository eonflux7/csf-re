#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

CSF_FUNC(char, __fastcall, UpdateChildrenAndElapsedTime,
         0x0071D520, (void* self, void* edx, float elapsedTime));
CSF_FUNC(void, __fastcall, CMenuMessageBox_UpdateIfEnabled,
         0x005288D0, (void* self, void* edx, float elapsedTime));
CSF_FUNC(int, __fastcall, MenuSystem_UpdatePendingMenu,
         0x004FB920, (void* self, void* edx, int elapsedTimeBits));

CSF_GLOBAL(const char*, g_IntroMenuName, 0x00899B2C);

static int __fastcall MenuSystem_UpdateFrame(void* self, void* /*edx*/)
{
    const float elapsedTime = CSF_AT(float, self, 0xA0);
    UpdateChildrenAndElapsedTime(
        CSF_AT(void*, self, 0x14), nullptr, elapsedTime);

    const float frameTime = CSF_AT(float, self, 0xA0);
    void** menuVtable = CSF_AT(void**, self, 0);
    reinterpret_cast<void (__fastcall*)(void*, void*, float)>(
        menuVtable[0x9C / 4])(self, nullptr, frameTime);

    void* messageBox = CSF_AT(void*, self, 0x80);
    if (messageBox)
    {
        CMenuMessageBox_UpdateIfEnabled(
            messageBox, nullptr, CSF_AT(float, self, 0xA0));
    }

    int result = CSF_AT(int, self, 0x78);
    if (result)
    {
        void* controller = CSF_AT(void*, self, 0x14);
        void** lookupVtable = CSF_AT(void**, controller, 0);
        void** savedVtable = CSF_AT(void**, controller, 0);

        const int introHandle =
            reinterpret_cast<int (__fastcall*)(void*, void*, const char*, int)>(
                lookupVtable[0x5C / 4])(
                    controller, nullptr, g_IntroMenuName, 1);

        controller = CSF_AT(void*, self, 0x14);
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            savedVtable[0x74 / 4])(controller, nullptr, introHandle);

        if (static_cast<std::uint8_t>(result) == 0)
        {
            const int elapsedTimeBits = CSF_AT(int, self, 0xA0);
            result = MenuSystem_UpdatePendingMenu(
                CSF_AT(void*, self, 0x78), nullptr, elapsedTimeBits);
        }
    }

    return result;
}

HOOK(0x0050DAF0, MenuSystem_UpdateFrame, "auto");
