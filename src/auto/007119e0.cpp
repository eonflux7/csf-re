#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Propagates a flag/mask pair to the two child objects stored at +0x08 and +0x0C.
// Returns the last child call result (or 0 if the last child slot is empty).
static int __fastcall C_00863d80_SetFlagAndPropagate(C_00863d80* self, void* edx, int flag, int mask)
{
    // Current stored flag byte; if it already matches, nothing changes.
    uint8_t currentFlag = self->field_01b3;
    int result = currentFlag;

    if (static_cast<uint8_t>(flag) == currentFlag) {
        return result;
    }

    self->field_01b3 = static_cast<uint8_t>(flag);

    // Working copy of the entry edx value: its low byte is rewritten per child.
    uint32_t argWord = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(edx));

    for (int childIndex = 0; childIndex < 2; ++childIndex) {
        uint32_t childValue = CSF_AT(uint32_t, self, 0x08 + childIndex * 4);
        result = static_cast<int>(childValue);

        if (childValue != 0) {
            // Child 0 receives 0xFF in the low byte, child 1 receives 0.
            argWord = (argWord & 0xFFFFFF00u) | static_cast<uint8_t>(childIndex == 1 ? 0u : 0xFFu);
            int maskedArg = static_cast<int>(argWord & static_cast<uint32_t>(mask));

            void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childValue));
            // Child slot +0x8 (index 2): stdcall-style args via thiscall on the child.
            result = ((int(__fastcall*)(void*, void*, int, int))CSF_AT(void**, child, 0)[2])(
                child, nullptr, flag, maskedArg);
        }
    }

    return result;
}

HOOK(0x7119E0, C_00863d80_SetFlagAndPropagate, "auto");
