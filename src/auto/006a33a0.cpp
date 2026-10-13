#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t __cdecl CoreZlibLog_SiftDownHuffmanHeap(
    hook::Regs* regs, const uint32_t* args)
{
    auto* state = reinterpret_cast<uint8_t*>(regs->eax);
    auto* tree = reinterpret_cast<uint8_t*>(regs->edi);
    int32_t position = static_cast<int32_t>(args[0]);

    int32_t heapLength = CSF_AT(int32_t, state, 0x1448);
    const int32_t savedNode = CSF_AT(
        int32_t, state + static_cast<uint32_t>(position) * 4, 0xB54);
    int32_t child = static_cast<int32_t>(
        static_cast<uint32_t>(position) * 2);

    while (child <= heapLength)
    {
        if (child < heapLength)
        {
            const int32_t rightNode = CSF_AT(
                int32_t, state + static_cast<uint32_t>(child) * 4, 0xB58);
            const int32_t leftNode = CSF_AT(
                int32_t, state + static_cast<uint32_t>(child) * 4, 0xB54);
            const uint16_t rightFrequency = CSF_AT(
                uint16_t, tree + static_cast<uint32_t>(rightNode) * 4, 0);
            const uint16_t leftFrequency = CSF_AT(
                uint16_t, tree + static_cast<uint32_t>(leftNode) * 4, 0);

            bool useRight = rightFrequency < leftFrequency;
            if (rightFrequency == leftFrequency)
            {
                const uint8_t rightDepth = CSF_AT(
                    uint8_t, state + static_cast<uint32_t>(rightNode), 0x1450);
                const int32_t currentLeftNode = CSF_AT(
                    int32_t, state + static_cast<uint32_t>(child) * 4, 0xB54);
                useRight = rightDepth <= CSF_AT(
                    uint8_t, state + static_cast<uint32_t>(currentLeftNode), 0x1450);
            }

            if (useRight)
                child = static_cast<int32_t>(static_cast<uint32_t>(child) + 1);
        }

        const int32_t childNode = CSF_AT(
            int32_t, state + static_cast<uint32_t>(child) * 4, 0xB54);
        const uint16_t savedFrequency = CSF_AT(
            uint16_t, tree + static_cast<uint32_t>(savedNode) * 4, 0);
        const uint16_t childFrequency = CSF_AT(
            uint16_t, tree + static_cast<uint32_t>(childNode) * 4, 0);

        if (savedFrequency < childFrequency)
            break;

        if (savedFrequency == childFrequency)
        {
            const uint8_t savedDepth = CSF_AT(
                uint8_t, state + static_cast<uint32_t>(savedNode), 0x1450);
            if (savedDepth <= CSF_AT(
                    uint8_t, state + static_cast<uint32_t>(childNode), 0x1450))
                break;
        }

        CSF_AT(int32_t, state + static_cast<uint32_t>(position) * 4, 0xB54)
            = childNode;
        heapLength = CSF_AT(int32_t, state, 0x1448);
        position = child;
        child = static_cast<int32_t>(static_cast<uint32_t>(child) * 2);
    }

    CSF_AT(int32_t, state + static_cast<uint32_t>(position) * 4, 0xB54)
        = savedNode;
    return regs->eax;
}

HOOK_REGS(0x006A33A0, CoreZlibLog_SiftDownHuffmanHeap, 0, "core_zlib_log");
