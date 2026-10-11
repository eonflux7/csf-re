#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, sub_41ED90, 0x0041ED90, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_41E750, 0x0041E750, (void* self, void* edx));

static bool __fastcall S_0041e750_RebuildPathPoints(S_0041e750* self, void* /*edx*/, uint32_t* a2, C_0085e7b8* a3)
{
    uint32_t i = 0;
    self->field_000c = 0;

    uint32_t count = (uint32_t)(uintptr_t)GetCachedObjectProvider(a3, nullptr);
    uint32_t needed = count + 1;
    if (needed > self->field_0008)
    {
        sub_4DE9B0((uint8_t*)self + 4, nullptr, (int)(needed - self->field_0008));
    }

    sub_41ED90(self, nullptr, a2);

    if (count != 0)
    {
        do
        {
            uint32_t* pathPointRecord =
                (uint32_t*)(uintptr_t)GuiControl_GetPathPointRecord(a3, nullptr, (int)i);
            sub_41ED90(self, nullptr, pathPointRecord);
            ++i;
        } while (i < count);
    }

    sub_41E750(self, nullptr);
    self->field_0000 = 0;
    self->field_0001 = 1;
    self->field_0002 = 1;
    return true;
}

HOOK(0x0041EFD0, S_0041e750_RebuildPathPoints, "auto");
