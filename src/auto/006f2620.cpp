#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* , int));

static int __fastcall C_00862598_ResetPositionToZero(C_00862598* self, void* /*edx*/, int a2)
{
    C_00860a30_ClearField000c(self, nullptr, a2);
    self->field_0044 = 0;
    *reinterpret_cast<float*>(&self->field_0030) = g_ZeroVectorX;

    const float zeroY = g_ZeroVectorY;
    CSF_AT(float, self, 0x34) = zeroY;

    self->field_0048 = 1;
    CSF_AT(float, self, 0x38) = g_ZeroVectorZ;

    int result;
    std::memcpy(&result, &zeroY, sizeof(result));
    return result;
}

HOOK(0x006F2620, C_00862598_ResetPositionToZero, "auto");
