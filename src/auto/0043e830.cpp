#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, SelectionData_BuildFromIndex, 0x00698DF0, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

CSF_GLOBAL(std::uint32_t, g_BriefingExtensionWord, 0x00830FA0);
CSF_GLOBAL(char, g_BriefingExtensionTerminator, 0x00830FA4);

static std::uint32_t __fastcall Campaign_LoadResourceRange(
    std::uint32_t* self, void* /*edx*/,
    std::uint32_t firstIndex, std::uint32_t lastIndex)
{
    const std::uint32_t begin = CSF_AT(std::uint32_t, self, 0x34);
    if (!begin)
        return begin;

    const std::uint32_t byteCount =
        CSF_AT(std::uint32_t, self, 0x38) - begin;
    const std::uint32_t resourceCount = static_cast<std::uint32_t>(
        static_cast<std::int32_t>(byteCount) / 196);
    if (!resourceCount)
        return resourceCount;

    if (lastIndex >= resourceCount)
        lastIndex = resourceCount - 1;
    if (firstIndex > lastIndex)
        return firstIndex;

    std::uint32_t resourceOffset = 196 * firstIndex;
    std::uint32_t remaining = lastIndex - firstIndex + 1;

    alignas(8) std::uint8_t pathBytes[0xCC];
    alignas(8) std::uint8_t selectionBytes[0x8];
    char* const path = reinterpret_cast<char*>(pathBytes);
    char* const selection = reinterpret_cast<char*>(selectionBytes);

    do
    {
        const std::uint32_t resourceAddress =
            CSF_AT(std::uint32_t, self, 0x34) + resourceOffset;
        const int selectionIndex = CSF_AT(
            int, reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(resourceAddress)), 0x5C);

        selection[0] = 0;
        CSF_AT(int, selectionBytes, 0x1) = 0;
        SelectionData_BuildFromIndex(
            selectionIndex,
            static_cast<int>(reinterpret_cast<std::uintptr_t>(selection)));

        if (selection[0])
        {
            std::memcpy(path,
                CSF_STR(0x00830FA8, "Sounds\\Scenes\\Briefings\\"), 25);
            std::memset(path + 25, 0, 0xAC);
            CSF_AT(std::uint16_t, pathBytes, 0xC5) = 0;
            CSF_AT(char, pathBytes, 0xC7) = 0;

            const std::size_t selectionLength = std::strlen(selection) + 1;
            std::memcpy(path + std::strlen(path), selection, selectionLength);

            char* const extension = path + std::strlen(path);
            const std::uint32_t extensionWord = g_BriefingExtensionWord;
            const char extensionTerminator = g_BriefingExtensionTerminator;
            std::memcpy(extension, &extensionWord, sizeof(extensionWord));
            extension[4] = extensionTerminator;

            void* const resourceManager = reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(GetGlobalValue_70DDE0()));
            reinterpret_cast<void (__fastcall*)(void*, void*, char*)>(
                CSF_AT(void**, resourceManager, 0)[0xA0 / 4])(
                    resourceManager, nullptr, path);
        }

        resourceOffset += 196;
        --remaining;
    } while (remaining);

    return remaining;
}

HOOK(0x0043E830, Campaign_LoadResourceRange, "auto");
