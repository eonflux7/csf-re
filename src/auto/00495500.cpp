#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, HudAlertIcons_ApplyRecoil, 0x004A1540, (void* self, void* , float* recoil));

// Original: push ebp / mov ebp,esp / add ecx,0x344 / pop ebp / jmp HudAlertIcons_ApplyRecoil
// Tail jump: the float argument stays in its stack slot and is popped by the callee.
static int __fastcall WeaponFirearms_ApplyRecoil(void* self, void* /*edx*/, float a1)
{
    void* hudSelf = reinterpret_cast<uint8_t*>(self) + 0x344;

    // The forwarded stack slot holds the raw dword of a1; pass those same bits as the pointer argument.
    uint32_t argBits;
    std::memcpy(&argBits, &a1, sizeof(argBits));
    float* recoilArg = reinterpret_cast<float*>(static_cast<uintptr_t>(argBits));

    return HudAlertIcons_ApplyRecoil(hudSelf, nullptr, recoilArg);
}

HOOK(0x495500, WeaponFirearms_ApplyRecoil, "auto");
