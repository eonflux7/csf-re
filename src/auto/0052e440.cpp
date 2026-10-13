#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ClearCampaignEntryList, 0x0043F890, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __cdecl, SoundMgr_StartActiveInstanceIfReady, 0x00521F10, ());
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall CMenuScenarioSelect_DestroyBody(
    CMenuScenarioSelect* self, void* /*edx*/)
{
    void* child = reinterpret_cast<void*>(self->field_0018);

    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x00841688));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00841638));

    if (child)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0])(child, nullptr, 1);
        self->field_0018 = 0;

        GetGlobalValue_521BB0();
        SoundMgr_StartActiveInstanceIfReady();
        ClearCampaignEntryList(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);
    }

    if (self->field_0090 >= 0x10 && self->field_007c)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_007c));

    self->field_0090 = 15;
    self->field_008c = 0;
    *reinterpret_cast<uint8_t*>(&self->field_007c) = 0;

    int result = ClearCampaignEntryList(
        reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);

    if (self->field_0034 >= 0x10)
    {
        result = static_cast<int>(self->field_0020);
        if (result)
            result = Mem_FreeDebug(reinterpret_cast<void*>(self->field_0020));
    }

    self->field_0030 = 0;
    self->field_0034 = 15;
    *reinterpret_cast<uint8_t*>(&self->field_0020) = 0;

    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00830488));
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x00830438));
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(CSF_VTBL(0x0083042C));

    return result;
}

HOOK(0x0052E440, CMenuScenarioSelect_DestroyBody, "auto");
