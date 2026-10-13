#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e090_Initialize, 0x00416530, (C_0082e090* self, void*));

static int __fastcall C_0082e128_Initialize(C_0082e128* self, void* /*edx*/)
{
    C_0082e090_Initialize(reinterpret_cast<C_0082e090*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0082E128);

    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    self->field_00d8 = 0;
    self->field_00dc = 0;
    self->field_00e0 = 0;
    self->field_00f0 = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00e4 = 0;
    self->field_00e8 = 0;
    self->field_00ec = 0;
    self->field_00f4 = 0;
    self->field_00f8 = 0;
    self->field_00f0 = 1065353216;
    self->field_010c = 1;
    self->field_011c = 0;
    self->field_0120 = 0;
    self->field_0124 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_0110 = 0;
    self->field_0114 = 0;
    self->field_0118 = 0;
    self->field_0119 = 0;

    self->field_0130 = 0;
    self->field_0134 = 0;
    self->field_0138 = 0;
    self->field_013c = 0;
    self->field_0140 = 0;
    self->field_0144 = 0;
    self->field_0154 = 0;
    self->field_0158 = 0;
    self->field_015c = 0;
    self->field_0148 = 0;
    self->field_014c = 0;
    self->field_0150 = 0;
    self->field_0154 = 1065353216;
    self->field_0158 = 0;
    self->field_015c = 0;
    self->field_0170 = 1;
    self->field_0180 = 0;
    self->field_0184 = 0;
    self->field_0188 = 0;
    self->field_0128 = 0;
    self->field_012c = 0;
    self->field_0174 = 0;
    self->field_0178 = 0;
    self->field_017c = 0;
    self->field_017d = 0;

    self->field_0194 = 0;
    self->field_0198 = 0;
    self->field_019c = 0;
    self->field_01a0 = 0;
    self->field_01a4 = 0;
    self->field_01a8 = 0;
    self->field_01b8 = 0;
    self->field_01bc = 0;
    self->field_01c0 = 0;
    self->field_01ac = 0;
    self->field_01b0 = 0;
    self->field_01b4 = 0;
    self->field_01b8 = 1065353216;
    self->field_01bc = 0;
    self->field_01c0 = 0;
    self->field_01d4 = 1;
    self->field_01e4 = 0;
    self->field_01e8 = 0;
    self->field_01ec = 0;
    self->field_018c = 0;
    self->field_0190 = 0;
    self->field_01d8 = 0;
    self->field_01dc = 0;
    self->field_01e0 = 0;
    self->field_01e1 = 0;

    self->field_0200 = 0;
    self->field_0204 = 0;
    self->field_0208 = 0;
    self->field_0200 = 0;
    self->field_01f2 = 0;
    self->field_01f3 = 0;
    self->field_01f0 = 0;
    self->field_01f1 = 0;
    self->field_0204 = 0;
    self->field_01fc = 0;
    self->field_0210 = 0;
    self->field_01f4 = 0;
    self->field_01f8 = 0.0f;
    self->field_00c0 = 0;
    self->field_00bc = 1045220557;
    self->field_020c = -1.0f;
    self->field_0208 = 0;

    return reinterpret_cast<int>(self);
}

HOOK(0x00417F00, C_0082e128_Initialize, "auto");
