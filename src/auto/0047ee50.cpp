#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, AppendConstructSlot, 0x0047EEC0, (uint32_t* a1, int a2, uint32_t* a3, int a4, int a5));
CSF_FUNC(uint32_t*, __fastcall, GrowAndInsertSlot, 0x0047EF30, (void* self, void* edx, uint32_t* a2, void* Src, int a4));

// Appends one slot to the array at self (begin +4, end +8, capacity +12).
// If there is spare capacity, constructs the new element in place and advances end.
// Otherwise delegates to the grow/insert path.
static uint32_t* __fastcall AiAttackTasks_AppendSlotOrGrow(void* self, void* /*edx*/, uint32_t* pointerArg)
{
    uint32_t begin = CSF_AT(uint32_t, self, 0x04);

    int32_t usedCount = 0;
    if (begin != 0)
        usedCount = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x08) - begin) >> 2;

    if (begin == 0 ||
        static_cast<uint32_t>(usedCount) >=
            static_cast<uint32_t>(static_cast<int32_t>(CSF_AT(uint32_t, self, 0x0C) - begin) >> 2))
    {
        uint32_t endPtr = CSF_AT(uint32_t, self, 0x08);
        return GrowAndInsertSlot(self, nullptr,
                                 reinterpret_cast<uint32_t*>(&pointerArg),
                                 reinterpret_cast<void*>(static_cast<uintptr_t>(endPtr)),
                                 static_cast<int>(reinterpret_cast<uintptr_t>(pointerArg)));
    }

    uint32_t* slot = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));
    uint32_t* result = AppendConstructSlot(slot, 1, pointerArg,
                                           static_cast<int>(reinterpret_cast<uintptr_t>(self)),
                                           0);
    CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(slot + 1));
    return result;
}

HOOK(0x0047EE50, AiAttackTasks_AppendSlotOrGrow, "auto");

