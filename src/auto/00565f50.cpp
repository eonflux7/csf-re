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

// Embedded base sub-object initializer (__thiscall, no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x712CC0, (void* self, void* edx));

// CMenuMpGameMode constructor (__thiscall, one stack arg: owning menu/parent).
// Initializes the CMenuScreen base, the embedded sub-object at +4, and the
// game-mode fields, then sets the final vtables and returns this.
static CMenuMpGameMode* __fastcall CMenuMpGameMode_Construct(CMenuMpGameMode* self, void* /*edx*/, int parentMenu)
{
    // Embedded sub-object lives at +4 (address only, via lea).
    void* embeddedBase = reinterpret_cast<uint8_t*>(self) + 4;

    // Base CMenuScreen vtable.
    CSF_AT(uint32_t, self, 0x0) = CSF_ADDR_VT(0x0083042C);

    C_00830438_Construct(embeddedBase, nullptr);

    CSF_AT(uint32_t, self, 0x1C) = static_cast<uint32_t>(parentMenu);
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;

    // Final CMenuMpGameMode vtable and embedded sub-object vtable.
    CSF_AT(uint32_t, self, 0x0) = CSF_ADDR_VT(0x00845CA8);
    CSF_AT(uint32_t, self, 0x4) = 0x00845C58;

    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;

    return self;
}

HOOK(0x565F50, CMenuMpGameMode_Construct, "auto");
