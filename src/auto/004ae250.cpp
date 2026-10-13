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

// Constructs TMovilAPuntoParapetandose: base TAiState ctor, then the object's own vtable is stored,
// then four embedded TAiState sub-objects at +0x04, +0x08, +0x0C, +0x14 are each constructed and given
// their own vtable (the one at +0x14 is stored after its constructor call). The three state fields
// at +0x18, +0x1C, +0x20 are zeroed. Returns the object pointer.
static uint32_t* __fastcall TMovilAPuntoParapetandose_Construct(TMovilAPuntoParapetandose* self, void* /*edx*/)
{
    TAiState_Constructor(self, nullptr);
    self->vtbl = CSF_VTBL(0x008396B8);

    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x04), nullptr);
    CSF_AT(void*, self, 0x04) = static_cast<void*>(CSF_VTBL(0x00836BE0));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x08), nullptr);
    CSF_AT(void*, self, 0x08) = static_cast<void*>(CSF_VTBL(0x00836C70));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x0C), nullptr);
    CSF_AT(void*, self, 0x0C) = static_cast<void*>(CSF_VTBL(0x00836CF8));

    TAiState_Constructor(reinterpret_cast<TAiState*>(base + 0x14), nullptr);
    CSF_AT(void*, self, 0x14) = static_cast<void*>(CSF_VTBL(0x00836D88));

    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004AE250, TMovilAPuntoParapetandose_Construct, "auto");
