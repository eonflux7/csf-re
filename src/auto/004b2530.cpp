#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base constructor of TAiState, invoked for the base subobject and each embedded sub-object.
CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

// Builds a TDistraido: the base TAiState at offset 0, followed by four TAiState-sized
// sub-objects at +4, +8, +0xC and +0x10. Each sub-object is constructed and then gets a
// data-table pointer written into its first dword. Returns the object address.
static uint32_t* __fastcall TDistraido_Construct(TDistraido* self, void* /*edx*/) {
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    *reinterpret_cast<void**>(self) = CSF_VTBL(0x0083A248);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 4), nullptr);
    CSF_AT(uint32_t, self, 4) = (uint32_t)CSF_ADDR_DATA(0x008355E8);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 8), nullptr);
    CSF_AT(uint32_t, self, 8) = (uint32_t)CSF_ADDR_DATA(0x00839000);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0xC), nullptr);
    CSF_AT(uint32_t, self, 0xC) = (uint32_t)CSF_ADDR_DATA(0x00835678);

    TAiState_Constructor(reinterpret_cast<TAiState*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);
    CSF_AT(uint32_t, self, 0x10) = (uint32_t)CSF_ADDR_DATA(0x00835710);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x4B2530, TDistraido_Construct, "auto");
