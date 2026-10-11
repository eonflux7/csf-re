#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

// Constructor of TAtacar: runs the TCharTask base constructor, installs the
// TAtacar vtable and data pointer at +4, then initializes five embedded
// sub-objects (at +0x14, +0x28, +0x3C, +0x54, +0x68). Each sub-object gets the
// base constructor followed by two pointer stores. Returns this.
static uint32_t* __fastcall TAtacar_Construct(TAtacar* self, void* /*edx*/)
{
    CharTask_InitBase(self, nullptr);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x00832D80);
    CSF_AT(void*, self, 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832D64));

    // Embedded sub-object at +0x14
    void* sub14 = reinterpret_cast<uint8_t*>(self) + 0x14;
    CharTask_InitBase(sub14, nullptr);
    CSF_AT(void*, self, 0x14) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832550));
    CSF_AT(void*, self, 0x18) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832530));

    // Embedded sub-object at +0x28
    void* sub28 = reinterpret_cast<uint8_t*>(self) + 0x28;
    CharTask_InitBase(sub28, nullptr);
    CSF_AT(void*, self, 0x28) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832608));
    CSF_AT(void*, self, 0x2C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008325E8));

    // Embedded sub-object at +0x3C
    void* sub3C = reinterpret_cast<uint8_t*>(self) + 0x3C;
    CharTask_InitBase(sub3C, nullptr);
    CSF_AT(void*, self, 0x3C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008326C8));
    CSF_AT(void*, self, 0x40) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x008326AC));

    // Embedded sub-object at +0x54
    void* sub54 = reinterpret_cast<uint8_t*>(self) + 0x54;
    CharTask_InitBase(sub54, nullptr);
    CSF_AT(void*, self, 0x54) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832780));
    CSF_AT(void*, self, 0x58) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832760));

    // Embedded sub-object at +0x68
    void* sub68 = reinterpret_cast<uint8_t*>(self) + 0x68;
    CharTask_InitBase(sub68, nullptr);
    CSF_AT(void*, self, 0x68) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832838));
    CSF_AT(void*, self, 0x6C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x00832818));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0047BE50, TAtacar_Construct, "auto");
