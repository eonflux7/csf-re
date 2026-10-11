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

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(char, __fastcall, sub_4476E0, 0x004476E0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_4E6710, 0x004E6710, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4E7890, 0x004E7890, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, sub_4E79F0, 0x004E79F0, (void* self, void* edx));

static void __fastcall C_0083eb70_OnTimerExpired(C_0083eb70* self, void* /*edx*/)
{
    if (self->field_004c && self->field_0048 <= g_fZero)
    {
        void* scene = g_pCurrentScene;
        void* sceneSubObject = CSF_AT(void*, scene, 0xB0);
        int checkArg = reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self->field_0008) + 0x1c);

        if (sub_4476E0(sceneSubObject, nullptr, checkArg))
        {
            sub_4E6710(self, nullptr);
            sub_4E7890(self, nullptr);
            sub_4E79F0(self, nullptr);
        }
        else
        {
            self->field_0048 = 0.000001f;
        }
    }
}

HOOK(0x004E81F0, C_0083eb70_OnTimerExpired, "auto");
