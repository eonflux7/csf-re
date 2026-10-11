#include <cstdint>
#include <cstddef>
#include <cstring>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, CGuiControl_GetColorAnim, 0x004FE4B0, (CGuiControl* self, void*));
CSF_FUNC(int, __fastcall, ApplyColorAnimationToControl, 0x00445050, (uint32_t* self, void*, C_00833908* animation, uint32_t* entry));

// Builds an 11-dword color-animation entry on the stack: vtable in dword 0,
// 0xFF bytes in dwords 1..3, zeroed dwords 4..10. Then applies the global
// control's color animation to the handle stored in this->field_000c.
static int __fastcall C_0083e160_ApplyColorAnimFromGlobal(C_0083e160* self, void* /*edx*/) {
    uint32_t animationHandle = self->field_000c;

    alignas(8) uint32_t entry[11];
    CSF_AT(void*, entry, 0) = CSF_VTBL(0x0082D448);
    std::memset(&entry[1], 255, 12);
    std::memset(&entry[4], 0, 28);

    CGuiControl* globalControl =
        reinterpret_cast<CGuiControl*>(static_cast<uintptr_t>(GetGlobalValue_8B0568()));
    int colorAnimControl = CGuiControl_GetColorAnim(globalControl, nullptr);

    return ApplyColorAnimationToControl(
        reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(colorAnimControl)),
        nullptr,
        reinterpret_cast<C_00833908*>(static_cast<uintptr_t>(animationHandle)),
        entry);
}

HOOK(0x4E1320, C_0083e160_ApplyColorAnimFromGlobal, "auto");
