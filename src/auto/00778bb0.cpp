#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

struct BichoEntryStorage
{
    unsigned char bytes[0x2C];
};

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(unsigned int, __fastcall, sub_4FD1A0, 0x004FD1A0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0, (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0, (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));

// Flatten the by-value 44-byte entry into its eleven ABI stack words.
// Together with count, these are the twelve arguments consumed by ret 48.
CSF_FUNC(char, __fastcall, ResizeBichoEntries, 0x00778860,
    (void* self, void* edx, unsigned int count,
     std::uint32_t stringBase, std::uint32_t stringWord0,
     std::uint32_t stringWord1, std::uint32_t stringWord2,
     std::uint32_t stringWord3, unsigned int length,
     unsigned int capacity, std::uint32_t vectorBase,
     void* block, std::uint32_t vectorEnd, std::uint32_t vectorCapacityEnd));
CSF_FUNC(int*, __fastcall, sub_778A90, 0x00778A90, (void* self, void* edx, void* a3));

CSF_GLOBAL(const char*, g_pszKey_BICHOS_game_data_defs, 0x008A9B2C);
CSF_GLOBAL(const char*, g_pszKey_NOMBRE_game_data_defs_2, 0x008A9B30);

static void __fastcall CLevelData_LoadBichoDefinitions(
    CLevelData* self, void* /*edx*/, S_00692b80* node)
{
    // This reported vtable address aliases the direct child-count callee.
    (void)CSF_VTBL(0x00692B80);

    if (!node)
        return;
    if (!MenuExtras_GetChildCount(node, nullptr))
        return;

    const unsigned int childCount = MenuExtras_GetChildCount(node, nullptr);
    void* entries = reinterpret_cast<std::uint8_t*>(self) + 0x34;

    ResizeBichoEntries(entries, nullptr, childCount,
        0, 0, 0, 0, 0, 0, 0xF, 0, nullptr, 0, 0);

    for (unsigned int index = 0; index < childCount; ++index)
    {
        auto* component = reinterpret_cast<std::uint8_t*>(
            GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, index));
        if (!component)
            continue;

        char buffer[100];
        BichoEntryStorage entry;
        auto* entryString = reinterpret_cast<C_0082d410*>(&entry);

        CSF_AT(std::uint32_t, &entry, 0x18) = 15;
        CSF_AT(std::uint32_t, &entry, 0x14) = 0;
        reinterpret_cast<unsigned char*>(&entryString->field_0004)[0] = 0;
        CSF_AT(void*, &entry, 0x20) = nullptr;
        CSF_AT(std::uint32_t, &entry, 0x24) = 0;
        CSF_AT(std::uint32_t, &entry, 0x28) = 0;

        MenuExtras_ReadText(component, nullptr,
            const_cast<char*>(g_pszKey_BICHOS_game_data_defs), buffer, 100);
        AssignStringRange_429B60(entryString, nullptr, buffer,
            static_cast<unsigned int>(std::strlen(buffer)));

        auto* names = FindProperty(component, nullptr,
            const_cast<char*>(g_pszKey_NOMBRE_game_data_defs_2));
        if (names && MenuExtras_GetChildCount(names, nullptr))
        {
            const unsigned int nameCount = MenuExtras_GetChildCount(names, nullptr);
            sub_4FD1A0(entry.bytes + 0x1C, nullptr, nameCount);

            for (unsigned int nameIndex = 0; nameIndex < nameCount; ++nameIndex)
            {
                int* destination =
                    static_cast<int*>(CSF_AT(void*, &entry, 0x20)) + nameIndex;
                const std::uint32_t valueNode =
                    GetComponent(names, nullptr, nameIndex);
                const int value = ParseIntegerComponent(valueNode);
                *destination = value;
            }
        }

        sub_778A90(entries, nullptr, &entry);

        if (CSF_AT(void*, &entry, 0x20))
            Mem_FreeDebug(CSF_AT(void*, &entry, 0x20));

        CSF_AT(void*, &entry, 0x20) = nullptr;
        CSF_AT(std::uint32_t, &entry, 0x24) = 0;
        CSF_AT(std::uint32_t, &entry, 0x28) = 0;

        if (CSF_AT(std::uint32_t, &entry, 0x18) >= 0x10)
        {
            if (entryString->field_0004)
                Mem_FreeDebug(reinterpret_cast<void*>(entryString->field_0004));
        }

        CSF_AT(std::uint32_t, &entry, 0x18) = 15;
        CSF_AT(std::uint32_t, &entry, 0x14) = 0;
        reinterpret_cast<unsigned char*>(&entryString->field_0004)[0] = 0;
    }
}

HOOK(0x00778BB0, CLevelData_LoadBichoDefinitions, "auto");
