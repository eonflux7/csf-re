#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (original addresses)
CSF_FUNC(int, __stdcall, sub_6A7A00, 0x006A7A00, (int a1));
CSF_FUNC(void, __fastcall, sub_6A8A10, 0x006A8A10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6A9270, 0x006A9270, (void* self, void* edx));

// Original: void __thiscall sub_6A9780(S_006a8940* this, int a2), ret 4.
// Advances the net-gathering state machine when the state pair is (3, 2).
static void __fastcall NetGatheringDdl_ProcessPendingState(S_006a8940* self, void* /*edx*/, int /*a2*/)
{
    if (self->field_0008 == 3 && self->field_000c == 2) {
        int handlerCode = 0;
        int slotCount = static_cast<int>(self->field_0010);
        if (slotCount > 0 && slotCount <= 4)
            handlerCode = sub_6A7A00(static_cast<int>(self->field_0090));

        if (handlerCode != 0) {
            int branch = handlerCode - 1;
            if (branch == 0) {
                sub_6A9270(self, nullptr);
                return;
            }
            if (branch != 1)
                return;
        }

        // Reset path (handlerCode == 0 or handlerCode == 2)
        self->field_02b0 = 0x494C3130;
        sub_6A8A10(self, nullptr);
        self->field_0008 = 2;
        self->field_000c = 0;
        self->field_0334 = 0;
    }
}

HOOK(0x006A9780, NetGatheringDdl_ProcessPendingState, "auto");
