#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_487400, 0x00487400, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));

static char __fastcall CCharActionFsm_TryBeginActionState(CCharActionFsm* self, void* /*edx*/)
{
    char result = sub_487400(
        self, nullptr, reinterpret_cast<uint32_t*>(self->field_008c));
    if (result)
    {
        sub_498F50(reinterpret_cast<void*>(self->field_0008), nullptr, 0x40000, 1);
        CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x2E4) |= 0x100u;
        self->field_00a4 = self->field_008c;
        return static_cast<char>(self->vBeginWeaponStateChange(12, 0));
    }
    return result;
}

HOOK(0x00487650, CCharActionFsm_TryBeginActionState, "auto");
