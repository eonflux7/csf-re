#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, AppMain_GrowAliasColors, 0x00507F60, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(void, __fastcall, Hud_ReadColor, 0x00693A90, (void* self, void* edx, char* String1, int a3));

static char __fastcall S_005080e0_LoadAliasColors(
    S_005080e0* self, void* /*edx*/, S_00692b80* node)
{
    // This address is also listed as a class tag in the packet.
    (void)CSF_VTBL(0x00692B80);

    alignas(8) std::uint8_t colorBytes[0x18];
    auto& childCount = CSF_AT(std::uint32_t, colorBytes, 0x10);
    auto& childIndex = CSF_AT(std::uint32_t, colorBytes, 0x14);

    childCount = MenuExtras_GetChildCount(node, nullptr);
    childIndex = 0;
    if (childCount == 0)
        return 1;

    auto* entries = reinterpret_cast<std::uint8_t*>(self) + 0x34;

    do
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node),
                         nullptr, childIndex));

        alignas(8) std::uint8_t aliasBytes[0x28];
        auto* alias = reinterpret_cast<char*>(aliasBytes);
        MenuExtras_ReadText(
            component, nullptr,
            const_cast<char*>(CSF_STR(0x0083F794, ".ALIAS")), alias, 40);

        CSF_AT(std::uint32_t, colorBytes, 0x00) = 0;
        CSF_AT(std::uint32_t, colorBytes, 0x04) = 0;
        CSF_AT(std::uint32_t, colorBytes, 0x08) = 0;
        CSF_AT(float, colorBytes, 0x0C) = 1.0f;
        Hud_ReadColor(
            component, nullptr,
            const_cast<char*>(CSF_STR(0x0083F78C, ".RGB")),
            reinterpret_cast<int>(colorBytes));

        alignas(8) std::uint8_t temporaryBytes[0x2C];
        auto& temporary =
            *reinterpret_cast<C_0082d410*>(temporaryBytes);

        CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;
        CSF_AT(std::uint32_t, temporaryBytes, 0x1C) = 0;
        CSF_AT(std::uint32_t, temporaryBytes, 0x20) = 0;
        CSF_AT(std::uint32_t, temporaryBytes, 0x24) = 0;
        CSF_AT(float, temporaryBytes, 0x28) = 1.0f;

        AssignStringRange_429B60(
            &temporary, nullptr, alias,
            static_cast<unsigned int>(std::strlen(alias)));

        CSF_AT(std::uint32_t, temporaryBytes, 0x20) =
            CSF_AT(std::uint32_t, colorBytes, 0x04);
        CSF_AT(std::uint32_t, temporaryBytes, 0x24) =
            CSF_AT(std::uint32_t, colorBytes, 0x08);
        CSF_AT(std::uint32_t, temporaryBytes, 0x1C) =
            CSF_AT(std::uint32_t, colorBytes, 0x00);
        CSF_AT(std::uint32_t, temporaryBytes, 0x28) =
            CSF_AT(std::uint32_t, colorBytes, 0x0C);

        if (CSF_AT(std::uint32_t, entries, 0x08) >=
            CSF_AT(std::uint32_t, entries, 0x04))
        {
            AppMain_GrowAliasColors(
                entries, nullptr, CSF_AT(int, entries, 0x0C));
        }

        const std::uint32_t entryIndex =
            CSF_AT(std::uint32_t, entries, 0x08);
        auto* entry = CSF_AT(std::uint8_t*, entries, 0x00) +
                      0x2C * entryIndex;
        CSF_AT(std::uint32_t, entries, 0x08) = entryIndex + 1;

        AssignStringSubstring(entry, nullptr, &temporary, 0,
                              static_cast<std::uint32_t>(-1));

        CSF_AT(std::uint32_t, entry, 0x1C) =
            CSF_AT(std::uint32_t, temporaryBytes, 0x1C);
        CSF_AT(std::uint32_t, entry, 0x20) =
            CSF_AT(std::uint32_t, temporaryBytes, 0x20);
        CSF_AT(std::uint32_t, entry, 0x24) =
            CSF_AT(std::uint32_t, temporaryBytes, 0x24);
        CSF_AT(std::uint32_t, entry, 0x28) =
            CSF_AT(std::uint32_t, temporaryBytes, 0x28);

        if (CSF_AT(std::uint32_t, temporaryBytes, 0x18) >= 0x10 &&
            temporary.field_0004 != 0)
        {
            Mem_FreeDebug(
                reinterpret_cast<void*>(temporary.field_0004));
        }

        const std::uint32_t nextIndex = childIndex + 1;
        const bool moreChildren = nextIndex < childCount;
        CSF_AT(std::uint32_t, temporaryBytes, 0x18) = 15;
        CSF_AT(std::uint32_t, temporaryBytes, 0x14) = 0;
        reinterpret_cast<std::uint8_t*>(&temporary.field_0004)[0] = 0;
        childIndex = nextIndex;
        if (!moreChildren)
            break;
    } while (true);

    return 1;
}

HOOK(0x00508490, S_005080e0_LoadAliasColors, "auto");
