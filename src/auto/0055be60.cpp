#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// char __thiscall sub_55BD30(std::uint8_t *this) - base class flag activation, tail-called by the target.
CSF_FUNC(char, __fastcall, C_00845368_ActivateFlag, 0x55BD30, (C_00845368* self, void* edx));

// 0x0055BE60: clears the object's flag/pointer members, runs its virtual reset slot (vtbl+8)
// and then tail-jumps into the base class ActivateFlag, forwarding its return value.
static char __fastcall C_008453a4_ResetAndActivate(C_008453a4* self, void* /*edx*/)
{
    // virtual call through vtable slot at byte offset 8 (slot 2), __thiscall, no args
    void** vtbl = CSF_AT(void**, self, 0);
    ((void(__fastcall*)(C_008453a4*, void*))vtbl[2])(self, nullptr);

    self->field_0018 = 0;   // byte ptr [esi+0x18] = 0
    self->field_0010 = 0;   // dword ptr [esi+0x10] = 0

    return C_00845368_ActivateFlag(reinterpret_cast<C_00845368*>(self), nullptr);
}

HOOK(0x55BE60, C_008453a4_ResetAndActivate, "auto");
