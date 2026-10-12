#include <cstdint>
#include "globals.h"
#include "hook.h"

// Module-level globals referenced by the original (raw dword copies).
CSF_GLOBAL(uint32_t, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(uint32_t, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(uint32_t, g_ZeroVectorZ, 0x008BB428);
CSF_GLOBAL(uint32_t, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(uint32_t, g_AxisX_Y, 0x008A7D70);
CSF_GLOBAL(uint32_t, g_AxisX_Z, 0x008A7D74);

// __thiscall, no stack arguments: ret 0.
static uint32_t* __fastcall InitZeroTransformWithAxes(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x08) = 0;
    CSF_AT(uint32_t, self, 0x0C) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x00) = 0;
    CSF_AT(uint32_t, self, 0x04) = 0x43480000u;
    CSF_AT(uint32_t, self, 0x20) = 0x48F42400u;

    CSF_AT(uint32_t, self, 0x08) = g_ZeroVectorX;
    CSF_AT(uint32_t, self, 0x0C) = g_ZeroVectorY;
    CSF_AT(uint32_t, self, 0x10) = g_ZeroVectorZ;
    CSF_AT(uint32_t, self, 0x14) = g_AxisX;
    CSF_AT(uint32_t, self, 0x18) = g_AxisX_Y;
    CSF_AT(uint32_t, self, 0x1C) = g_AxisX_Z;

    return static_cast<uint32_t*>(self);
}

HOOK(0x0040ED30, InitZeroTransformWithAxes, "auto");
