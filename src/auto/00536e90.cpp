#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData, 0x006F2850, (void* self, void* edx, void* data));
CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);

// Resets the intro-menu scene state at this+0x100..0x112, binds the intro page
// through the scene's manager (vtable slots 0x5C and 0x58), and sets the
// "instanced" flag at +0x130. Returns the value of the final manager call.
static int __fastcall Interfaz_InstancingIntroScene(void* self, void* /*edx*/, int /*a2*/)
{
    // Reset the page-descriptor block at +0x100.
    CSF_AT(uint32_t, self, 0x100) = 0;
    CSF_AT(uint32_t, self, 0x104) = 0;
    CSF_AT(uint8_t, self, 0x108) = 0;
    CSF_AT(uint8_t, self, 0x109) = 0;
    CSF_AT(uint32_t, self, 0x10C) = 0;
    CSF_AT(uint8_t, self, 0x110) = 1;
    CSF_AT(uint8_t, self, 0x111) = 0;
    CSF_AT(uint8_t, self, 0x112) = 0;

    // Pick the text buffer: inline buffer at +0x118 when capacity < 16, else the pointer stored there.
    uint32_t textPtr;
    if (CSF_AT(uint32_t, self, 0x12C) < 0x10u)
        textPtr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + 0x118));
    else
        textPtr = CSF_AT(uint32_t, self, 0x118);

    CSF_AT(uint8_t, self, 0x110) = 0;

    void* managerObj = CSF_AT(void*, self, 0x18);
    CSF_AT(uint32_t, self, 0x100) = textPtr;
    CSF_AT(uint32_t, self, 0x104) = 1;
    CSF_AT(uint8_t, self, 0x111) = 1;

    // manager->vtbl[0x5C / 4](g_pszMenu_P_Intro, &page descriptor)
    void* pageObj = reinterpret_cast<void*>(
        ((void* (__fastcall*)(void*, void*, const char*, void*))CSF_AT(void**, managerObj, 0)[23])(
            managerObj, nullptr, g_pszMenu_P_Intro,
            static_cast<uint8_t*>(self) + 0x100));

    MenuManager_SetIntroPageData(pageObj, nullptr, nullptr);

    // Reload the manager and call manager->vtbl[0x58 / 4](g_pszMenu_P_Intro).
    void* managerObjAgain = CSF_AT(void*, self, 0x18);
    int result = ((int (__fastcall*)(void*, void*, const char*))CSF_AT(void**, managerObjAgain, 0)[22])(
        managerObjAgain, nullptr, g_pszMenu_P_Intro);

    CSF_AT(uint32_t, self, 0x130) = 1;
    return result;
}

HOOK(0x536E90, Interfaz_InstancingIntroScene, "auto");
