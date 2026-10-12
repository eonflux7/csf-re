#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __fastcall, sub_6B31E0, 0x006B31E0, (void* self, void* edx));

static bool __fastcall GfxObject_CheckActiveAndResetFlag(int* self, void* /*edx*/)
{
    uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self[1]));

    bool result;
    if ((CSF_AT(uint32_t, obj, 0x64) & 0x140) == 0x140 && sub_6B31E0(obj, nullptr) != 0)
        result = true;
    else
        result = (CSF_AT(uint32_t, obj, 0x64) & 8) != 0;

    uint32_t flags = CSF_AT(uint32_t, obj, 0x64);
    if ((flags & 0x20000) != 0)
    {
        CSF_AT(uint32_t, obj, 0x78) = 0xBF800000u;
        CSF_AT(uint32_t, obj, 0x64) = flags & 0xFFFDFFFFu;
    }
    return result;
}

HOOK(0x006B3F30, GfxObject_CheckActiveAndResetFlag, "auto");
