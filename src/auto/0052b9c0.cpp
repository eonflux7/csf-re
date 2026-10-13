#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __fastcall, CMenuMessageBox_ResetControlsAndReleaseRaster, 0x00528980, (CMenuMessageBox* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0085e7d0_Construct, 0x0069AE70, (C_0085e7d0* self, void*));
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

static int __fastcall CMenuMessageBox_Construct(
    CMenuMessageBox* self, void* /*edx*/, int mode)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083042C);
    C_00830438_Construct(bytes + 0x04, nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008304E0);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00830488));
    C_0085e7d0_Construct(
        reinterpret_cast<C_0085e7d0*>(bytes + 0x10), nullptr);

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00841320));
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00841370);
    CSF_AT(uint32_t, self, 0x10) =
        reinterpret_cast<uint32_t>(CSF_VTBL(0x00841318));

    MenuBuffer_Init(bytes + 0x8A0, nullptr);

    self->field_0954 = 0;
    self->field_0958 = 0;
    self->field_095c = 0;
    self->field_0960 = 10;
    self->field_0968 = 0;
    self->field_096c = 0;
    self->field_0978 = 0;
    self->field_097c = 0;
    self->field_0980 = 0;
    self->field_0984 = 0;
    self->field_0988 = 0;
    self->field_098c = 0;
    self->field_0990 = 0;
    self->field_0994 = 0;
    self->field_0898 = mode;
    self->field_0894 = 0;
    self->field_0090 = 0;
    self->field_007c = 0;
    self->field_0964 = 0;
    self->field_0060 = 0;
    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_005c = 0;
    self->field_006c = 0;
    self->field_0070 = 0;
    self->field_0074 = 0;
    self->field_09a0 = 0;
    self->field_08a4 = 0;
    CSF_AT(uint32_t, self, 0x8A0) = 1;
    self->field_08a8 = 0;
    self->field_09ac = 0;
    self->m_raster = 0;

    CMenuMessageBox_ResetControlsAndReleaseRaster(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x0052B9C0, CMenuMessageBox_Construct, "auto");
