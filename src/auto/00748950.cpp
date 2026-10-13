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

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(void, __fastcall, C_00867890_ResetVtableAndClearField4, 0x00740840, (C_00867890* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_748040, 0x00748040, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_748780, 0x00748780, (void* self, void* edx));

// Tears down the C_00868010 object: resets its vtables and the embedded
// C_00867890 member, clears the id field, and optionally releases the
// transport storage (when bit 0 of a2 is set). Returns the object itself.
static uint32_t* __fastcall C_00868010_Teardown(C_00868010* self, void* /*edx*/, char a2)
{
    // Embedded member at +0x308 (pad_0220[232]).
    C_00867890* embedded = reinterpret_cast<C_00867890*>(reinterpret_cast<uint8_t*>(self) + 0x308);

    self->vtbl = CSF_VTBL(0x00868010);
    CSF_AT(uint32_t, self, 0x308) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_VT(0x00868000)));
    self->field_0314 = 0xFFFFFFFFu;

    sub_748040(self, nullptr);
    C_00867890_ResetVtableAndClearField4(embedded, nullptr);
    sub_748780(self, nullptr);

    if ((a2 & 1) != 0)
        NetSession_ReleaseTransportStorage(static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00748950, C_00868010_Teardown, "auto");
