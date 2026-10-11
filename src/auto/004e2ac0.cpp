#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));

static int __fastcall C_0083e6d8_DetachFromSceneAndReset(C_0083e6d8* self, void* /*edx*/)
{
    int result;

    self->field_0018 = 1;
    CScene_RemoveObjectFromCategories(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                                      (int)(self->field_000c + 0x1C), 1);

    void* linkedObj = reinterpret_cast<void*>(self->field_000c);
    result = ((int(__fastcall*)(void*, void*, int, int, int, int))
              CSF_AT(void**, linkedObj, 0)[0x8C / 4])(linkedObj, nullptr, 10, 0, -1, 0);

    self->field_001c = 1092616192;
    return result;
}

HOOK(0x004E2AC0, C_0083e6d8_DetachFromSceneAndReset, "auto");
