#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class initializer at 0x5183C0 (__fastcall, ecx = this).
CSF_FUNC(uint32_t*, __fastcall, sub_5183C0, 0x005183C0, (void* self, void* edx));
// Constructs one embedded menu member object (ecx = object).
CSF_FUNC(void*, __fastcall, MenuMemberObject_Construct, 0x005188D0, (void* self, void* edx));

// Constructor for CNetProfile: runs the base init, installs the vtable,
// constructs the 10 embedded menu member objects, then sets default field values.
static uint32_t* __fastcall CNetProfile_Construct(CNetProfile* self, void* /*edx*/)
{
    sub_5183C0(self, nullptr);

    self->vtbl = CSF_VTBL(0x0083F6C0);

    // Ten MenuMemberObject elements, starting at +0x0C, stride 0x24C.
    uint8_t* elementAddress = reinterpret_cast<uint8_t*>(self) + 0x0C;
    for (int remaining = 10; remaining != 0; --remaining)
    {
        MenuMemberObject_Construct(elementAddress, nullptr);
        elementAddress += 0x24C;
    }

    self->field_1720 = 0;
    self->field_1724 = 15;
    CSF_AT(uint8_t, self, 0x1710) = 0;
    self->field_173c = 0;
    self->field_1740 = 7;
    CSF_AT(uint16_t, self, 0x172C) = 0;
    self->field_1758 = 0;
    self->field_175c = 7;
    CSF_AT(uint16_t, self, 0x1748) = 0;
    self->field_0008 = 0;

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x506D80, CNetProfile_Construct, "auto");
