#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedElement, 0x00693360, (uint8_t* self, void* edx, const char* name));
CSF_FUNC(int, __fastcall, AnimationDefs_ResizeDataArray, 0x00774160, (void* self, void* edx, unsigned int a2));

CSF_GLOBAL(const char*, g_pszKey_LISTADATOS_8A9984, 0x008A9984);
CSF_GLOBAL(const char*, g_pszKey_NUM_ANIMS_PS2, 0x008A9990);
CSF_GLOBAL(const char*, g_pszKey_NUM_ANIMS_XBOX, 0x008A9994);

[[maybe_unused]] static const auto kComponentVtable = CSF_VTBL(0x00692B80);

static char __fastcall AnimationDefs_LoadAnimationRecordsWithCounts(
    void* self, void* /*edx*/, std::uint8_t* root)
{
    auto* list = LookupNamedElement(root, nullptr, g_pszKey_LISTADATOS_8A9984);
    const std::uint32_t childCount = MenuExtras_GetChildCount(list, nullptr);

    AnimationDefs_ResizeDataArray(CSF_AT(void*, self, 0x4), nullptr, childCount);

    std::uint32_t recordOffset = 0;
    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(list, nullptr, index));

        auto* property = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_NUM_ANIMS_PS2));
        int ps2AnimationCount = 1;
        if (property)
            ps2AnimationCount = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(property));

        property = FindProperty(
            component, nullptr, const_cast<char*>(g_pszKey_NUM_ANIMS_XBOX));
        int xboxAnimationCount = 1;
        if (property)
            xboxAnimationCount = ParseIntegerComponent(
                reinterpret_cast<std::uint32_t>(property));

        auto* destination = CSF_AT(std::uint8_t*, CSF_AT(void*, self, 0x4), 0x4)
                          + recordOffset;

        reinterpret_cast<void (__fastcall*)(
            void*, void*, std::uint8_t*, std::uint8_t*, int, int)>(
                CSF_AT(void**, self, 0)[0x14 / 4])(
                    self, nullptr, component, destination,
                    ps2AnimationCount, xboxAnimationCount);

        recordOffset += 0x60;
    }

    return 1;
}

HOOK(0x00774A20, AnimationDefs_LoadAnimationRecordsWithCounts, "auto");
