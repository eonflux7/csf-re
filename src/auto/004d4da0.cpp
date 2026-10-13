#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

// Constructor of TDarObjeto: runs the TCharTask base constructor, installs the
// TDarObjeto vtable and a data pointer at +0x04, then initializes five embedded
// sub-objects (at +0x14, +0x28, +0x3C, +0x50, +0x64). Each sub-object gets the
// base constructor followed by two pointer stores. Returns this.
CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall TDarObjeto_Construct(TDarObjeto* self, void* /*edx*/)
{
    CharTask_InitBase(self, nullptr);
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083C0D8);
    CSF_AT(void*, self, 0x04) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083C0BC));

    // Embedded sub-object at +0x14
    void* sub14 = reinterpret_cast<uint8_t*>(self) + 0x14;
    CharTask_InitBase(sub14, nullptr);
    CSF_AT(void*, self, 0x14) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BD10));
    CSF_AT(void*, self, 0x18) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BCF4));

    // Embedded sub-object at +0x28
    void* sub28 = reinterpret_cast<uint8_t*>(self) + 0x28;
    CharTask_InitBase(sub28, nullptr);
    CSF_AT(void*, self, 0x28) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BDD8));
    CSF_AT(void*, self, 0x2C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BDB8));

    // Embedded sub-object at +0x3C
    void* sub3C = reinterpret_cast<uint8_t*>(self) + 0x3C;
    CharTask_InitBase(sub3C, nullptr);
    CSF_AT(void*, self, 0x3C) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BE98));
    CSF_AT(void*, self, 0x40) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BE7C));

    // Embedded sub-object at +0x50
    void* sub50 = reinterpret_cast<uint8_t*>(self) + 0x50;
    CharTask_InitBase(sub50, nullptr);
    CSF_AT(void*, self, 0x50) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BF50));
    CSF_AT(void*, self, 0x54) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BF30));

    // Embedded sub-object at +0x64
    void* sub64 = reinterpret_cast<uint8_t*>(self) + 0x64;
    CharTask_InitBase(sub64, nullptr);
    CSF_AT(void*, self, 0x64) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083C010));
    CSF_AT(void*, self, 0x68) = reinterpret_cast<void*>(CSF_ADDR_DATA(0x0083BFF4));

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D4DA0, TDarObjeto_Construct, "auto");
