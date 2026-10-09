#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* AsPointer(int value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int __stdcall CopyResolvedHandleTextToOwner(int a1, int a2, int a3)
{
    // Clear the owner's text first: AssignStringRange(owner+4, "", 0).
    uint32_t ownerPtr = CSF_AT(uint32_t, AsPointer(a1), 4);
    AssignStringRange_429B60(AsPointer(static_cast<int>(ownerPtr + 4)), nullptr,
                             reinterpret_cast<char*>(&g_byte_82E65C), 0);

    // Walk a3 -> object -> slot record.
    uint32_t requestObj = CSF_AT(uint32_t, AsPointer(a3), 0);
    uint32_t slotRecord = CSF_AT(uint32_t, AsPointer(static_cast<int>(requestObj)), 4);
    int32_t slotIndex = static_cast<int32_t>(CSF_AT(uint32_t, AsPointer(static_cast<int>(slotRecord)), 12));
    int result = static_cast<int>(slotRecord + 4);

    if (slotIndex >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result)
        {
            char* resolvedText = reinterpret_cast<char*>(static_cast<uintptr_t>(static_cast<uint32_t>(result)) + 88);
            // Owner pointer is re-read from a1 after the previous call, as the original does.
            uint32_t ownerPtrAgain = CSF_AT(uint32_t, AsPointer(a1), 4);
            AssignStringRange_429B60(AsPointer(static_cast<int>(ownerPtrAgain + 4)), nullptr,
                                     resolvedText, static_cast<unsigned int>(strlen(resolvedText)));
        }
    }
    return result;
}

HOOK(0x465060, CopyResolvedHandleTextToOwner, "auto");
