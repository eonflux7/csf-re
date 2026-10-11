#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Constructor of TNavegando: runs the TCharTask base constructor, installs the
// TNavegando vtable, then initializes six embedded sub-objects (at +0x14, +0x28,
// +0x3C, +0x54, +0x68). Each sub-object gets the base constructor followed by two
// pointer stores. Returns this.
CSF_FUNC(uint32_t*, __fastcall, TCharTask_BaseConstruct, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall TNavegando_Construct(TNavegando* self, void* /*edx*/)
{
    TCharTask_BaseConstruct(self, nullptr);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0082F878);
    CSF_AT(void*, self, 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F85C));

    // Embedded sub-object at +0x14
    void* sub14 = reinterpret_cast<uint8_t*>(self) + 0x14;
    TCharTask_BaseConstruct(sub14, nullptr);
    CSF_AT(void*, self, 0x14) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082EF70));
    CSF_AT(void*, self, 0x18) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082EF54));

    // Embedded sub-object at +0x28
    void* sub28 = reinterpret_cast<uint8_t*>(self) + 0x28;
    TCharTask_BaseConstruct(sub28, nullptr);
    CSF_AT(void*, self, 0x28) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F030));
    CSF_AT(void*, self, 0x2C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F010));

    // Embedded sub-object at +0x3C
    void* sub3C = reinterpret_cast<uint8_t*>(self) + 0x3C;
    TCharTask_BaseConstruct(sub3C, nullptr);
    CSF_AT(void*, self, 0x3C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F0F8));
    CSF_AT(void*, self, 0x40) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F0D8));

    // Embedded sub-object at +0x54
    void* sub54 = reinterpret_cast<uint8_t*>(self) + 0x54;
    TCharTask_BaseConstruct(sub54, nullptr);
    CSF_AT(void*, self, 0x54) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F1B8));
    CSF_AT(void*, self, 0x58) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F19C));

    // Embedded sub-object at +0x68
    void* sub68 = reinterpret_cast<uint8_t*>(self) + 0x68;
    TCharTask_BaseConstruct(sub68, nullptr);
    CSF_AT(void*, self, 0x68) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F280));
    CSF_AT(void*, self, 0x6C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0082F264));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00421450, TNavegando_Construct, "auto");
