#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Advances the 93 cheat timer slots (stride 20 bytes, state dword at +12, float at +20
// relative to the start of the object). Slots in state 1 move to 2, slots in state 2
// accumulate delta, and slots in state 3 are reset to 0. If any slot was touched, the
// global state's object at +0x14 receives a virtual call in slot 42 (offset 0xA8).
static int __fastcall Cheats_AdvanceTimerSlots(void* self, void* /*edx*/, float delta) {
    bool anySlotTouched = false;

    for (int i = 0; i < 93; ++i) {
        const int offState = 12 + 20 * i;
        const int offValue = offState + 8;
        const int state = CSF_AT(int, self, offState);

        if (state == 1) {
            CSF_AT(int, self, offState) = 2;
            CSF_AT(float, self, offValue) = delta + CSF_AT(float, self, offValue);
            anySlotTouched = true;
        } else if (state == 2) {
            CSF_AT(float, self, offValue) = delta + CSF_AT(float, self, offValue);
            anySlotTouched = true;
        } else if (state == 3) {
            CSF_AT(int, self, offState) = 0;
            anySlotTouched = true;
        }
    }

    // Address one past the last slot (this + 20 + 93 * 20).
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(self) + 20 + 93 * 20);

    if (anySlotTouched) {
        result = GetGlobalState_50D190();
        void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(result));
        const int v5 = CSF_AT(int, globalState, 0x14);
        if (v5 != 0) {
            void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(v5));
            using SlotFn = int(__fastcall*)(void*, void*);
            SlotFn fn = reinterpret_cast<SlotFn>(CSF_AT(void**, obj, 0)[0xa8 / 4]);
            return fn(obj, nullptr);
        }
    }
    return result;
}

HOOK(0x00428850, Cheats_AdvanceTimerSlots, "auto");
