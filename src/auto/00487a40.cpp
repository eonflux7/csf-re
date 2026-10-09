#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, sub_486B40, 0x00486B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_481790, 0x00481790, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Releases the action-FSM resources and resets its state, then tail-jumps into the base cleanup.
static int __fastcall CCharActionFsm_ReleaseActionResources(CCharActionFsm* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x008334F0);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008334D0)));
    sub_486B40(self, nullptr);

    uint8_t* actionBlock = reinterpret_cast<uint8_t*>(self->field_00d4);
    if (actionBlock)
    {
        CSF_AT(void*, actionBlock, 0x24) = CSF_VTBL(0x0082D410);
        CSF_AT(void*, actionBlock, 0x10) = CSF_VTBL(0x0082D410);
        Mem_Free(actionBlock);
        self->field_00d4 = 0;
    }

    CSF_AT(void*, self, 0xB0) = CSF_VTBL(0x0082D410);

    if (self->field_0078)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0078));
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0080 = 0;

    return sub_481790(self, nullptr);
}

HOOK(0x00487A40, CCharActionFsm_ReleaseActionResources, "auto");
