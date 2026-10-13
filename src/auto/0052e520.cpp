#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(std::uint32_t*, __fastcall, S_00440ee0_AppendElement196, 0x004413B0, (S_00440ee0* self, void* , int element));
CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0, (char* first, char* second));

static int __fastcall MenuScenarioSelect_AddScenarioByName(
    void* self, void* /*edx*/, int fallbackAddress, int slot, int nameAddress)
{
    auto* registry = reinterpret_cast<std::uint8_t*>(GetGlobal8afc7c());
    const std::uint32_t beginAddress =
        CSF_AT(std::uint32_t, registry, 0x34);
    auto* scenarios = registry + 0x30;

    std::uint32_t count = 0;
    if (beginAddress != 0)
    {
        const std::int32_t byteCount = static_cast<std::int32_t>(
            CSF_AT(std::uint32_t, scenarios, 8) - beginAddress);
        count = static_cast<std::uint32_t>(byteCount / 196);
    }

    auto* requestedName = reinterpret_cast<std::uint8_t*>(nameAddress);
    std::uint32_t index = 0;
    std::uint32_t byteOffset = 0;

    while (index < count)
    {
        auto* entry = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, scenarios, 4) + byteOffset);

        char* candidateText =
            CSF_AT(std::uint32_t, entry, 0x1C) < 0x10
                ? reinterpret_cast<char*>(entry + 8)
                : CSF_AT(char*, entry, 8);

        char* requestedText =
            CSF_AT(std::uint32_t, requestedName, 0x18) < 0x10
                ? reinterpret_cast<char*>(requestedName + 4)
                : CSF_AT(char*, requestedName, 4);

        if (SceneNamesMatch(requestedText, candidateText))
            break;

        byteOffset += 196;
        ++index;
    }

    auto* selected = reinterpret_cast<std::uint8_t*>(fallbackAddress);
    if (index < count)
    {
        selected = reinterpret_cast<std::uint8_t*>(
            CSF_AT(std::uint32_t, scenarios, 4) + 196 * index);
    }

    void* controller = CSF_AT(void*, self, 0x54);
    ((void(__fastcall*)(void*, void*, int, int, std::uint32_t, int))
        CSF_AT(void**, controller, 0)[652 / 4])(
            controller, nullptr, slot, 0,
            CSF_AT(std::uint32_t, selected, 0xB8), slot);

    S_00440ee0_AppendElement196(
        reinterpret_cast<S_00440ee0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x44),
        nullptr, reinterpret_cast<int>(selected));

    return index < count;
}

HOOK(0x0052E520, MenuScenarioSelect_AddScenarioByName, "auto");
