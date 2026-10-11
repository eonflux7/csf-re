#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: TFncGirar::OnRotationComplete(int) (ret 4, __thiscall)
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

// Enables the rotation on the owner's linked object (virtual slot 0xB4 with argument 1),
// then runs the rotation-complete handler with argument 0.
static void __fastcall TFncGirar_EnableRotatorAndFinish(TFncGirar* self, void* /*edx*/)
{
    // self->owner (+0x08)
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x08)));

    // owner->rotator (+0x14), an object whose first dword is its vtable
    void* rotator = CSF_AT(void*, owner, 0x14);

    // rotator->vtbl[0xB4 / 4](rotator, 1)
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, rotator, 0)[0xB4 / 4])(rotator, nullptr, 1);

    TFncGirar_OnRotationComplete(self, nullptr, 0);
}

HOOK(0x004D7570, TFncGirar_EnableRotatorAndFinish, "auto");
