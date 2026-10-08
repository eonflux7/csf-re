#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CMenuMpSessions_FinishInitialization(
    CMenuMpSessions* self, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [ecx]
        mov dword ptr [ecx + 0x18], 5
        mov dword ptr [ecx + 0x1cc], edx
        mov dword ptr [ecx + 0x1d4], edx
        jmp dword ptr [eax + 0x40]
    }
}

static int __fastcall CMenuMpSessions_InitializeState(
    CMenuMpSessions* self, void* /*edx*/)
{
    self->field_00b7 = 0;
    self->field_004c = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_006c = 0;
    self->field_005c = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_00a4 = 0;
    self->field_00b0 = 0;
    self->field_00b2 = 0;
    self->field_00b3 = 0;
    self->field_00b4 = 0;
    self->field_00b5 = 0;
    self->field_00b6 = 0;
    self->field_01b8 = 0;
    self->field_01bc = 0;
    self->field_01c0 = 0;
    self->field_001c = 0;
    self->field_01c4 = 0;
    self->field_01c8 = 0;
    self->field_01d0 = 0;
    self->field_01d8 = 0;
    self->field_01dc = 0;
    self->field_0208 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_01fc = 0;
    self->field_0014 = 0;
    self->field_0200 = 0;
    self->field_01f4 = 0;
    self->field_01f8 = 0;
    self->field_020c = 0.0f;

    [[clang::musttail]] return CMenuMpSessions_FinishInitialization(
        self, reinterpret_cast<void*>(UINT32_MAX));
}

HOOK(0x00560140, CMenuMpSessions_InitializeState, "auto");
