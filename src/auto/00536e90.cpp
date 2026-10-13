#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData, 0x006F2850, (void* self, void* edx, void* data));
CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);

static int __fastcall Interface_InstanceIntroScene(void* self, void* /*edx*/, int /*unused*/)
{
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint8_t, self, 0x108) = 0;
    CSF_AT(uint8_t, self, 0x109) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 0;
    CSF_AT(uint8_t, self, 0x110) = 1;
    CSF_AT(uint8_t, self, 0x111) = 0;
    CSF_AT(uint8_t, self, 0x112) = 0;

    void* nameBuffer;
    if (CSF_AT(uint32_t, self, 0x12C) < 0x10u)
        nameBuffer = reinterpret_cast<uint8_t*>(self) + 0x118;
    else
        nameBuffer = CSF_AT(void*, self, 0x118);

    CSF_AT(uint8_t, self, 0x110) = 0;
    void* manager = CSF_AT(void*, self, 0x18);
    CSF_AT(void*, self, 0x100) = nameBuffer;
    CSF_AT(uint32_t, self, 0x104) = 1;
    CSF_AT(uint8_t, self, 0x111) = 1;

    void* introPage =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*, void*)>(
            CSF_AT(void**, manager, 0)[0x5C / 4])(
                manager, nullptr, g_pszMenu_P_Intro,
                reinterpret_cast<uint8_t*>(self) + 0x100);

    MenuManager_SetIntroPageData(introPage, nullptr, nullptr);

    manager = CSF_AT(void*, self, 0x18);
    int result =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, manager, 0)[0x58 / 4])(manager, nullptr);

    CSF_AT(uint32_t, self, 0x130) = 1;
    return result;
}

HOOK(0x00536E90, Interface_InstanceIntroScene, "auto");
