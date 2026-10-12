#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_0082e598_Construct, 0x0041E1D0, (C_0082e598* self, void*, int initialValue));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360, (C_0086c12c* self, void*));

static int* __fastcall C_0082fcd8_Construct(C_0082fcd8* self, void* /*edx*/, int a2)
{
    C_0082e598_Construct(reinterpret_cast<C_0082e598*>(self), nullptr, a2);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082FCD8);
    CSF_AT(uint32_t, self, 0x04) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082FCB8));

    C_0086c12c_Construct(reinterpret_cast<C_0086c12c*>(reinterpret_cast<uint8_t*>(self) + 0x24), nullptr);

    CSF_AT(float, self, 0x38) = 0.0f;
    CSF_AT(float, self, 0x3C) = 0.0f;
    CSF_AT(float, self, 0x40) = 0.0f;
    CSF_AT(uint8_t, self, 0x48) = 1;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(int, self, 0x4C) = 0;
    CSF_AT(int, self, 0x50) = 0;
    CSF_AT(int, self, 0x5C) = 0;
    CSF_AT(int, self, 0x58) = 0;
    CSF_AT(int, self, 0x44) = 0;
    CSF_AT(int, self, 0x20) = 0;

    // virtual slot 0xA8 / 4 = 42, called with this and argument 0
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, self, 0)[42])(self, nullptr, 0);

    return reinterpret_cast<int*>(self);
}

HOOK(0x425050, C_0082fcd8_Construct, "auto");
