#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static void* __cdecl CoreZlibLog_SiftDownHuffmanHeap(
    void* state, const void* tree, std::int32_t heapIndex)
{
    auto* stateBytes = static_cast<std::uint8_t*>(state);
    auto* treeBytes = static_cast<const std::uint8_t*>(tree);

    std::int32_t heapSize = CSF_AT(std::int32_t, state, 0x1448);
    const std::int32_t savedNode =
        CSF_AT(std::int32_t,
               stateBytes + static_cast<std::uint32_t>(heapIndex) * 4,
               0xB54);
    std::int32_t child =
        static_cast<std::int32_t>(static_cast<std::uint32_t>(heapIndex) * 2);

    while (child <= heapSize)
    {
        if (child < heapSize)
        {
            const std::int32_t rightNode =
                CSF_AT(std::int32_t,
                       stateBytes + static_cast<std::uint32_t>(child) * 4,
                       0xB58);
            const std::int32_t leftNode =
                CSF_AT(std::int32_t,
                       stateBytes + static_cast<std::uint32_t>(child) * 4,
                       0xB54);
            const std::uint16_t rightFrequency =
                CSF_AT(std::uint16_t,
                       treeBytes + static_cast<std::uint32_t>(rightNode) * 4,
                       0);
            const std::uint16_t leftFrequency =
                CSF_AT(std::uint16_t,
                       treeBytes + static_cast<std::uint32_t>(leftNode) * 4,
                       0);

            if (rightFrequency < leftFrequency)
            {
                ++child;
            }
            else if (rightFrequency == leftFrequency)
            {
                const std::uint8_t rightDepth =
                    CSF_AT(std::uint8_t, stateBytes + rightNode, 0x1450);
                const std::int32_t currentLeftNode =
                    CSF_AT(std::int32_t,
                           stateBytes + static_cast<std::uint32_t>(child) * 4,
                           0xB54);

                if (rightDepth <=
                    CSF_AT(std::uint8_t, stateBytes + currentLeftNode, 0x1450))
                {
                    ++child;
                }
            }
        }

        const std::int32_t childNode =
            CSF_AT(std::int32_t,
                   stateBytes + static_cast<std::uint32_t>(child) * 4,
                   0xB54);
        const std::uint16_t savedFrequency =
            CSF_AT(std::uint16_t,
                   treeBytes + static_cast<std::uint32_t>(savedNode) * 4,
                   0);
        const std::uint16_t childFrequency =
            CSF_AT(std::uint16_t,
                   treeBytes + static_cast<std::uint32_t>(childNode) * 4,
                   0);

        if (savedFrequency < childFrequency ||
            (savedFrequency == childFrequency &&
             CSF_AT(std::uint8_t, stateBytes + savedNode, 0x1450) <=
                 CSF_AT(std::uint8_t, stateBytes + childNode, 0x1450)))
        {
            break;
        }

        CSF_AT(std::int32_t,
               stateBytes + static_cast<std::uint32_t>(heapIndex) * 4,
               0xB54) = childNode;

        heapSize = CSF_AT(std::int32_t, state, 0x1448);
        heapIndex = child;
        child = static_cast<std::int32_t>(
            static_cast<std::uint32_t>(child) * 2);
    }

    CSF_AT(std::int32_t,
           stateBytes + static_cast<std::uint32_t>(heapIndex) * 4,
           0xB54) = savedNode;
    return state;
}

HOOK(0x006A33A0, CoreZlibLog_SiftDownHuffmanHeap, "auto");
