#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Vtable pointer values stored into the object (the original writes these immediates).
static const uint32_t kCMenuScreenVtable = CSF_ADDR_VT(0x0083042C);     // CMenuScreen base vtable
static const uint32_t kEmbeddedMenuPartVtable = CSF_ADDR_VT(0x00842438); // vtable of the member at +0x04
static const uint32_t kCMenuPauseVtable = CSF_ADDR_VT(0x00842488);       // CMenuPause vtable

// Initializer of the embedded member object at CMenuPause+0x04 (__thiscall, no stack args).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

// Constructor of CMenuPause. Original: std::uint32_t *__thiscall sub_53A500(std::uint32_t *this, int a2), ret 4.
static CMenuPause* __fastcall CMenuPause_Construct(CMenuPause* self, void* /*edx*/, int restoreArg)
{
    // Embedded sub-object sits at self+4.
    uint8_t* embeddedPart = reinterpret_cast<uint8_t*>(self) + 4;

    // First vtable write: CMenuScreen base vtable (0x0083042C).
    CSF_AT(uint32_t, self, 0x00) = kCMenuScreenVtable;

    C_00830438_Construct(embeddedPart, nullptr);

    self->field_0038 = static_cast<uint32_t>(restoreArg);

    // Embedded member vtable (0x00842438).
    *reinterpret_cast<uint32_t*>(embeddedPart) = kEmbeddedMenuPartVtable;

    self->field_0018 = 0;
    self->field_0010 = 0;
    self->field_003c = 0;
    self->field_0020 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_001c = 0;
    self->field_0034 = 0;
    self->field_0024 = 0;
    self->field_0040 = 0;

    // Final vtable write: CMenuPause vtable (0x00842488).
    CSF_AT(uint32_t, self, 0x00) = kCMenuPauseVtable;

    self->field_0044 = 0xFFFFFFFFu;
    self->field_0014 = 0xFFFFFFFFu;

    return self;
}

HOOK(0x0053A500, CMenuPause_Construct, "auto");
