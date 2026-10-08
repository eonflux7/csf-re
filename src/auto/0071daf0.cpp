#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint8_t*, __fastcall, FindAttribute, 0x006932B0,
         (std::uint8_t* node, void* edx, char* name));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40,
         (std::uint8_t* attribute, void* edx, char* buffer, int capacity));

CSF_GLOBAL(char*, g_LookupAttribute, 0x008A8604);

static std::uint8_t* __stdcall CGuiBuilder_ResolveObjectFromAttribute(
    void* owner, std::uint8_t* node)
{
    char buffer[256] = {};

    if (!node)
        return nullptr;

    std::uint8_t* attribute =
        FindAttribute(node, nullptr, g_LookupAttribute);
    if (!attribute)
        return nullptr;

    CopyAttributeText(attribute, nullptr, buffer, 256);

    return reinterpret_cast<std::uint8_t* (__fastcall*)(
        void*, void*, char*)>(
            CSF_AT(void**, owner, 0)[0x258 / 4])(
                owner, nullptr, buffer);
}

HOOK(0x0071DAF0, CGuiBuilder_ResolveObjectFromAttribute, "auto");
