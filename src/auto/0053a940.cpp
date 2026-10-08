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

// Base CMenuScreen constructor, bound by address. __thiscall with no stack args.
CSF_FUNC(void*, __fastcall, CMenuScreen_BaseConstruct, 0x712CC0, (void* self, void* edx));

// CMenuMain constructor (__thiscall, one stack argument, callee pops 4).
static CMenuMain* __fastcall CMenuMain_Construct(CMenuMain* self, void* /*edx*/, int menuOwner)
{
    // Primary vtable (CMenuMain-level header slot).
    CSF_AT(uint32_t, self, 0x000) = 0x0083042C;

    // Embedded CMenuScreen sub-object at +0x004.
    CMenuScreen_BaseConstruct(reinterpret_cast<uint8_t*>(self) + 0x004, nullptr);

    self->field_002c = static_cast<uint32_t>(menuOwner);

    // Sub-object vtable slot at +0x004.
    CSF_AT(uint32_t, self, 0x004) = 0x00842500;

    self->field_0018 = 0;
    self->field_0010 = 0;
    self->field_0030 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0014 = 0;

    // Final vtable for CMenuMain.
    CSF_AT(uint32_t, self, 0x000) = 0x00842550;

    self->field_0034 = 1;

    return self;
}

HOOK(0x0053A940, CMenuMain_Construct, "auto");
