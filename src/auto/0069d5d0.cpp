#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));

static char __fastcall C_0085ea2c_CloseSceneFileIfOpen(C_0085ea2c* self, void* /*edx*/)
{
    if (self->field_0008 && self->field_0004)
        CloseSceneFile(reinterpret_cast<void*>(self->field_0004));
    return 1;
}

HOOK(0x0069D5D0, C_0085ea2c_CloseSceneFileIfOpen, "auto");
