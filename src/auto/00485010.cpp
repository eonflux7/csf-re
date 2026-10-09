#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copy-constructs the 0x26-byte state record at src into self, then sets the
// vtable at +4. Returns self, popping the one stack argument (ret 4).
static uint32_t* __fastcall C_0082d448_CopyConstructFrom(uint32_t* self, void* /*edx*/, int srcPtr)
{
    uint8_t* src = reinterpret_cast<uint8_t*>(srcPtr);
    uint8_t* dst = reinterpret_cast<uint8_t*>(self);

    CSF_AT(uint32_t, dst, 0x00) = CSF_AT(uint32_t, src, 0x00);
    CSF_AT(void*, dst, 0x04) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, dst, 0x08) = CSF_AT(uint32_t, src, 0x08);
    CSF_AT(uint32_t, dst, 0x0C) = CSF_AT(uint32_t, src, 0x0C);

    CSF_AT(uint32_t, dst, 0x10) = CSF_AT(uint32_t, src, 0x10);
    CSF_AT(uint32_t, dst, 0x14) = CSF_AT(uint32_t, src, 0x14);
    CSF_AT(uint32_t, dst, 0x18) = CSF_AT(uint32_t, src, 0x18);

    CSF_AT(uint32_t, dst, 0x1C) = CSF_AT(uint32_t, src, 0x1C);

    CSF_AT(uint8_t, dst, 0x20) = CSF_AT(uint8_t, src, 0x20);
    CSF_AT(uint8_t, dst, 0x21) = CSF_AT(uint8_t, src, 0x21);
    CSF_AT(uint8_t, dst, 0x22) = CSF_AT(uint8_t, src, 0x22);
    CSF_AT(uint8_t, dst, 0x23) = CSF_AT(uint8_t, src, 0x23);
    CSF_AT(uint8_t, dst, 0x24) = CSF_AT(uint8_t, src, 0x24);
    CSF_AT(uint8_t, dst, 0x25) = CSF_AT(uint8_t, src, 0x25);

    return self;
}

HOOK(0x00485010, C_0082d448_CopyConstructFrom, "auto");
