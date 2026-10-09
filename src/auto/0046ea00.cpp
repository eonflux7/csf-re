#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MissionScript_ResolveName, 0x0046E4E0,
         (void* self, void* edx, char* String1));
CSF_FUNC(int, __fastcall, MissionScript_ParseEntryValue, 0x0046E910,
         (void* self, void* edx, void* a2, int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, AttributeNode_GetType, 0x006927F0,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40,
         (std::uint8_t* attribute, void* edx, char* buffer, int capacity));

[[maybe_unused]] static const auto g_NodeVtable = CSF_VTBL(0x00692B80);

static std::uint32_t __fastcall S_0046c260_ParseNamedEntry(
    S_0046c260* self, void* /*edx*/, S_00692b80* node, int* entry)
{
    std::uint32_t result =
        static_cast<std::uint32_t>(AttributeNode_GetType(node, nullptr));
    if (result != 1)
        return result;

    result = MenuExtras_GetChildCount(node, nullptr);
    if (result == 0)
        return result;

    char buffer[256] = {};
    const std::uint32_t nameComponent =
        GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 0);
    CopyAttributeText(reinterpret_cast<std::uint8_t*>(nameComponent),
                      nullptr, buffer, 256);

    entry[0] = MissionScript_ResolveName(self, nullptr, buffer);

    result = MenuExtras_GetChildCount(node, nullptr);
    if (result < 2)
        return result;

    result = GetComponent(reinterpret_cast<unsigned char*>(node), nullptr, 1u);
    if (result == 0)
        return result;

    void* valueComponent = reinterpret_cast<void*>(result);
    void* value = Mem_Alloc(8);
    if (value)
    {
        CSF_AT(std::uint16_t, value, 0) = 0;
        CSF_AT(std::int16_t, value, 2) = -1;
        CSF_AT(std::uint32_t, value, 4) = 0;
    }

    entry[1] = static_cast<int>(reinterpret_cast<std::intptr_t>(value));
    return static_cast<std::uint32_t>(
        MissionScript_ParseEntryValue(self, nullptr, valueComponent, entry[1]));
}

HOOK(0x0046EA00, S_0046c260_ParseNamedEntry, "auto");
