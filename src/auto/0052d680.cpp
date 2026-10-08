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

// Base-class constructor of the embedded sub-object at this+4 (__thiscall, no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x712CC0, (void* self, void* edx));

// Constructor of CMenuProgressBar (__thiscall, one stack arg: ret 4).
static CMenuProgressBar* __fastcall CMenuProgressBar_Construct(CMenuProgressBar* self, void* /*edx*/, int progressOwner)
{
    // Primary vtable of CMenuScreen base at offset 0 (set before the embedded sub-object is built).
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x0083042C);

    // Embedded sub-object at +4: construct it.
    void* embedded = reinterpret_cast<uint8_t*>(self) + 0x0004;
    C_00830438_Construct(embedded, nullptr);

    // Store the constructor argument at +0x24.
    self->field_0024 = static_cast<uint32_t>(progressOwner);

    // Sub-object vtable at +4.
    CSF_AT(uint32_t, self, 0x0004) = 0x00841548;

    // Clear +0x20 and +0x1C.
    self->field_0020 = 0;
    self->field_001c = 0;

    // Final vtable for CMenuProgressBar.
    CSF_AT(uint32_t, self, 0x0000) = CSF_ADDR_VT(0x00841598);

    // Progress value sentinel -1 at +0x14.
    self->field_0014 = 0xFFFFFFFFu;

    return self;
}

HOOK(0x52D680, CMenuProgressBar_Construct, "auto");
