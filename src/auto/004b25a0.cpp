#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));
CSF_FUNC(uint32_t*, __fastcall, TMovilAPunto_Construct, 0x004AE010, (TMovilAPunto* self, void*));

static uint32_t* __fastcall TDudoso_ConstructSuspiciousState(TDudoso* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x0083A2D8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x04), nullptr);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00839090);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x08), nullptr);
    CSF_AT(void**, self, 0x08) = CSF_VTBL(0x008357A0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x0C), nullptr);
    CSF_AT(void**, self, 0x0C) = CSF_VTBL(0x00835828);

    TMovilAPunto_Construct(reinterpret_cast<TMovilAPunto*>(bytes + 0x14), nullptr);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004B25A0, TDudoso_ConstructSuspiciousState, "auto");
