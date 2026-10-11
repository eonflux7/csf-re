#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static void __fastcall C_008670f0_SetPosition(
    C_008670f0* self, void* /*edx*/, float* position)
{
    void* child = reinterpret_cast<void*>(self->field_0014);
    if (child &&
        (self->field_00c0 != position[0] ||
         self->field_00c4 != position[1]))
    {
        self->field_00c0 = position[0];
        self->field_00c4 = position[1];
        self->field_00d8 = 1;

        reinterpret_cast<void (__fastcall*)(void*, void*, float*)>(
            CSF_AT(void**, child, 0)[0x28 / 4])(
                child, nullptr, position);
    }
}

HOOK(0x00733230, C_008670f0_SetPosition, "auto");
