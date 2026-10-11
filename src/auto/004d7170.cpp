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

// CharTask_StartMovement (0x004DB140): __thiscall, one stack arg, ret 4.
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

// Hooked method, original 0x004D7170 (__thiscall, ret 4).
// Starts the movement task using one of two embedded sub-objects of the
// conversation task: the one at +0x14, or the one at +0x50 (= +0x14 + 60),
// depending on a virtual state query on the object stored at +0x10.
static int __fastcall TConversacion_StartMovementByState(void* self, void* /*edx*/, int /*a2*/)
{
    using StateQueryFn = int(__fastcall*)(void* self, void* edx);

    const uint32_t stateObjectValue = CSF_AT(uint32_t, self, 0x10);
    bool startFromPrimary = true;

    if (stateObjectValue != 0) {
        void* stateObject = reinterpret_cast<void*>(stateObjectValue);
        // Slot 128 / 4 = 32 in the state object's vtable.
        void* vfnSlot = CSF_AT(void**, stateObject, 0)[128 / 4];
        const int stateQueryResult = ((StateQueryFn)vfnSlot)(stateObject, nullptr);
        // Non-zero result selects the primary sub-object; zero selects the secondary one.
        startFromPrimary = (stateQueryResult != 0);
    }

    if (startFromPrimary) {
        uint8_t* primarySubObject = reinterpret_cast<uint8_t*>(self) + 0x14;
        return CharTask_StartMovement(self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(primarySubObject)));
    }

    uint8_t* secondarySubObject = reinterpret_cast<uint8_t*>(self) + 0x50;
    return CharTask_StartMovement(self, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(secondarySubObject)));
}

HOOK(0x004D7170, TConversacion_StartMovementByState, "auto");
