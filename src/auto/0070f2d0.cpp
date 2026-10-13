#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_70F220, 0x0070F220, (void* self, void* edx));
CSF_FUNC(int, __cdecl, _rwGetNumEngineInstances, 0x007DB820, ());
CSF_FUNC(int, __cdecl, sub_7DB830, 0x007DB830, ());
CSF_FUNC(int, __cdecl, _rxPipelineClose, 0x007DB960, ());
CSF_FUNC(int, __cdecl, sub_7DBC20, 0x007DBC20, (int a1));
CSF_FUNC(int, __cdecl, RpMTEffectDictGetCurrent, 0x007DBE70, ());
CSF_FUNC(int, __cdecl, sub_7DBE90, 0x007DBE90, ());
CSF_FUNC(int, __cdecl, AudioEffect_ReleaseHandle_7DEF60, 0x007DEF60,
         (void* handle, void* callback, int flags));
CSF_FUNC(int, __cdecl, sub_7F1810, 0x007F1810, ());

static int __fastcall C_00863ba8_CleanupAudio(
    C_00863ba8* self, void* /*edx*/)
{
    void* ownedObject = reinterpret_cast<void*>(self->field_0014);
    self->vtbl = CSF_VTBL(0x00863BA8);

    if (ownedObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0014 = 0;
    }

    sub_70F220(self, nullptr);

    if (sub_7DBE90())
    {
        int dictionary = sub_7DBE90();
        int currentDictionary = RpMTEffectDictGetCurrent();
        if (dictionary != currentDictionary)
            sub_7DBC20(sub_7DBE90());
    }

    if (self->field_0004)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0xD4 / 4])(self, nullptr);
        AudioEffect_ReleaseHandle_7DEF60(
            reinterpret_cast<void*>(self->field_0004), nullptr, 0);
        self->field_0004 = 0;
    }

    uint32_t secondaryHandle = self->field_0008;
    if (secondaryHandle)
    {
        AudioEffect_ReleaseHandle_7DEF60(
            reinterpret_cast<void*>(secondaryHandle), nullptr, 0);
        self->field_0008 = 0;
    }

    int result = _rwGetNumEngineInstances();
    if (result)
    {
        sub_7F1810();
        _rxPipelineClose();
        result = sub_7DB830();
    }

    self->vtbl = CSF_VTBL(0x00863800);
    return result;
}

HOOK(0x0070F2D0, C_00863ba8_CleanupAudio, "auto");
