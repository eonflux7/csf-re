#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// S_00402350 constructor: fills the object at ecx with its vtable and defaults.
CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));

// Vtable that InitS00402350Object installs in the local S_00402350 object.
[[maybe_unused]] static void* const kS00402350Vtbl = CSF_VTBL(0x00402350);

// Builds a 0x3C-byte snapshot of the water state (vtable, two dwords from the
// embedded sub-object at +8, and the float at +0x14 replicated three times)
// and copies it into the caller's output buffer. Returns the output buffer.
static void* __fastcall C_00863760_ExportWaterStateTo(C_00863760* self, void* /*edx*/, void* outBuffer)
{
    // Local S_00402350 object: room 0x48 bytes in the frame.
    alignas(8) uint8_t stateBytes[0x48];

    InitS00402350Object(stateBytes, nullptr);

    const float waterValue = self->field_0014;

    void* subVtbl = (void*)CSF_AT(uint32_t, self, 0x08);
    const uint32_t subField4 = CSF_AT(uint32_t, self, 0x0C);
    const uint32_t subField8 = CSF_AT(uint32_t, self, 0x10);

    CSF_AT(void*, stateBytes, 0x00) = subVtbl;
    CSF_AT(uint32_t, stateBytes, 0x04) = subField4;
    CSF_AT(uint32_t, stateBytes, 0x08) = subField8;
    CSF_AT(float, stateBytes, 0x0C) = waterValue;
    CSF_AT(float, stateBytes, 0x10) = waterValue;
    CSF_AT(float, stateBytes, 0x14) = waterValue;

    std::memcpy(outBuffer, stateBytes, 0x3C);
    return outBuffer;
}

HOOK(0x0070BD30, C_00863760_ExportWaterStateTo, "auto");
