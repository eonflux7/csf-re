#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Vtable addresses referenced by the constructor, bound by address.
// The stores below write these addresses into the object's vtable pointers.
CSF_GLOBAL(void*, g_CMenuScreenSubBaseVtable, 0x0083042C);
CSF_GLOBAL(void*, g_CMenuExtrasSecondaryBaseVtable, 0x00843C68);
CSF_GLOBAL(void*, g_CMenuExtrasVtable, 0x00843CB8);

// Callee: base-class initializer, __thiscall (ecx = this + 4, 0 stack args).
CSF_FUNC(void*, __fastcall, CMenuExtras_InitSecondaryBase, 0x00712CC0, (void* self, void* edx));

// Constructor of CMenuExtras. __thiscall with one stack argument (ret 4).
static CMenuExtras* __fastcall CMenuExtras_Construct(CMenuExtras* self, void* /*edx*/, int menuArg)
{
    // Install the CMenuScreen sub-base vtable (0x0083042C), then initialize the embedded base at +4.
    CSF_AT(uint32_t, self, 0x000) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(g_CMenuScreenSubBaseVtable));
    CMenuExtras_InitSecondaryBase(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    // Install the final CMenuExtras vtable (0x00843CB8) and the secondary base vtable (0x00843C68).
    CSF_AT(uint32_t, self, 0x000) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(g_CMenuExtrasVtable));
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(g_CMenuExtrasSecondaryBaseVtable));

    // Zero-initialized members.
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_0018 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0044 = 0;
    self->field_0045 = 0;
    self->field_0046 = 0;
    self->field_0047 = 0;
    self->field_0048 = 0;
    self->field_005c = 0;
    self->field_0010 = 0;
    self->field_0024 = 0;

    // Constructor argument and sentinel.
    self->field_0014 = static_cast<uint32_t>(menuArg);
    self->field_0028 = 0xFFFFFFFFu;

    return self;
}

HOOK(0x00547CB0, CMenuExtras_Construct, "auto");

