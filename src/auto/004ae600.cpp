#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

static uint32_t* __fastcall TFncAtacarDesdeParapeto_Construct(
    TFncAtacarDesdeParapeto* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00839EA8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x04), nullptr);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00837BA0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x08), nullptr);
    CSF_AT(void**, self, 0x08) = CSF_VTBL(0x00837C38);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x0C), nullptr);
    CSF_AT(void**, self, 0x0C) = CSF_VTBL(0x00837CC0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x10), nullptr);
    CSF_AT(void**, self, 0x10) = CSF_VTBL(0x00837D48);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x14), nullptr);
    CSF_AT(void**, self, 0x14) = CSF_VTBL(0x00837DD0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x18), nullptr);
    CSF_AT(void**, self, 0x18) = CSF_VTBL(0x00837E68);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x1C), nullptr);
    CSF_AT(void**, self, 0x1C) = CSF_VTBL(0x00837EF0);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x20), nullptr);
    CSF_AT(void**, self, 0x20) = CSF_VTBL(0x00837F78);

    TAiState_Constructor(reinterpret_cast<TAiState*>(bytes + 0x24), nullptr);
    CSF_AT(void**, self, 0x24) = CSF_VTBL(0x00838000);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004AE600, TFncAtacarDesdeParapeto_Construct, "auto");
