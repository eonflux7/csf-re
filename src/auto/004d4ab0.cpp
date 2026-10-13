#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// TFncGirar_OnRotationComplete: __thiscall callee, ret 4
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (TFncGirar* self, void* , int a2));

// Polls the owner's actor rotation state. When the actor is no longer rotating
// and its status flag bit 1 is clear, completes the rotation task with flag 1.
static void __fastcall TFncGirar_TickRotation(TFncGirar* self, void* /*edx*/, int a2) {
    (void)a2;

    // self->owner (+0x08) -> owner+0x14 -> actor pointer at +0x30
    void* owner = CSF_AT(void*, self, 0x08);
    void* actor = CSF_AT(void*, CSF_AT(void*, owner, 0x14), 0x30);

    // vtbl slot 0xB8 / 4 = 46: bool IsRotating()
    bool isRotating = reinterpret_cast<bool(__fastcall*)(void*, void*)>(
        CSF_AT(void**, actor, 0)[0xB8 / 4])(actor, nullptr);

    bool shouldFinish = isRotating;
    if (!shouldFinish) {
        // vtbl slot 0xBC / 4 = 47: returns pointer to a status dword written into the out buffer
        alignas(4) uint8_t statusBytes[4];
        uint32_t* statusPtr = reinterpret_cast<uint32_t*>(
            reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, actor, 0)[0xBC / 4])(actor, nullptr, statusBytes));
        shouldFinish = ((*statusPtr & 2) == 0);
    }

    if (shouldFinish) {
        TFncGirar_OnRotationComplete(self, nullptr, 1);
    }
}

HOOK(0x004D4AB0, TFncGirar_TickRotation, "auto");
