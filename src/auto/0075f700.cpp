#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, WaypointGraph_IteratorPrev, 0x0075EE00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, WaypointGraph_InsertNode, 0x0075F560, (uint32_t* a1, int a2, uint32_t* a3, char a4, uint32_t* a5, void* a6));

// Unique-key tree insert: finds the position for the value's key (a3[6] + a3[7]),
// writes {iterator, inserted} into the out pair and returns it.
static int __fastcall WaypointGraph_InsertUnique(int** self, void* /*edx*/, uint8_t* outPair, float* value)
{
    uint8_t* head = reinterpret_cast<uint8_t*>(self[1]);
    uint8_t* node = CSF_AT(uint8_t*, head, 4);
    uint8_t* parent = head;
    char goLeft = 1;
    char lastLeft = 1;

    while (!CSF_AT(uint8_t, node, 0x31)) {
        parent = node;
        float nodeKey = CSF_AT(float, node, 0x28) + CSF_AT(float, node, 0x24);
        float key = value[7] + value[6];
        if (key >= nodeKey) {
            node = CSF_AT(uint8_t*, node, 8);
            goLeft = 0;
            lastLeft = 0;
        } else {
            node = CSF_AT(uint8_t*, node, 0);
            goLeft = 1;
            lastLeft = 1;
        }
    }

    uint32_t* iter = reinterpret_cast<uint32_t*>(parent);
    uint8_t* cur = parent;

    if (goLeft) {
        uint8_t* first = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(self[1]), 0);
        if (parent == first) {
            uint32_t* res = WaypointGraph_InsertNode(reinterpret_cast<uint32_t*>(self), 0,
                                                     reinterpret_cast<uint32_t*>(&iter), 1,
                                                     reinterpret_cast<uint32_t*>(parent), value);
            CSF_AT(uint32_t, outPair, 0) = *res;
            CSF_AT(uint8_t, outPair, 4) = 1;
            return reinterpret_cast<int>(outPair);
        }
        WaypointGraph_IteratorPrev(&iter, nullptr);
        cur = reinterpret_cast<uint8_t*>(iter);
    }

    float curKey = CSF_AT(float, cur, 0x28) + CSF_AT(float, cur, 0x24);
    float newKey = value[7] + value[6];
    if (newKey > curKey) {
        uint32_t* res = WaypointGraph_InsertNode(reinterpret_cast<uint32_t*>(self), 0,
                                                 reinterpret_cast<uint32_t*>(&iter), lastLeft,
                                                 reinterpret_cast<uint32_t*>(parent), value);
        CSF_AT(uint32_t, outPair, 0) = *res;
        CSF_AT(uint8_t, outPair, 4) = 1;
        return reinterpret_cast<int>(outPair);
    }

    CSF_AT(uint32_t, outPair, 0) = reinterpret_cast<uint32_t>(cur);
    CSF_AT(uint8_t, outPair, 4) = 0;
    return reinterpret_cast<int>(outPair);
}

HOOK(0x0075F700, WaypointGraph_InsertUnique, "auto");
