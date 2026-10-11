#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_75CF20, 0x0075CF20, (void* self, void* edx, uint32_t* a2));

// Forwards to sub_75CF20 with the same argument, then stores this object's
// field_001c into the dword at +0x18 of the returned object.
static int __fastcall C_0086c288_CopyIndexIntoResult(C_0086c288* self, void* /*edx*/, std::uint32_t* a2)
{
    int result = sub_75CF20(self, nullptr, reinterpret_cast<uint32_t*>(a2));
    CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x18) = self->field_001c;
    return result;
}

HOOK(0x0075D1D0, C_0086c288_CopyIndexIntoResult, "auto");
