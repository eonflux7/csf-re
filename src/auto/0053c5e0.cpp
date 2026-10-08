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

// Embedded base sub-object initializer (this+4), __thiscall, no stack args.
CSF_FUNC(void*, __fastcall, MenuDemoExit_InitEmbeddedBase, 0x712CC0, (void* self, void* edx));

// CMenuDemoExit constructor (__thiscall, one stack arg: owner/parent pointer, ret 4).
static CMenuDemoExit* __fastcall CMenuDemoExit_Construct(CMenuDemoExit* self, void* /*edx*/, int parentOwner)
{
    // Primary vtable (CMenuScreen layer).
    CSF_AT(uint32_t, self, 0x000) = 0x0083042C;

    // Initialize embedded base sub-object at +0x004.
    MenuDemoExit_InitEmbeddedBase(reinterpret_cast<uint8_t*>(self) + 0x004, nullptr);

    self->field_0018 = static_cast<uint32_t>(parentOwner);

    // Sub-object vtable at +0x004.
    self->field_0004 = 0x008429B8;

    self->m_pGui = 0;
    self->m_pPendingMenu = 0;
    self->m_pManager = 0;
    self->m_timer = 0.0f;

    // Final vtable: CMenuDemoExit.
    CSF_AT(uint32_t, self, 0x000) = 0x00842A08;

    return self;
}

HOOK(0x0053C5E0, CMenuDemoExit_Construct, "auto");
