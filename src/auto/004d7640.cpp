#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee: TFncGirar::OnRotationComplete (__thiscall, 1 stack arg, ret 4)
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

// TFncGirar::CompleteRotationIfDone (__thiscall, 1 stack arg, ret 4).
// Asks the rotation-state object reached through owner->+0x14->+0x44 whether
// its virtual slot 0x90 reports done; if so, notifies this task with 0.
static void __fastcall TFncGirar_CompleteRotationIfDone(void* self, void* /*edx*/, int /*a2*/)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x08);
    uint8_t* ownerChild = CSF_AT(uint8_t*, owner, 0x14);
    void* rotationState = CSF_AT(void*, ownerChild, 0x44);

    uint8_t isRotationDone =
        ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, rotationState, 0)[0x90 / 4])(rotationState, nullptr);

    if (isRotationDone) {
        TFncGirar_OnRotationComplete(self, nullptr, 0);
    }
}

HOOK(0x004D7640, TFncGirar_CompleteRotationIfDone, "auto");
