#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __stdcall Campaign_FreeBriefingNodeList(void** block);

// Walks a linked list of briefing nodes starting at `block`. Each node recurses into its
// child at +0x08, releases its out-of-line buffer at +0x10 when the capacity at +0x24 is
// at least 16, resets the node, and frees the node itself. Stops at the node whose +0x2D
// flag is set.
static char __stdcall Campaign_FreeBriefingNodeList(void** block)
{
    void** prevNode = block;
    char result = CSF_AT(char, block, 0x2d);
    void** curNode = block;

    while (!result)
    {
        Campaign_FreeBriefingNodeList(CSF_AT(void**, curNode, 0x08));
        curNode = CSF_AT(void**, curNode, 0x00);

        if (CSF_AT(uint32_t, prevNode, 0x24) >= 0x10 && CSF_AT(void*, prevNode, 0x10))
            Mem_FreeDebug(CSF_AT(void*, prevNode, 0x10));

        CSF_AT(uint32_t, prevNode, 0x24) = 15;
        CSF_AT(void*, prevNode, 0x20) = nullptr;
        CSF_AT(uint8_t, prevNode, 0x10) = 0;

        if (prevNode)
            Mem_FreeDebug(prevNode);

        result = CSF_AT(char, curNode, 0x2d);
        prevNode = curNode;
    }
    return result;
}

HOOK(0x0043F310, Campaign_FreeBriefingNodeList, "auto");
