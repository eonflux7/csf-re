#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

static uint32_t* __fastcall TOrden_Construct(TOrden* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0083A660);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x04), nullptr);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x00838120);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x08), nullptr);
    CSF_AT(void*, self, 0x08) = CSF_VTBL(0x008381B0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x0C), nullptr);
    CSF_AT(void*, self, 0x0C) = CSF_VTBL(0x00838240);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x10), nullptr);
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x008382C8);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x14), nullptr);
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x00838358);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x18), nullptr);
    CSF_AT(void*, self, 0x18) = CSF_VTBL(0x008383E0);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x1C), nullptr);
    CSF_AT(void*, self, 0x1C) = CSF_VTBL(0x00838468);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x20), nullptr);
    CSF_AT(void*, self, 0x20) = CSF_VTBL(0x008384F8);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x24), nullptr);
    CSF_AT(void*, self, 0x24) = CSF_VTBL(0x00838588);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x28), nullptr);
    CSF_AT(void*, self, 0x28) = CSF_VTBL(0x00838618);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x2C), nullptr);
    CSF_AT(void*, self, 0x2C) = CSF_VTBL(0x008386A8);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x30), nullptr);
    CSF_AT(void*, self, 0x30) = CSF_VTBL(0x00838730);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x34), nullptr);
    CSF_AT(void*, self, 0x34) = CSF_VTBL(0x008387B8);

    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;
    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x58), nullptr);
    CSF_AT(void*, self, 0x58) = CSF_VTBL(0x00839F48);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x5C), nullptr);
    CSF_AT(void*, self, 0x5C) = CSF_VTBL(0x00838850);
    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x60), nullptr);
    CSF_AT(void*, self, 0x60) = CSF_VTBL(0x008388D8);

    self->field_0064 = 0;
    self->field_0068 = 0;
    self->field_006c = 0;
    self->field_0070 = 10;
    self->field_0080 = 10;
    self->field_0074 = 0;
    self->field_0078 = 0;
    self->field_007c = 0;
    self->field_0090 = 10;
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    CSF_AT(void*, self, 0x94) = CSF_VTBL(0x0082D448);
    self->field_0098 = -1;
    self->field_009c = -1;
    self->field_00a0 = 0;
    self->field_00a4 = 0;
    self->field_00a8 = 0;
    self->field_00b4 = 0;
    self->field_00b8 = 0;
    self->field_00bc = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004B27B0, TOrden_Construct, "auto");
