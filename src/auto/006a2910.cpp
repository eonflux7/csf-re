#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Callee at 0x6A2510: __stdcall, 2 stack args, ret 8. The second is a pointer to a 16-byte
// descriptor (name, two dwords, type flags) that the callee reads and may write.
CSF_FUNC(char, __stdcall, CoreZlibLog_ReadPropertyIntoDescriptor, 0x006A2510, (void* node, void* descriptor));

// Walks the 16-byte descriptors stored between [list+4] (begin) and [list+8] (end): copies each
// to the stack, lets the callee fill it from the node and writes the copy back.
// Returns the address of the last descriptor written, or 0 when the list is empty.
static int __fastcall S_00692b80_ApplyComponentCallback(S_00692b80* self, void* /*edx*/,
                                                        S_00692b80* a1, void* componentList)
{
    (void)self;

    int result = 0;
    int count = 0;
    if (CSF_AT(uint32_t, componentList, 4) != 0)
        count = (CSF_AT(int, componentList, 8)
                 - static_cast<int>(CSF_AT(uint32_t, componentList, 4))) >> 4;

    if (count == 0)
        return result;

    int offset = 0;
    int remaining = count;
    do {
        // Re-read the list begin at each iteration: the callback may change the list.
        uint32_t descriptor[4];
        std::memcpy(descriptor, CSF_AT(uint8_t*, componentList, 4) + offset, 16);

        CoreZlibLog_ReadPropertyIntoDescriptor(a1, descriptor);

        uint8_t* writeElement = CSF_AT(uint8_t*, componentList, 4) + offset;
        std::memcpy(writeElement, descriptor, 16);
        result = static_cast<int>(reinterpret_cast<uintptr_t>(writeElement));

        offset += 16;
        --remaining;
    } while (remaining != 0);

    return result;
}

HOOK(0x006A2910, S_00692b80_ApplyComponentCallback, "auto");
