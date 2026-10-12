#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __fastcall, C_008629a4_Destroy, 0x006F8FF0, (C_008629a4* self, void*));
CSF_FUNC(int, __fastcall, C_00862c74_CleanupEffectState, 0x006F9D90, (void* self, void* edx));

static void __fastcall C_00862c74_Cleanup(C_00862c74* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00862C74);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_ADDR_DATA(0x00862C58));
    C_00862c74_CleanupEffectState(self, nullptr);

    uint32_t block = self->field_00b8;
    if (block)
        Mem_FreeDebug(reinterpret_cast<void*>(block));
    self->field_00b8 = 0;
    self->field_00bc = 0;
    self->field_00c0 = 0;

    block = self->field_00a8;
    if (block)
        Mem_FreeDebug(reinterpret_cast<void*>(block));
    self->field_00a8 = 0;
    self->field_00ac = 0;
    self->field_00b0 = 0;

    block = self->field_0098;
    if (block)
        Mem_FreeDebug(reinterpret_cast<void*>(block));
    self->field_0098 = 0;
    self->field_009c = 0;
    self->field_00a0 = 0;

    block = self->field_0088;
    if (block)
        Mem_FreeDebug(reinterpret_cast<void*>(block));
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;

    C_008629a4_Destroy(reinterpret_cast<C_008629a4*>(self), nullptr);
}

HOOK(0x006FB4C0, C_00862c74_Cleanup, "auto");
