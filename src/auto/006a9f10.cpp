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

CSF_FUNC(char*, __fastcall, sub_656DE0, 0x00656DE0, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, sub_653E90, 0x00653E90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_652810, 0x00652810, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, sub_6508A0, 0x006508A0, (void* self, void* edx));

// __thiscall with 2 stack args (ret 8): a2 = vtable pointer, a3 = int
static C_0085839c* __fastcall C_0085839c_Construct(C_0085839c* self, void* /*edx*/, void* vtable, int a3)
{
    // First sub-object at self+0x08
    void* firstSub = reinterpret_cast<uint8_t*>(self) + 0x08;
    CSF_AT(uint32_t, self, 0x0C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sub_656DE0(firstSub, nullptr)));
    CSF_AT(uint32_t, self, 0x10) = 0;

    // Second sub-object at self+0x18
    void* secondSub = reinterpret_cast<uint8_t*>(self) + 0x18;
    CSF_AT(uint32_t, self, 0x1C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sub_653E90(secondSub, nullptr)));
    CSF_AT(uint32_t, self, 0x20) = 0;

    CSF_AT(void*, self, 0x00) = vtable;
    CSF_AT(uint32_t, self, 0x04) = 0;

    sub_652810(firstSub, nullptr);
    sub_6508A0(secondSub, nullptr);

    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(float, self, 0x2C) = -1.0f;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint32_t, self, 0x34) = 0;
    CSF_AT(uint32_t, self, 0x38) = 0;
    CSF_AT(int, self, 0x3C) = a3;
    CSF_AT(uint32_t, self, 0x40) = 0;

    return self;
}

HOOK(0x006A9F10, C_0085839c_Construct, "auto");
