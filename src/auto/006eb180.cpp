#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));
CSF_FUNC(int, __fastcall, C_00861e98_ProcessHierarchyCommand, 0x006E8CD0, (void* self, void* edx, int a2));

static unsigned int __fastcall C_00861e98_UpdateHierarchyMatrices(
    C_00861e98* self, void* /*edx*/, char forceUpdate)
{
    unsigned int result = CSF_AT(std::uint32_t, self, 0x1C);
    if (!result)
        return result;

    if ((CSF_AT(std::uint8_t, self, 0x58) & 4) != 0)
    {
        C_00861e98_ProcessHierarchyCommand(self, nullptr, 70);
        C_00861e98_ProcessHierarchyCommand(self, nullptr, 71);
        return C_00861e98_ProcessHierarchyCommand(self, nullptr, 72);
    }

    const std::uint32_t hierarchyCount = CSF_AT(std::uint32_t, self, 0x0C);
    for (std::uint32_t index = 0; index < hierarchyCount; ++index)
    {
        auto* entry = CSF_AT(std::uint8_t*, self, 0x04) + 36 * index;
        result = CSF_AT(std::uint32_t, entry, 0x14);

        if ((result & 0x200) == 0 && !forceUpdate)
            continue;

        bool shouldUpdate = (result & 4) != 0;
        if (!shouldUpdate && index == 0)
        {
            result = CSF_AT(std::uint32_t, self, 0x20);
            shouldUpdate = result != 0;
        }
        if (!shouldUpdate)
            continue;

        RpHAnimHierarchyUpdateMatrices(CSF_AT(int*, entry, 0x00));

        const int nodeCount = CSF_AT(
            int, CSF_AT(void*, self, 0x14), 0x04);
        for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
        {
            void* node = CSF_AT(
                void*, CSF_AT(std::uint8_t*, self, 0x4C), 4 * nodeIndex);
            if (node)
                CSF_AT(std::uint8_t, node, 0x20) = 0;
        }

        result = CSF_AT(std::uint32_t, entry, 0x14) & 0xFFFFFDFF;
        CSF_AT(std::uint32_t, entry, 0x14) = result;
    }
    return result;
}

HOOK(0x006EB180, C_00861e98_UpdateHierarchyMatrices, "auto");
