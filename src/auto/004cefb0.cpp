#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (void* self, void* edx, int a2));

// Constructor of C_0083b118 (derives from C_0083b958 / TCharTask).
// __thiscall with one stack arg, returns this.
static uint32_t* __fastcall C_0083b118_Construct(C_0083b118* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(self, nullptr, a2);

    *(void***)self = CSF_VTBL(0x0083B118);
    CSF_AT(void*, self, 0x0004) = CSF_VTBL(0x0083B0FC);

    CSF_AT(int, self, 0x0048) = 0;
    CSF_AT(int, self, 0x004C) = 0;
    CSF_AT(int, self, 0x0050) = 0;
    CSF_AT(int, self, 0x0040) = 0;
    CSF_AT(float, self, 0x0044) = 200.0f;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4CEFB0, C_0083b118_Construct, "auto");
