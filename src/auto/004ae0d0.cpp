#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// TAiState base constructor (__fastcall, ecx=this, edx unused, ret 0)
CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

// Constructs a TMovilAPuntoRastreo: base TAiState, then five embedded
// TAiState-derived sub-objects at +0x04, +0x08, +0x0C, +0x10, +0x14,
// then zeroes the three tracking fields at +0x18, +0x1C, +0x20.
static TMovilAPuntoRastreo* __fastcall TMovilAPuntoRastreo_Construct(TMovilAPuntoRastreo* self, void* /*edx*/)
{
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x008392C0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x00835E20);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    CSF_AT(void*, self, 0x08) = CSF_VTBL(0x00835EA8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x0C), nullptr);
    CSF_AT(void*, self, 0x0C) = CSF_VTBL(0x00835F30);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);
    CSF_AT(void*, self, 0x10) = CSF_VTBL(0x00835FB8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x14), nullptr);
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x00836048);

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    return self;
}

HOOK(0x004AE0D0, TMovilAPuntoRastreo_Construct, "auto");
