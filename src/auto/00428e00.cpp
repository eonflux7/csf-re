#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));

// The original's vtable references to S_0041e5f0 (0x41E5F0); the grow path never stores a vtable.
static const auto kGrowArrayVtbl = CSF_VTBL(0x0041E5F0);

// Resets outList's count, grows it to at least 20 entries, then appends the slot index (68..87)
// of every cheat slot in this object's table (stride 20 at +0x558) whose first dword is not -1.
// Returns 1 (a byte return: the original sets al only).
static char __fastcall C_00830030_CollectActiveCheatSlots(C_00830030* self, void* /*edx*/, S_0041e5f0* outList)
{
    uint32_t initialCapacity = outList->field_0004;
    outList->field_0008 = 0;
    if (initialCapacity < 0x14)
        S_0041e5f0_GrowArrayCapacity(outList, nullptr, (int)(20 - initialCapacity));

    bool done = false;
    unsigned int slotIndex = 68;
    auto* slot = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x558);
    do
    {
        if (done)
            break;
        done = slot->field_0000 == 0xFFFFFFFFu;
        if (slot->field_0000 != 0xFFFFFFFFu)
        {
            if (outList->field_0008 >= outList->field_0004)
                S_0041e5f0_GrowArrayCapacity(outList, nullptr, (int)outList->field_000c);
            uint32_t writeIndex = outList->field_0008;
            uint32_t* dst = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(outList->field_0000) + 4u * writeIndex);
            outList->field_0008 = writeIndex + 1;
            *dst = slotIndex;
        }
        ++slotIndex;
        slot = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(slot) + 20);
    } while (slotIndex <= 0x57);

    return 1;
}

HOOK(0x428E00, C_00830030_CollectActiveCheatSlots, "auto");
