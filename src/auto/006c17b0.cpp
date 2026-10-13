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

CSF_FUNC(int, __cdecl, RwTexDictionaryForAllTextures, 0x006316E0, (int dictionary, int (__cdecl* callback)(uint32_t*, int), int context));
CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());

static int __fastcall C_008607f8_EnumerateCurrentTextures(C_008607f8* self, void* /*edx*/)
{
    int currentDictionary = RwTexDictionaryGetCurrent();
    auto textureCallback = reinterpret_cast<int (__cdecl*)(uint32_t*, int)>(CSF_ADDR_FN(0x006C1230));
    int result = RwTexDictionaryForAllTextures(currentDictionary, textureCallback, 0);
    self->field_0004 = 0;
    return result;
}

HOOK(0x006C17B0, C_008607f8_EnumerateCurrentTextures, "auto");
