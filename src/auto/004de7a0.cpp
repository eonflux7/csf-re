#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: 0x004DE100, __fastcall-style with one stack argument (ret 4).
CSF_FUNC(int, __fastcall, SceneSubObjectLookup, 0x004DE100, (void* self, void* edx, unsigned int a2));

// Original: __thiscall, one stack argument (ret 4). If the flag byte at +5 is set,
// the original tail-jumps to 0x4DE100 with ecx = [this+0x18] and the same stack argument.
// Otherwise it returns 0.
static int __fastcall SceneFindSubObject(void* self, void* /*edx*/, unsigned int a2)
{
    int result = 0;
    if (CSF_AT(uint8_t, self, 0x05) != 0)
    {
        void* subObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18));
        return SceneSubObjectLookup(subObject, nullptr, a2);
    }
    return result;
}

HOOK(0x004DE7A0, SceneFindSubObject, "auto");
