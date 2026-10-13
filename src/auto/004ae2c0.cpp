#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

static uint32_t* __fastcall TMovilAInteractivo_2_Construct(TMovilAInteractivo_2* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(base), nullptr);
    *(void***)base = CSF_VTBL(0x00839758);

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x04), nullptr);
    CSF_AT(uint32_t, base, 0x04) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836E10));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x08), nullptr);
    CSF_AT(uint32_t, base, 0x08) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836E98));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x0C), nullptr);
    CSF_AT(uint32_t, base, 0x0C) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836F20));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x10), nullptr);
    CSF_AT(uint32_t, base, 0x10) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836FA8));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x14), nullptr);
    CSF_AT(uint32_t, base, 0x14) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00837030));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004AE2C0, TMovilAInteractivo_2_Construct, "auto");
