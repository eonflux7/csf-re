#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(C_0082d410**, __fastcall, Hud_ResizeTextureEntries, 0x00435F80,
         (S_00434780* self, void* edx, unsigned int count,
          std::uint32_t stringPart0, std::uint32_t stringPart4,
          std::uint32_t stringPart8, std::uint32_t stringPart12,
          std::uint32_t stringPart16, std::uint32_t length,
          std::uint32_t capacity, std::uint32_t reserved,
          std::uint32_t left, std::uint32_t top,
          std::uint32_t width, std::uint32_t height));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910, (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(int, __cdecl, Hud_FindTextureEntryIndex, 0x00761410, (char* String1));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);
CSF_GLOBAL(const char*, g_pszKey_TEXTURE, 0x008995A4);
CSF_GLOBAL(const char*, g_pszKey_ID_hud, 0x008995A8);
CSF_GLOBAL(const char*, g_pszKey_TOP, 0x008995AC);
CSF_GLOBAL(const char*, g_pszKey_LEFT, 0x008995B0);
CSF_GLOBAL(const char*, g_pszKey_WIDTH, 0x008995B4);
CSF_GLOBAL(const char*, g_pszKey_HEIGHT, 0x008995B8);
CSF_GLOBAL(unsigned int, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(unsigned int, g_ScreenHeight, 0x00899BA0);

static void __fastcall Hud_LoadTextureLayout(void* self, void* /*edx*/,
                                            S_00692b80* definition)
{
    if (!definition)
        return;

    for (std::size_t offset = 0x168; offset <= 0x1E4; offset += 4)
        CSF_AT(std::uint32_t, self, offset) = 0;

    char buffer[256] = {};

    // The packet additionally classifies this direct callee as a vtable.
    (void)CSF_VTBL(0x00692B80);

    const unsigned int childCount = MenuExtras_GetChildCount(definition, nullptr);
    const unsigned int entryCount = childCount > 4 ? childCount : 4;

    // Pass the 48-byte entry template as twelve stack words. Together
    // with count, these match the original callee's 52-byte stack cleanup.
    Hud_ResizeTextureEntries(
        reinterpret_cast<S_00434780*>(static_cast<std::uint8_t*>(self) + 0x158),
        nullptr, entryCount,
        0, 0, 0, 0, 0, 0, 0xF, 0, 0, 0, 0, 0);

    for (unsigned int index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(definition), nullptr, index));

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_ID_hud),
                            buffer, 256);
        const int entryIndex = Hud_FindTextureEntryIndex(buffer);
        if (entryIndex == -1)
            continue;

        MenuExtras_ReadText(component, nullptr, const_cast<char*>(g_pszKey_TEXTURE),
                            buffer, 256);

        const std::uint32_t entryOffset = 48 * static_cast<std::uint32_t>(entryIndex);
        AssignStringRange_429B60(
            reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x15C) + entryOffset),
            nullptr, buffer, static_cast<unsigned int>(std::strlen(buffer)));

        std::uint32_t pixels =
            ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_TOP))
            * g_ScreenHeight;
        const std::uint32_t top = static_cast<std::uint32_t>(
            static_cast<std::uint64_t>(static_cast<double>(pixels) * g_ProgressScale));

        pixels =
            ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_LEFT))
            * g_ScreenWidth;
        const std::uint32_t left = static_cast<std::uint32_t>(
            static_cast<std::uint64_t>(static_cast<double>(pixels) * g_ProgressScale));

        pixels =
            ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_WIDTH))
            * g_ScreenWidth;
        const std::uint32_t width = static_cast<std::uint32_t>(
            static_cast<std::uint64_t>(static_cast<double>(pixels) * g_ProgressScale));

        pixels =
            ReadIntegerProperty(component, nullptr, const_cast<char*>(g_pszKey_HEIGHT))
            * g_ScreenHeight;
        const std::uint32_t height = static_cast<std::uint32_t>(
            static_cast<std::uint64_t>(static_cast<double>(pixels) * g_ProgressScale));

        auto* entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x15C) + entryOffset);
        CSF_AT(std::uint32_t, entry, 0x20) = left;
        CSF_AT(std::uint32_t, entry, 0x24) = top;

        entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, self, 0x15C) + entryOffset);
        CSF_AT(std::uint32_t, entry, 0x28) = width;
        CSF_AT(std::uint32_t, entry, 0x2C) = height;
    }
}

HOOK(0x00436140, Hud_LoadTextureLayout, "auto");
