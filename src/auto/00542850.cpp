#include <cstdint>
#include "globals.h"
#include "hook.h"

// Bindings to original functions and data (by address)
CSF_FUNC(int, __cdecl, MenuMpCharSelect_GetGlobalState, 0x0050D190, (void));
CSF_FUNC(int, __cdecl, MenuMpCharSelect_GetGlobal8B0688, 0x00550CC0, (void));
CSF_FUNC(int, __fastcall, MenuMpCharSelect_ValidateCandidate, 0x00551650, (void* self, void* edx));
CSF_GLOBAL(const char*, g_pszMenu_P_EstaMP, 0x00899B18);

// Returns nonzero when the camera preview should be blocked by the current menu state
static int __cdecl MenuMpCharSelect_ShouldShowPreview(void)
{
    void* globalState = reinterpret_cast<void*>(MenuMpCharSelect_GetGlobalState());
    void* menuObject = CSF_AT(void*, globalState, 0x14);

    // Slot 28 (+0x70): returns bool; if true, abort
    if (reinterpret_cast<unsigned char (__fastcall*)(void*, void*)>(CSF_AT(void**, menuObject, 0)[28])(menuObject, nullptr))
        return 0;

    // Slot 23 (+0x5C): takes the P_EstaMP string, returns an int handle
    int stringHandle = reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(CSF_AT(void**, menuObject, 0)[23])(
        menuObject, nullptr, g_pszMenu_P_EstaMP);

    // Slot 29 (+0x74): takes handle and 0, returns bool; if true, abort
    if (reinterpret_cast<unsigned char (__fastcall*)(void*, void*, int, int)>(CSF_AT(void**, menuObject, 0)[29])(
            menuObject, nullptr, stringHandle, 0))
        return 0;

    int candidate = MenuMpCharSelect_GetGlobal8B0688();
    return MenuMpCharSelect_ValidateCandidate(reinterpret_cast<void*>(candidate), nullptr) != 0;
}

HOOK(0x00542850, MenuMpCharSelect_ShouldShowPreview, "auto");
