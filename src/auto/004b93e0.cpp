#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));
CSF_FUNC(uint32_t*, __fastcall, TEstatico_Construct, 0x004B2690, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, TMovilAPuntoParapetandose_Construct, 0x004AE250, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, TMovilAInteractivo_2_Construct, 0x004AE2C0, (void* self, void* edx));

static uint32_t* __fastcall TCombate_Construct(TCombate* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0083A8B8);
    TEstatico_Construct(bytes + 0x04, nullptr);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x24), nullptr);
    CSF_AT(void**, self, 0x24) = CSF_VTBL(0x00839510);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x28), nullptr);
    CSF_AT(void**, self, 0x28) = CSF_VTBL(0x008368A0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x2C), nullptr);
    CSF_AT(void**, self, 0x2C) = CSF_VTBL(0x00836930);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x30), nullptr);
    CSF_AT(void**, self, 0x30) = CSF_VTBL(0x008395A8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x34), nullptr);
    CSF_AT(void**, self, 0x34) = CSF_VTBL(0x008369B8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x38), nullptr);
    CSF_AT(void**, self, 0x38) = CSF_VTBL(0x00836A40);
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;
    CSF_AT(uint32_t, self, 0x44) = 0;

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x48), nullptr);
    CSF_AT(void**, self, 0x48) = CSF_VTBL(0x00839630);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x4C), nullptr);
    CSF_AT(void**, self, 0x4C) = CSF_VTBL(0x00836AD0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x50), nullptr);
    CSF_AT(void**, self, 0x50) = CSF_VTBL(0x00836B58);

    TMovilAPuntoParapetandose_Construct(bytes + 0x54, nullptr);
    TMovilAInteractivo_2_Construct(bytes + 0x78, nullptr);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004B93E0, TCombate_Construct, "auto");
