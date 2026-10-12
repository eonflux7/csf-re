#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Buffer initializer at the sub-object pad_000c / pad_001c (8 fastcall stack args, callee pops 32).
CSF_FUNC(int, __fastcall, BufferSlot_Init, 0x0040C1F0,
         (void* self, void* edx, unsigned int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9));

static uint32_t* __fastcall C_0082daf4_ConstructEntry(C_0082daf4* self, void* /*edx*/, int a2, int a3)
{
  *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082DAF4);

  self->field_0010 = 0;
  self->field_0014 = 0;
  self->field_0018 = 0;
  self->field_0020 = 0;
  self->field_0024 = 0;
  self->field_0028 = 0;

  self->field_0004 = static_cast<uint32_t>(a2);
  self->field_0008 = static_cast<uint32_t>(a3);

  BufferSlot_Init(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr, 8u, 0, 0, 0, 0, 0, 0, 10);
  BufferSlot_Init(reinterpret_cast<uint8_t*>(self) + 0x1C, nullptr, 8u, 0, 0, 0, 0, 0, 0, 10);

  return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0040C330, C_0082daf4_ConstructEntry, "auto");
