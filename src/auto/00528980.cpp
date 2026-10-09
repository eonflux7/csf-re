#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));

static __declspec(naked) int __fastcall CMenuMessageBox_TailResetState(
    CMenuMessageBox* self, void* /*edx*/)
{
    __asm
    {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x48]
    }
}

static int __fastcall CMenuMessageBox_ResetControlsAndReleaseRaster(
    CMenuMessageBox* self, void* /*edx*/)
{
    const uint32_t raster = self->field_09b0;

    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->m_staticControl = 0;
    self->field_0890 = 0;

    if (raster)
    {
        RwRasterDestroy(raster);
        self->field_09b0 = 0;
    }

    [[clang::musttail]] return CMenuMessageBox_TailResetState(self, nullptr);
}

HOOK(0x00528980, CMenuMessageBox_ResetControlsAndReleaseRaster, "auto");
