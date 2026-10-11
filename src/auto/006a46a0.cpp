#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// pqdownheap: eax = state, edi = tree, one stack argument (heap index), the caller pops.
static void CoreZlibLog_RestoreHeap(std::uint32_t* state, std::uint8_t* tree, int heapIndex)
{
    hook::Regs r{};
    r.eax = reinterpret_cast<std::uint32_t>(state);
    r.edi = reinterpret_cast<std::uint32_t>(tree);
    std::uint32_t stack[1] = {static_cast<std::uint32_t>(heapIndex)};
    hook::call_regs(CSF_ADDR_FN(0x006A33A0), &r, stack, 1);
}

// The original takes the deflate state in esi and the tree descriptor on the stack (the caller pops it).
static std::uint32_t __cdecl CoreZlibLog_BuildTree(hook::Regs* regs, const std::uint32_t* args)
{
    auto* state = reinterpret_cast<std::uint32_t*>(regs->esi);
    void* descriptor = reinterpret_cast<void*>(args[0]);
    auto* tree = CSF_AT(std::uint8_t*, descriptor, 0x00);
    void* staticDescriptor = CSF_AT(void*, descriptor, 0x08);
    const int symbolCount = CSF_AT(int, staticDescriptor, 0x0C);
    auto* staticTree = CSF_AT(std::uint8_t*, staticDescriptor, 0x00);
    auto* heap = reinterpret_cast<std::uint8_t*>(state) + 0xB54;
    int maxCode = -1;

    CSF_AT(int, state, 0x1448) = 0;
    CSF_AT(int, state, 0x144C) = 573;

    for (int symbol = 0; symbol < symbolCount; ++symbol)
    {
        if (CSF_AT(std::uint16_t, tree, 4 * symbol) != 0)
        {
            const int heapLength = CSF_AT(int, state, 0x1448) + 1;
            CSF_AT(int, state, 0x1448) = heapLength;
            CSF_AT(int, heap, 4 * heapLength) = symbol;
            maxCode = symbol;
            CSF_AT(std::uint8_t, state, 0x1450 + symbol) = 0;
        }
        else
        {
            CSF_AT(std::uint16_t, tree, 4 * symbol + 2) = 0;
        }
    }

    while (CSF_AT(int, state, 0x1448) < 2)
    {
        const int symbol = maxCode >= 2 ? 0 : ++maxCode;
        const int heapLength = CSF_AT(int, state, 0x1448) + 1;
        CSF_AT(int, state, 0x1448) = heapLength;
        CSF_AT(int, heap, 4 * heapLength) = symbol;
        CSF_AT(std::uint16_t, tree, 4 * symbol) = 1;
        CSF_AT(std::uint8_t, state, 0x1450 + symbol) = 0;
        --CSF_AT(std::uint32_t, state, 0x16A0);
        if (staticTree)
        {
            CSF_AT(std::uint32_t, state, 0x16A4) -=
                CSF_AT(std::uint16_t, staticTree, 4 * symbol + 2);
        }
    }

    CSF_AT(int, descriptor, 0x04) = maxCode;
    for (int index = CSF_AT(int, state, 0x1448) / 2; index >= 1; --index)
        CoreZlibLog_RestoreHeap(state, tree, index);

    int nextNode = symbolCount;
    do
    {
        const int first = CSF_AT(int, state, 0xB58);
        const int heapLength = CSF_AT(int, state, 0x1448);
        const int replacement = CSF_AT(int, heap, 4 * heapLength);
        CSF_AT(int, state, 0x1448) = heapLength - 1;
        CSF_AT(int, state, 0xB58) = replacement;
        CoreZlibLog_RestoreHeap(state, tree, 1);

        const int second = CSF_AT(int, state, 0xB58);
        const int firstSortedIndex = CSF_AT(int, state, 0x144C) - 1;
        CSF_AT(int, state, 0x144C) = firstSortedIndex;
        CSF_AT(int, heap, 4 * firstSortedIndex) = first;
        const int secondSortedIndex = CSF_AT(int, state, 0x144C) - 1;
        CSF_AT(int, state, 0x144C) = secondSortedIndex;
        CSF_AT(int, heap, 4 * secondSortedIndex) = second;

        CSF_AT(std::uint16_t, tree, 4 * nextNode) =
            static_cast<std::uint16_t>(
                CSF_AT(std::uint16_t, tree, 4 * second) +
                CSF_AT(std::uint16_t, tree, 4 * first));

        const std::uint8_t secondDepth =
            CSF_AT(std::uint8_t, state, 0x1450 + second);
        const std::uint8_t firstDepth =
            CSF_AT(std::uint8_t, state, 0x1450 + first);
        CSF_AT(std::uint8_t, state, 0x1450 + nextNode) =
            static_cast<std::uint8_t>(
                (firstDepth >= secondDepth ? firstDepth : secondDepth) + 1);

        CSF_AT(std::uint16_t, tree, 4 * second + 2) =
            static_cast<std::uint16_t>(nextNode);
        CSF_AT(std::uint16_t, tree, 4 * first + 2) =
            static_cast<std::uint16_t>(nextNode);
        CSF_AT(int, state, 0xB58) = nextNode++;
        CoreZlibLog_RestoreHeap(state, tree, 1);
    }
    while (CSF_AT(int, state, 0x1448) >= 2);

    const int sortedIndex = CSF_AT(int, state, 0x144C) - 1;
    const int root = CSF_AT(int, state, 0xB58);
    CSF_AT(int, state, 0x144C) = sortedIndex;
    CSF_AT(int, heap, 4 * sortedIndex) = root;

    // gen_bitlen: eax = state, ecx = descriptor. gen_codes: edx = bl_count, edi = tree, stack = max code.
    hook::Regs r{};
    r.eax = reinterpret_cast<std::uint32_t>(state);
    r.ecx = reinterpret_cast<std::uint32_t>(descriptor);
    r.esi = reinterpret_cast<std::uint32_t>(state);
    r.edi = reinterpret_cast<std::uint32_t>(tree);
    hook::call_regs(CSF_ADDR_FN(0x006A3480), &r, nullptr, 0);

    r.edx = reinterpret_cast<std::uint32_t>(state) + 0xB34;
    std::uint32_t stack[1] = {static_cast<std::uint32_t>(maxCode)};
    return hook::call_regs(CSF_ADDR_FN(0x006A4610), &r, stack, 1);
}

HOOK_REGS(0x006A46A0, CoreZlibLog_BuildTree, 0, "auto");
