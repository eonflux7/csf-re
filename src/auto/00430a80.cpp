#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, Hud_ConvertSize, 0x0042DE60, (float* a1));
CSF_FUNC(uint64_t, __cdecl, Hud_ConvertPosition, 0x0042F3B0, (int* a1));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

CSF_GLOBAL(const char*, g_pszKey_TOP, 0x008995AC);
CSF_GLOBAL(const char*, g_pszKey_LEFT, 0x008995B0);
CSF_GLOBAL(const char*, g_pszKey_FUENTE, 0x008995FC);

static const auto kMenuNodeVtable = CSF_VTBL(0x00692B80);

static void __fastcall Hud_InitializeTextWidget(
    void* self, void* /*edx*/, S_00692b80* node)
{
    if (!node || MenuExtras_GetChildCount(node, nullptr) != 2)
        return;

    char fontName[256] = {};
    auto* fontComponent = reinterpret_cast<std::uint8_t*>(
        GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 0));
    if (!fontComponent)
        return;

    MenuExtras_ReadText(fontComponent, nullptr,
                       const_cast<char*>(g_pszKey_FUENTE), fontName, 256);

    void* manager = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(void*, self, 0x250) =
        reinterpret_cast<void* (__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, manager, 0)[20])(manager, nullptr, fontName);

    auto* positionComponent = reinterpret_cast<std::uint8_t*>(
        GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 1u));
    if (!positionComponent)
        return;

    int top = ReadIntegerProperty(
        positionComponent, nullptr, const_cast<char*>(g_pszKey_TOP));
    CSF_AT(int, self, 0x254) = ReadIntegerProperty(
        positionComponent, nullptr, const_cast<char*>(g_pszKey_LEFT));
    CSF_AT(int, self, 0x258) = top;

    int* position = reinterpret_cast<int*>(
        reinterpret_cast<std::uint8_t*>(self) + 0x254);
    Hud_ConvertPosition(position);

    void* widget = CSF_AT(void*, self, 0x250);
    reinterpret_cast<void (__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, widget, 0)[20])(widget, nullptr, position);

    widget = CSF_AT(void*, self, 0x250);
    reinterpret_cast<void (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, widget, 0)[15])(
            widget, nullptr, CSF_STR(0x0083098C, "                              "));

    float size[2] = {0.0f, 0.0f};
    Hud_ConvertSize(size);

    widget = CSF_AT(void*, self, 0x250);
    reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, widget, 0)[38])(widget, nullptr, size);
}

HOOK(0x00430A80, Hud_InitializeTextWidget, "auto");
