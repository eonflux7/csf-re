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

// Mem_FreeDebug: __cdecl, one stack argument, rets=0
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Slot 16 (byte offset 0x40) of the object's vtable. The original passes the
// object in ecx and one out-buffer pointer on the stack.
static void* MenuObject_CallRefreshSlot(void* object, void* outBlock)
{
    return ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, object, 0)[16])(object, nullptr, outBlock);
}

static void* __fastcall MenuObject_Refresh(void* self, void* /*edx*/)
{
    void* result = CSF_AT(void*, self, 0x4);
    if (result)
    {
        // 0x18-byte stack frame used as the out-buffer:
        //   [0x00] Block[0] (pointer, low word cleared)
        //   [0x10] Block[4] (nullptr)
        //   [0x14] v3       (state counter, set to 7)
        alignas(8) uint8_t frame[0x18];
        std::memset(frame, 0, sizeof(frame));

        CSF_AT(uint32_t, frame, 0x14) = 7;          // v3 = 7
        CSF_AT(void*, frame, 0x10) = nullptr;       // Block[4] = nullptr
        CSF_AT(uint16_t, frame, 0x00) = 0;          // LOWORD(Block[0]) = 0

        result = MenuObject_CallRefreshSlot(result, frame);

        if (CSF_AT(uint32_t, frame, 0x14) >= 8)
        {
            result = CSF_AT(void*, frame, 0x00);
            if (result)
            {
                return reinterpret_cast<void*>(static_cast<intptr_t>(Mem_FreeDebug(result)));
            }
        }
    }
    return result;
}

HOOK(0x004386D0, MenuObject_Refresh, "auto");
