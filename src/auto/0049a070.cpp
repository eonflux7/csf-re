#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00833ee8_ClearBuffers(C_00833ee8* self, void* /*edx*/)
{
    self->field_02e4 &= 0xFFF1F804u;

    uint32_t firstBuffer = self->field_0300;
    if (firstBuffer)
        Mem_FreeDebug(reinterpret_cast<void*>(firstBuffer));
    self->field_0300 = 0;
    self->field_0304 = 0;
    self->field_0308 = 0;

    int result = static_cast<int>(self->field_0310);
    if (result)
        result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0310));
    self->field_0310 = 0;
    self->field_0314 = 0;
    self->field_0318 = 0;
    return result;
}

HOOK(0x0049A070, C_00833ee8_ClearBuffers, "auto");
