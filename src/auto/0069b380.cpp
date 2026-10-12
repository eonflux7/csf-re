#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the byte at +0x29 of `self` to `value`, then sets the same byte on every
// object pointed to by the pointer array at +0x2C (count at +0x34).
// Returns the number of child entries visited (0 if there are none).
static unsigned int __fastcall SetFlag29PropagateToChildren(void* self, void* /*edx*/, char value)
{
    uint32_t childCount = CSF_AT(uint32_t, self, 0x34);
    unsigned int visited = 0;

    CSF_AT(uint8_t, self, 0x29) = static_cast<uint8_t>(value);

    if (childCount != 0) {
        do {
            void* child = reinterpret_cast<void*>(CSF_AT(uint32_t*, self, 0x2C)[visited]);
            CSF_AT(uint8_t, child, 0x29) = static_cast<uint8_t>(value);
            visited++;
        } while (visited < CSF_AT(uint32_t, self, 0x34));
    }

    return visited;
}

HOOK(0x69B380, SetFlag29PropagateToChildren, "auto");
