#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, C_00863dd0_Initialize, 0x00712920, (C_00863dd0* self, void* edx));

// Virtual slot 0x84 on C_00863ba8 (no typed wrapper): returns a handle for the (a2, a3) pair.
// Stack args: (a2, a3, 0, 0); thiscall via vtable.
static int __fastcall C_00863ba8_QueryEffectHandle(C_00863ba8* self, void* /*edx*/, int a2, int a3) {
    auto slot84 = reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, self, 0)[0x84 / 4]);
    return slot84(self, nullptr, a2, a3, 0, 0);
}

static int __fastcall C_00863ba8_QueryAndCreateEffectObject(C_00863ba8* self, void* /*edx*/, int a2, int a3) {
    int effectHandle = C_00863ba8_QueryEffectHandle(self, nullptr, a2, a3);

    C_00863dd0* effect = reinterpret_cast<C_00863dd0*>(CoreMemory_AllocUntagged(16));
    float* effectObject = nullptr;
    if (effect) {
        effectObject = C_00863dd0_Initialize(effect, nullptr);
    }

    // Slot 1 of the new object's vtable (+0x4): init with (handle, a2, a3).
    auto init = reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int)>(
        CSF_AT(void**, effectObject, 0)[1]);
    init(effectObject, nullptr, effectHandle, a2, a3);

    return reinterpret_cast<int>(effectObject);
}

HOOK(0x0070F5A0, C_00863ba8_QueryAndCreateEffectObject, "auto");
