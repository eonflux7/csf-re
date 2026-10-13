#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StdException_Construct, 0x007AF6A0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuExtras_AssignStringRange, 0x00403B60,
         (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));

// Constructs the C_008769d4 object: base std::exception init, vtable install,
// string-like member reset (length 0, capacity 15, SSO flag cleared), then
// assigns the full source string into the embedded sub-object at +0x0C.
static C_008769d4* __fastcall C_008769d4_Construct(C_008769d4* self, void* /*edx*/, void* source) {
    StdException_Construct(self, nullptr);
    self->vtbl = CSF_VTBL(0x008769D4);
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 15;
    CSF_AT(uint8_t, self, 0x10) = 0;
    MenuExtras_AssignStringRange(reinterpret_cast<uint8_t*>(self) + 0x0C, nullptr, source, 0u, 0xFFFFFFFFu);
    return self;
}

HOOK(0x403DA0, C_008769d4_Construct, "auto");
