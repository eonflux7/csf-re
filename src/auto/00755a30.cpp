// Lower-bound search in a std::map-style red-black tree keyed by uint32_t.
// Node layout: +0x00 left, +0x08 right, +0x0C key, +0x11 isnil flag.
// The map object holds its head sentinel at +4; the sentinel's +4 is the root.
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

static int** __fastcall FindLowerBoundNode(int** self, void* /*edx*/, int** outSlot, const uint32_t* keyPtr)
{
    uint8_t* head = reinterpret_cast<uint8_t*>(self[1]);
    uint8_t* candidate = head;
    uint8_t* node = CSF_AT(uint8_t*, head, 0x04);
    const uint32_t key = *keyPtr;

    // Walk down from the root while nodes are not the nil sentinel.
    while (CSF_AT(uint8_t, node, 0x11) == 0)
    {
        if (CSF_AT(uint32_t, node, 0x0C) >= key)
        {
            candidate = node;
            node = CSF_AT(uint8_t*, node, 0x00);
        }
        else
        {
            node = CSF_AT(uint8_t*, node, 0x08);
        }
    }

    uint8_t* result;
    if (candidate == head || key < CSF_AT(uint32_t, candidate, 0x0C))
    {
        result = head;
    }
    else
    {
        result = candidate;
    }

    *outSlot = reinterpret_cast<int*>(result);
    return outSlot;
}

HOOK(0x00755A30, FindLowerBoundNode, "auto");
