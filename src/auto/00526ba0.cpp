#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) char __fastcall AudioSoundManager_ForwardCompletionQuery(
    void* delegateObject, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x48]
    }
}

static char __fastcall C_008411a0_IsComplete(
    C_008411a0* self, void* /*edx*/)
{
    if (!self->field_003c)
        return 1;

    if (!self->field_003a)
        return 0;

    const uint32_t delegateObject = self->field_0060;
    if (delegateObject)
        return AudioSoundManager_ForwardCompletionQuery(
            reinterpret_cast<void*>(delegateObject), nullptr);

    return self->field_0048 >= self->field_0050;
}

HOOK(0x00526BA0, C_008411a0_IsComplete, "auto");
