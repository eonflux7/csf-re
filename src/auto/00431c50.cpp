#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, BuildSceneMenuRecord, 0x00692580, (int a1, int a2));

static int __fastcall Hud_InvokeMenuTail(void* self, void* edx);

static __declspec(naked) int __fastcall Hud_InvokeMenuTail(
    void* self, void* edx) {
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 8]
    }
}

static int __fastcall Hud_RefreshSceneMenu(void* self, void* /*edx*/) {
    if (CSF_AT(uint32_t, self, 0x250) == 0)
        return 0;

    uint32_t context = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (context == 0)
        return 0;

    uint32_t contextData =
        CSF_AT(uint32_t, reinterpret_cast<void*>(context), 0x2C);

    uint32_t recordSource;
    if (CSF_AT(uint32_t, reinterpret_cast<void*>(contextData), 0x1C) < 0x10)
        recordSource = contextData + 8;
    else
        recordSource = CSF_AT(uint32_t, reinterpret_cast<void*>(contextData), 8);

    int record = BuildSceneMenuRecord(static_cast<int>(recordSource), 8);

    void* menu = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x250));
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, menu, 0)[16])(menu, nullptr, record);

    [[clang::musttail]] return Hud_InvokeMenuTail(
        reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x250)), nullptr);
}

HOOK(0x00431C50, Hud_RefreshSceneMenu, "auto");
