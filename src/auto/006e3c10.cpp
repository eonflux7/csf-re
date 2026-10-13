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

// Per-item commit helper (original 0x006E3A70, __thiscall, ret 8).
CSF_FUNC(int, __fastcall, GfxWater_CommitObject, 0x006E3A70, (void* self, void* edx, void* a2, float* a3));

CSF_GLOBAL(float, g_ColorRed, 0x008A7DB0);

// Flush the cached water objects: optionally commit the first list with the
// 0x8A7DD0 tint, then optionally commit the second list with g_ColorRed.
static char __fastcall GfxWater_CommitCachedObjects(S_006e3740* self, void* /*edx*/)
{
    int v5 = 0;

    if (self->field_0004)
    {
        void* obj = reinterpret_cast<void*>(self->field_0000);
        uint32_t count = ((uint32_t(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[16])(obj, nullptr);
        for (uint32_t i = 0; i < count; ++i)
        {
            obj = reinterpret_cast<void*>(self->field_0000);
            void* item = ((void* (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, obj, 0)[21])(obj, nullptr, i);
            float* tint = reinterpret_cast<float*>(CSF_ADDR_DATA(0x008A7DD0));
            GfxWater_CommitObject(self, nullptr, item, tint);
        }
    }

    v5 = self->field_0005;
    if ((char)v5)
    {
        void* obj = reinterpret_cast<void*>(self->field_0000);
        v5 = ((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[15])(obj, nullptr);
        uint32_t count = (uint32_t)v5;
        for (uint32_t j = 0; j < count; ++j)
        {
            obj = reinterpret_cast<void*>(self->field_0000);
            void* item = ((void* (__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, obj, 0)[20])(obj, nullptr, j);
            v5 = GfxWater_CommitObject(self, nullptr, item, &g_ColorRed);
        }
    }

    return (char)v5;
}

HOOK(0x006E3C10, GfxWater_CommitCachedObjects, "auto");
