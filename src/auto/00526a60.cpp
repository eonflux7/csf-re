#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall AudioSoundManager_TailResetChild(
    void* /*child*/, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x2c]
    }
}

static int __fastcall C_008411a0_ResetState(C_008411a0* self, void* edx)
{
    self->field_0054 = 0;
    self->field_0048 = 0.0f;
    self->field_004c = 0.0f;
    self->field_0050 = 0.0f;

    const uint32_t child = self->field_0060;
    if (child)
        return AudioSoundManager_TailResetChild(
            reinterpret_cast<void*>(child), edx);

    return 0;
}

HOOK(0x526A60, C_008411a0_ResetState, "auto");
