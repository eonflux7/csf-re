#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));

static int __fastcall C_0082f9f8_StartMovement(C_0082f9f8* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    return CharTask_StartMovement(self, nullptr, 0);
}

HOOK(0x0047B670, C_0082f9f8_StartMovement, "auto");
