#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"
#include "core/memory.h"

// Callee at 0x6A2510: __stdcall, 2 stack args, ret 8. The original pushes a pointer to a local
// float as the second argument (lea eax,[ebp-0x14]; push eax), so the callee writes through it.
CSF_FUNC(char, __stdcall, S_006A2510_ProcessComponent, 0x006A2510, (void* a1, float ComponentFloat));

// Walks a list of 16-byte components (float + 3 dwords) stored between [list+4] (begin) and
// [list+8] (end), passes each component's float to the callback, and writes it back.
// Returns the address of the last component written, or 0 when the list is empty.
static int __fastcall S_00692b80_ApplyComponentCallback(S_00692b80* self, void* /*edx*/,
                                                        S_00692b80* a1, void* componentList)
{
    (void)self;

    // Element count = (end - begin) / 16, zero when begin is null.
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
        uint8_t* element = CSF_AT(uint8_t*, componentList, 4) + offset;
        float componentFloat = CSF_AT(float, element, 0);
        uint32_t extraA = CSF_AT(uint32_t, element, 4);
        uint32_t extraB = CSF_AT(uint32_t, element, 8);
        uint32_t extraC = CSF_AT(uint32_t, element, 12);

        // The callee receives the address of componentFloat in its stack slot, so pass the
        // pointer bits through the float parameter of the bound signature.
        uint32_t pointerBits = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&componentFloat));
        float pointerAsArg;
        std::memcpy(&pointerAsArg, &pointerBits, sizeof(pointerAsArg));
        S_006A2510_ProcessComponent(a1, pointerAsArg);

        uint8_t* writeElement = CSF_AT(uint8_t*, componentList, 4) + offset;
        CSF_AT(float, writeElement, 0) = componentFloat;
        CSF_AT(uint32_t, writeElement, 4) = extraA;
        CSF_AT(uint32_t, writeElement, 8) = extraB;
        CSF_AT(uint32_t, writeElement, 12) = extraC;
        result = static_cast<int>(reinterpret_cast<uintptr_t>(writeElement));

        offset += 16;
        --remaining;
    } while (remaining != 0);

    return result;
}

HOOK(0x006A2910, S_00692b80_ApplyComponentCallback, "auto");
