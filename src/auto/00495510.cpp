#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: S_004a1210 toggle-timer check (__fastcall, no stack args).
CSF_FUNC(int, __fastcall, S_004a1210_IsToggleTimerActive, 0x004A1210, (S_004a1210* self, void*));

// Vtable of the S_004a1210 object reached by the tail jump. No code here reads or writes it.
[[maybe_unused]] static void* const kToggleTimerVtable = CSF_VTBL(0x004A1210);

// Original: add ecx, 0x344 ; jmp S_004a1210_IsToggleTimerActive
// Tail-forwards to the toggle-timer check on the sub-object at this+0x344.
static int __fastcall SuppressesDamageRecoil(void* self, void* /*edx*/)
{
    auto* toggleTimer = reinterpret_cast<S_004a1210*>(static_cast<uint8_t*>(self) + 0x344);
    return S_004a1210_IsToggleTimerActive(toggleTimer, nullptr);
}

HOOK(0x495510, SuppressesDamageRecoil, "auto");
