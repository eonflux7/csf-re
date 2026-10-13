#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

static uint32_t* __fastcall TFncBusqueda_Construct(TFncBusqueda* self, void* /*edx*/) {
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00839878);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x4), nullptr);
    CSF_AT(uintptr_t, self, 0x4) = CSF_ADDR_DATA(0x00837518);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x8), nullptr);
    CSF_AT(uintptr_t, self, 0x8) = CSF_ADDR_DATA(0x008375A0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0xC), nullptr);
    CSF_AT(uintptr_t, self, 0xC) = CSF_ADDR_DATA(0x00837628);

    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004AE3D0, TFncBusqueda_Construct, "auto");
