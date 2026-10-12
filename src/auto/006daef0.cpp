#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, RwMatrixRotate, 0x006248B0, (uint32_t* a1, float* a2, float a3, int a4));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, S_006d24a0_ReleaseLightResources, 0x006DA9A0, (void* self, void* edx));

static char __fastcall S_006d24a0_InitializeLightState(
    S_006d24a0* self, void* /*edx*/)
{
    if (self->field_0000)
    {
        S_006d24a0_ReleaseLightResources(self, nullptr);
        if (self->field_0040)
        {
            RwMatrixDestroy(
                reinterpret_cast<std::uint32_t*>(self->field_0040));
            self->field_0040 = 0;
        }
        self->field_0000 = 0;
    }

    self->field_0108 = 0;
    self->field_00d0 = 1;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_00c4 = 0;
    self->field_0034 = 0;
    self->field_010c = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0014 = 0;

    std::uint32_t* matrix = RwMatrixCreate();

    alignas(8) std::uint8_t rotationBytes[0x18];
    float* rotation = reinterpret_cast<float*>(rotationBytes);
    rotation[4] = 0.0f;
    rotation[1] = 0.0f;
    rotation[3] = 1.0f;
    rotation[0] = 1.0f;
    rotation[5] = 0.0f;
    self->field_0040 = reinterpret_cast<std::uint32_t>(matrix);
    rotation[2] = 0.0f;

    RwMatrixRotate(matrix, rotation, 90.0f, 0);

    self->field_00cc = 0;
    self->field_00c8 = 0;
    self->field_0000 = 1;
    return 1;
}

HOOK(0x006DAEF0, S_006d24a0_InitializeLightState, "auto");
