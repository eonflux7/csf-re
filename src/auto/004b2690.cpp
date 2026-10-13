#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// TAiState base constructor (__thiscall, no stack args, ret 0).
CSF_FUNC(TAiState*, __fastcall, TAiState_Constructor, 0x004DBF70, (TAiState* self, void*));

// TEstatico::TEstatico(): constructs the TAiState base at offset 0, then the
// six embedded state sub-objects at +4, +8, +0xC, +0x14, +0x18, +0x1C
// (+0x10 is skipped by the original). Each sub-object gets its base constructor
// and then a dword store of a data-table address. Returns this in eax.
static TEstatico* __fastcall TEstatico_Construct(TEstatico* self, void* /*edx*/)
{
    TAiState_Constructor(reinterpret_cast<TAiState*>(self), nullptr);
    *(void***)self = CSF_VTBL(0x0083A3F8);

    TAiState_Constructor(reinterpret_cast<TAiState*>((uint8_t*)self + 0x4), nullptr);
    CSF_AT(uint32_t, self, 0x4) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00839488));

    TAiState_Constructor(reinterpret_cast<TAiState*>((uint8_t*)self + 0x8), nullptr);
    CSF_AT(uint32_t, self, 0x8) = static_cast<uint32_t>(CSF_ADDR_DATA(0x008365E0));

    TAiState_Constructor(reinterpret_cast<TAiState*>((uint8_t*)self + 0xC), nullptr);
    CSF_AT(uint32_t, self, 0xC) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836668));

    TAiState_Constructor(reinterpret_cast<TAiState*>((uint8_t*)self + 0x14), nullptr);
    CSF_AT(uint32_t, self, 0x14) = static_cast<uint32_t>(CSF_ADDR_DATA(0x008366F0));

    TAiState_Constructor(reinterpret_cast<TAiState*>((uint8_t*)self + 0x18), nullptr);
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836778));

    TAiState_Constructor(reinterpret_cast<TAiState*>((uint8_t*)self + 0x1C), nullptr);
    CSF_AT(uint32_t, self, 0x1C) = static_cast<uint32_t>(CSF_ADDR_DATA(0x00836808));

    return self;
}

HOOK(0x004B2690, TEstatico_Construct, "auto");
