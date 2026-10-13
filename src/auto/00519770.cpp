#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Flips the boolean flag at +0x04 and writes the new value to +0x20 of every
// entry in the pointer list (list at +0x08, count at +0x10). Returns the number
// of entries visited.
static unsigned int __fastcall FlipFlagAndPropagateToList(void* self, void* /*edx*/)
{
    uint8_t* obj = static_cast<uint8_t*>(self);

    uint32_t initialCount = CSF_AT(uint32_t, obj, 0x10);
    uint8_t newFlag = static_cast<uint8_t>(CSF_AT(uint8_t, obj, 4) ^ 1);
    unsigned int visitedCount = 0;

    CSF_AT(uint8_t, obj, 4) = newFlag;

    if (initialCount != 0) {
        do {
            uint32_t* list = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, obj, 8)));
            uint8_t* element = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(list[visitedCount]));
            CSF_AT(uint8_t, element, 0x20) = newFlag;
            visitedCount++;
        } while (visitedCount < CSF_AT(uint32_t, obj, 0x10));
    }

    return visitedCount;
}

HOOK(0x00519770, FlipFlagAndPropagateToList, "auto");
