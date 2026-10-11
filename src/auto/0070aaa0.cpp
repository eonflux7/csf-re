// Destroys a C_00863600 (water object): tears down its render handle and optionally frees the block.
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

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, sub_6D6E60, 0x006D6E60, (uint32_t* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_6FF710, 0x006FF710, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_6FF970, 0x006FF970, (void* self, void* edx));

static int __fastcall C_00863600_Destroy(C_00863600* self, void* /*edx*/, char freeFlag)
{
    uint8_t ownsRender = self->field_0011;
    *(void***)self = CSF_VTBL(0x00863600);
    if (ownsRender)
    {
        sub_6FF710(self, nullptr);
        uint32_t renderObj = self->field_0004;
        self->field_0030 = 0;
        if (renderObj)
        {
            C_0085e7b8* globalInt = reinterpret_cast<C_0085e7b8*>(GetGlobalInt());
            int renderHandle = RenderHandle_FromObject(globalInt, nullptr);
            sub_6D6E60(reinterpret_cast<uint32_t*>(renderHandle), nullptr, (int)self->field_0004);
            self->field_0004 = 0;
        }
    }
    sub_6FF970(self, nullptr);
    if ((freeFlag & 1) != 0)
        Mem_Free(self);
    return (int)self;
}

HOOK(0x0070AAA0, C_00863600_Destroy, "auto");
