#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall C_00868af0_Reset(C_00868af0* self, void* /*edx*/)
{
    void* attachedObject = CSF_AT(void*, self, 0x08);
    if (attachedObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attachedObject, 0)[0x18 / 4])(
                attachedObject, nullptr, 0);
    }

    self->field_0024 = 0;
    self->field_0004 = 1;
    return 1;
}

HOOK(0x007520A0, C_00868af0_Reset, "auto");
