#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee pushes four cdecl arguments (eax, ecx, edx, a2) and the caller pops 0x10.
CSF_FUNC(int, __cdecl, sub_558DA0, 0x00558DA0, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Walks 20-byte slots from firstSlot up to endSlot. Each slot holds a pointer at +8,
// a companion value at +12 and a third dword at +16. Releases the entry and clears the slot.
static int __cdecl ModRules_ReleaseSlotRange(int firstSlot, int endSlot)
{
    int result = endSlot;
    if (firstSlot != endSlot)
    {
        int cursor = firstSlot;
        int* slot = reinterpret_cast<int*>(firstSlot + 8);
        do
        {
            if (slot[0])
            {
                sub_558DA0(slot[0], slot[1], reinterpret_cast<int>(slot - 1), endSlot);
                // Re-read after the callee: it may have changed the pointer.
                if (slot[0])
                    Mem_FreeDebug(reinterpret_cast<void*>(slot[0]));
            }
            result = endSlot;
            slot[0] = 0;
            slot[1] = 0;
            slot[2] = 0;
            cursor += 20;
            slot += 5;
        } while (cursor != endSlot);
    }
    return result;
}

HOOK(0x00559910, ModRules_ReleaseSlotRange, "auto");
