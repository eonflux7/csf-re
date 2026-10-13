#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CInputBase_Initialize, 0x00712D30,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, InputDinput_InitializeEmbeddedState, 0x00712EB0,
         (void* self, void* edx));

static int __fastcall CInputDirectInput_Initialize(
    CInputDirectInput* self, void* /*edx*/)
{
    CInputBase_Initialize(self, nullptr);
    self->vtbl = CSF_VTBL(0x00863ED8);

    for (int index = 0; index < 64; ++index) {
        CSF_AT(uint32_t, self, 0x2C + index * 8) = 0;
        CSF_AT(uint8_t, self, 0x30 + index * 8) = 0;
    }

    self->field_0028 = 0;
    InputDinput_InitializeEmbeddedState(&self->field_022c, nullptr);

    std::memset(&CSF_AT(uint8_t, self, 0xB30), 0, 0x100u);
    self->field_0c30 = 0;
    self->field_0c34 = 0;
    self->field_0c38 = 0;
    self->field_0c3c = 0.0f;
    self->field_0c40 = 0.0f;
    self->field_0c44 = 0;
    self->field_0c48 = 0;
    self->field_0c4c = 0;
    self->field_0c4d = 0;
    self->field_0c4e = 0;
    self->field_0c4f = 0;
    self->field_0cb8 = 0;
    self->field_0cbc = 0;
    self->field_0cc0 = 0;
    self->field_0cb4 = 6;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0c91 = 0;
    self->field_0c90 = 0;
    self->field_0c98 = 1065353216;
    self->field_0c9c = 1065353216;
    self->field_0c94 = 1045220557;
    self->field_0ca0 = 500;
    self->field_0ca4 = 0;
    CSF_AT(uint32_t, self, 0xCA8) = 0;
    CSF_AT(uint32_t, self, 0xCAC) = 0;
    CSF_AT(uint32_t, self, 0xCB0) = 0;
    std::memset(&CSF_AT(uint8_t, self, 0xC50), 0, 0x40);

    return static_cast<int>(reinterpret_cast<std::intptr_t>(self));
}

HOOK(0x00713160, CInputDirectInput_Initialize, "auto");
