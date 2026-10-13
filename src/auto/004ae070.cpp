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

// Constructs TMovilAPunto_2: base TAiState ctor, then four embedded TAiState
// sub-objects at +0x04, +0x08, +0x0C, +0x10 (each constructed, then given its own vtable),
// then the three state fields at +0x14, +0x18, +0x1C are zeroed.
static uint32_t* __fastcall TMovilAPunto_2_Construct(TMovilAPunto_2* self, void* /*edx*/)
{
    TAiState_Constructor(self, nullptr);
    self->vtbl = CSF_VTBL(0x00839238);

    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x04), nullptr);
    CSF_AT(void*, self, 0x04) = static_cast<void*>(CSF_VTBL(0x00835C00));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x08), nullptr);
    CSF_AT(void*, self, 0x08) = static_cast<void*>(CSF_VTBL(0x00835C88));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x0C), nullptr);
    CSF_AT(void*, self, 0x0C) = static_cast<void*>(CSF_VTBL(0x00835D10));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x10), nullptr);
    CSF_AT(void*, self, 0x10) = static_cast<void*>(CSF_VTBL(0x00835D98));

    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004AE070, TMovilAPunto_2_Construct, "auto");
