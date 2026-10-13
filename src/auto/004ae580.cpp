#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class constructor, called once for each embedded TAiState sub-object.
CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

// Constructs a TFncVigilarDesdeParapeto: the base TAiState at offset 0, then four
// embedded TAiState sub-objects at +4, +8, +0xC and +0x10, each followed by a
// store of its own table pointer. Returns the object itself.
static TFncVigilarDesdeParapeto* __fastcall TFncVigilarDesdeParapeto_Constructor(TFncVigilarDesdeParapeto* self, void* /*edx*/)
{
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    self->vtbl = CSF_VTBL(0x00839E08);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    CSF_AT(uint32_t, self, 0x04) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00837968)));

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    CSF_AT(uint32_t, self, 0x08) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008379F8)));

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x0C), nullptr);
    CSF_AT(uint32_t, self, 0x0C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00837A80)));

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);
    CSF_AT(uint32_t, self, 0x10) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00837B10)));

    return self;
}

HOOK(0x004AE580, TFncVigilarDesdeParapeto_Constructor, "auto");
