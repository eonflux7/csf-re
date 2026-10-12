#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, C_00869cd0_Construct, 0x007562D0, (char* self, void*));

static uint32_t* __fastcall C_00840de4_ConstructMenuEffects(C_00840de4* self, void* /*edx*/)
{
    C_00869cd0_Construct(reinterpret_cast<char*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00840DE4);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0051B600, C_00840de4_ConstructMenuEffects, "auto");
