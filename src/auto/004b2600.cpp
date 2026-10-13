#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));
CSF_FUNC(uint32_t*, __fastcall, TMovilAPunto_2_Construct, 0x004AE070, (TMovilAPunto_2* self, void*));
CSF_FUNC(TMovilAPuntoRastreo*, __fastcall, TMovilAPuntoRastreo_Construct, 0x004AE0D0, (TMovilAPuntoRastreo* self, void*));
CSF_FUNC(TMovilAParapeto*, __fastcall, TMovilAParapeto_Constructor, 0x004AE150, (TMovilAParapeto* self, void*));
CSF_FUNC(uint32_t*, __fastcall, TMovilAInteractivo_Construct, 0x004AE1B0, (TMovilAInteractivo* self, void*));

// Constructs a TAlerta (alert AI state) in place: base TAiState at +0x00,
// three secondary TAiState-derived slots, and the embedded movement/interaction
// sub-objects at +0x18, +0x38, +0x5C and +0x70.
static TAlerta* __fastcall TAlerta_Construct(TAlerta* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    // Primary base and vtable.
    TAiState_Constructor(reinterpret_cast<TAiState*>(base), nullptr);
    self->vtbl = CSF_VTBL(0x0083A368);

    // Slot at +0x04.
    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x04), nullptr);
    *reinterpret_cast<void**>(base + 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008391B0));

    // Slot at +0x08.
    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x08), nullptr);
    *reinterpret_cast<void**>(base + 0x08) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00835A58));

    // Slot at +0x0C.
    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x0C), nullptr);
    *reinterpret_cast<void**>(base + 0x0C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00835AE0));

    // Slot at +0x14.
    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x14), nullptr);
    *reinterpret_cast<void**>(base + 0x14) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00835B68));

    // Embedded sub-objects.
    TMovilAPunto_2_Construct(reinterpret_cast<TMovilAPunto_2*>(base + 0x18), nullptr);
    TMovilAPuntoRastreo_Construct(reinterpret_cast<TMovilAPuntoRastreo*>(base + 0x38), nullptr);
    TMovilAParapeto_Constructor(reinterpret_cast<TMovilAParapeto*>(base + 0x5C), nullptr);
    TMovilAInteractivo_Construct(reinterpret_cast<TMovilAInteractivo*>(base + 0x70), nullptr);

    return self;
}

HOOK(0x004B2600, TAlerta_Construct, "auto");
