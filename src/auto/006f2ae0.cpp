#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00860a30_InitBase, 0x006CA660, (C_00860a30* self, void*));

CSF_GLOBAL(uint32_t, g_DefaultParamA_8BB42C, 0x008BB42C);
CSF_GLOBAL(uint32_t, g_BlackClearColor, 0x008BB430);

static uint32_t* __fastcall C_00862598_Construct(C_00862598* self, void* /*edx*/)
{
  C_00860a30_InitBase(self, nullptr);

  self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862518)));
  self->vtbl = CSF_VTBL(0x00862598);
  self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0086258C)));
  self->field_0008 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862570)));
  self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x00862544)));

  self->field_0030 = 0;
  self->field_0034 = 0.0f;
  self->field_0038 = 0.0f;
  self->field_0050 = 0;
  self->field_004c = 0xFFFFFFFFu;
  self->field_002c = 0;
  self->field_0024 = 0xFFFFFFFFu;
  self->field_0028 = 0xFFFFFFFFu;
  self->field_0040 = g_BlackClearColor;
  self->field_0044 = 0;
  self->field_003c = g_DefaultParamA_8BB42C;
  self->field_0048 = 1;

  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x006F2AE0, C_00862598_Construct, "auto");
