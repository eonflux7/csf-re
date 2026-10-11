#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, sub_774120, 0x00774120, (void* self, void* edx));
CSF_FUNC(int, __cdecl, CreateListSentinel, 0x0050E9D0, ());

static uint32_t* __fastcall C_00840724_Construct(C_00840724* self, void* /*edx*/) {
    // Base-class initialization (C_00871a18 ctor, bound by address).
    sub_774120(self, nullptr);

    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->vtbl = CSF_VTBL(0x00840724);

    // Allocate and link the circular list sentinel node.
    int sentinelValue = CreateListSentinel();
    CSF_AT(uint32_t, self, 0x1C) = static_cast<uint32_t>(sentinelValue);
    CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x1C))), 0x11) = 1;

    // Node points to itself through next / prev / head links.
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x1C))), 0x04) =
        CSF_AT(uint32_t, self, 0x1C);
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x1C))), 0x00) =
        CSF_AT(uint32_t, self, 0x1C);
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x1C))), 0x08) =
        CSF_AT(uint32_t, self, 0x1C);

    CSF_AT(uint32_t, self, 0x20) = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00510680, C_00840724_Construct, "auto");
