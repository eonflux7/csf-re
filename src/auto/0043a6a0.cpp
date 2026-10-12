#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees, bound by address.
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, Hud_ApplyRadarGeneralProperty, 0x0043A480, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, Hud_ApplyRadarDirectionsProperty, 0x00439D60, (void* self, void* edx, void* a2));

// Data: property keyword pointers.
CSF_GLOBAL(const char*, g_pszKey_GENERAL, 0x00899678);
CSF_GLOBAL(const char*, g_pszKey_DIRECCIONES, 0x00899674);

// S_00692b80's vtable address, referenced by the original.
static void* const kLinearRadarVtbl = CSF_VTBL(0x00692B80);

static void __fastcall Hud_InitializeLinearRadar(S_00692b80* self, void* /*edx*/, S_00692b80* a2)
{
    CSF_AT(std::uint8_t, self, 0x4D) = 0;

    if (a2 == nullptr)
        return;

    if (MenuExtras_GetChildCount(a2, nullptr) != 2)
        return;

    // FindProperty's keyword parameter is bound as char* to match the packet signature;
    // the original passes the const char* keyword pointer unchanged.
    std::uint8_t* generalProperty = FindProperty(reinterpret_cast<std::uint8_t*>(a2), nullptr,
                                                 const_cast<char*>(g_pszKey_GENERAL));
    if (!Hud_ApplyRadarGeneralProperty(self, nullptr, generalProperty))
        return;

    std::uint8_t* directionsProperty = FindProperty(reinterpret_cast<std::uint8_t*>(a2), nullptr,
                                                    const_cast<char*>(g_pszKey_DIRECCIONES));
    if (!Hud_ApplyRadarDirectionsProperty(self, nullptr, directionsProperty))
        return;

    CSF_AT(std::uint8_t, self, 0x4C) = 1;
    CSF_AT(std::uint8_t, self, 0x4D) = 1;
}

HOOK(0x0043A6A0, Hud_InitializeLinearRadar, "auto");
