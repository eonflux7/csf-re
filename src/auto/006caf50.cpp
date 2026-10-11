#include <cstdint>
#include <cstring>
#include "hook.h"
#include "globals.h"

// 0x006B89B0: __cdecl setter (ret 0) that stores a global float value.
CSF_FUNC(int, __cdecl, SetGlobalFloat_8A7E40, 0x006B89B0, (float value));

// 0x006CAF50: __stdcall wrapper (ret 4). Takes one raw 32-bit argument and
// forwards its bit pattern to SetGlobalFloat_8A7E40 as a float, returning the
// callee's result unchanged. The original never reads ecx, so this is a plain function.
static int __stdcall SceneGfx_SetGlobalFloatFromDword(int rawBits)
{
    float value;
    std::memcpy(&value, &rawBits, sizeof(value));
    return SetGlobalFloat_8A7E40(value);
}

HOOK(0x6CAF50, SceneGfx_SetGlobalFloatFromDword, "auto");
