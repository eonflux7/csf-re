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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destroys the trailing task block at +0x18 and the trailing slot array at +0x08.
// The slot-array cleanup (formerly a tail jump to 0x4B24F0) is inlined here, since
// 0x4B24F0 is not a function start in the original binary.
static int __fastcall AiCore_DestroyTrailingTask(void* self, void* /*edx*/)
{
    int result = reinterpret_cast<int>(CSF_VTBL(0x0082D410));

    CSF_AT(void*, self, 0x4C) = CSF_VTBL(0x0082D410);
    CSF_AT(void*, self, 0x34) = CSF_VTBL(0x0082D410);

    void* taskBlock = CSF_AT(void*, self, 0x18);
    if (taskBlock != nullptr) {
        // The count header sits just before the object.
        if (CSF_AT(int, static_cast<uint8_t*>(taskBlock) - 4, 0) != 0) {
            // Virtual destructor, slot 0, with flag 3.
            result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, taskBlock, 0)[0])(taskBlock, nullptr, 3);
            CSF_AT(void*, self, 0x18) = nullptr;
        } else {
            result = Mem_Free(static_cast<uint8_t*>(taskBlock) - 4);
            CSF_AT(void*, self, 0x18) = nullptr;
        }
    }

    // Trailing slot array at +0x08: reset each element's two vtable slots, then free it.
    void* slotArray = CSF_AT(void*, self, 0x08);
    if (slotArray != nullptr) {
        int slotCount = CSF_AT(int, static_cast<uint8_t*>(slotArray) - 4, 0);
        uint8_t* cursor = static_cast<uint8_t*>(slotArray) + slotCount * 0x34 + 12;
        for (int remaining = slotCount; remaining > 0; --remaining) {
            cursor -= 0x34;
            CSF_AT(void*, cursor, 0x18) = CSF_VTBL(0x0082D410);
            CSF_AT(void*, cursor, 0x00) = CSF_VTBL(0x0082D410);
        }
        result = Mem_Free(static_cast<uint8_t*>(slotArray) - 4);
        CSF_AT(void*, self, 0x08) = nullptr;
    }

    return result;
}

HOOK(0x004B9660, AiCore_DestroyTrailingTask, "auto");
