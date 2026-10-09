#include <cstdint>
#include "globals.h"
#include "hook.h"

// Binary search over a sorted vector of node pointers (begin at +4, end at +8).
// Each node stores its sort key (uint32) at +4. Returns the node whose key equals
// keyToFind; otherwise returns the first element.
static int __fastcall MissionScript_GetNodeDefinition(void* self, void* /*edx*/, uint32_t keyToFind)
{
    uint32_t beginAddr = CSF_AT(uint32_t, self, 0x4);
    int elementCount = 0;
    if (beginAddr != 0) {
        uint32_t endAddr = CSF_AT(uint32_t, self, 0x8);
        elementCount = static_cast<int>(endAddr - beginAddr) >> 2;
    }

    uint32_t* elements = reinterpret_cast<uint32_t*>(beginAddr);
    int lowIndex = 0;
    int highIndex = elementCount - 1;

    if (highIndex < 0)
        return static_cast<int>(elements[0]);

    while (true) {
        int midIndex = (highIndex + lowIndex) / 2;
        uint8_t* node = reinterpret_cast<uint8_t*>(elements[midIndex]);
        uint32_t nodeKey = CSF_AT(uint32_t, node, 0x4);

        if (keyToFind > nodeKey) {
            lowIndex = midIndex + 1;
        } else if (keyToFind < nodeKey) {
            highIndex = midIndex - 1;
        } else {
            return static_cast<int>(elements[midIndex]);
        }

        if (lowIndex > highIndex)
            return static_cast<int>(elements[0]);
    }
}

HOOK(0x00769360, MissionScript_GetNodeDefinition, "auto");
