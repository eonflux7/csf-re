#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CMenuGameOptions_TailDispatch(
    CMenuGameOptions* self, void* edx)
{
    __asm
    {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x40]
    }
}

static int __fastcall CMenuGameOptions_ResetOptionFields(
    CMenuGameOptions* self, void* edx)
{
    self->field_0018 = 0;
    self->field_0050 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;

    [[clang::musttail]] return CMenuGameOptions_TailDispatch(self, edx);
}

HOOK(0x0053AFE0, CMenuGameOptions_ResetOptionFields, "auto");
