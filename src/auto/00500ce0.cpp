#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Original: 0x004109E0 AssignStringSubstring(this, source, position, count), ret 12
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));

// Original: 0x00453260 sub_453260(this, a2), ret 4
CSF_FUNC(uint32_t*, __fastcall, sub_453260, 0x00453260,
         (void* self, void* edx, uint32_t* a2));

// Original: 0x00500CE0, __thiscall C_0082d410 copy-from-source. Ret 4.
static int __fastcall C_0082d410_CopyFrom(C_0082d410* self, void* /*edx*/, int a2)
{
    CSF_AT(uint32_t, self, 0x18) = 15;
    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint8_t, self, 0x04) = 0;

    AssignStringSubstring(self, nullptr, reinterpret_cast<const void*>(a2), 0u, 0xFFFFFFFFu);

    uint8_t* src = reinterpret_cast<uint8_t*>(a2);
    const uint32_t subobjectVtbl =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));

    CSF_AT(uint8_t, self, 0x1C) = CSF_AT(uint8_t, src, 0x1C);
    CSF_AT(uint8_t, self, 0x1D) = CSF_AT(uint8_t, src, 0x1D);
    CSF_AT(uint32_t, self, 0x20) = CSF_AT(uint32_t, src, 0x20);
    CSF_AT(uint32_t, self, 0x24) = subobjectVtbl;
    CSF_AT(uint32_t, self, 0x28) = CSF_AT(uint32_t, src, 0x28);
    CSF_AT(uint32_t, self, 0x2C) = CSF_AT(uint32_t, src, 0x2C);
    CSF_AT(uint32_t, self, 0x30) = subobjectVtbl;
    CSF_AT(uint32_t, self, 0x34) = CSF_AT(uint32_t, src, 0x34);
    CSF_AT(uint32_t, self, 0x38) = CSF_AT(uint32_t, src, 0x38);
    CSF_AT(uint32_t, self, 0x3C) = subobjectVtbl;
    CSF_AT(uint32_t, self, 0x40) = CSF_AT(uint32_t, src, 0x40);
    CSF_AT(uint32_t, self, 0x44) = CSF_AT(uint32_t, src, 0x44);

    sub_453260(reinterpret_cast<uint8_t*>(self) + 0x48, nullptr,
               reinterpret_cast<uint32_t*>(src + 0x48));

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00500CE0, C_0082d410_CopyFrom, "auto");
