#include <cstdint>

#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void*, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, CMenuMpServerInfo_ApplyReadyState, 0x005322A0, (CMenuMpServerInfo* self, void* edx, char a2));

static int __fastcall CMenuMpServerInfo_SetServerEntryAndRefresh(CMenuMpServerInfo* self, void* /*edx*/, int a2, int /*a3, unused*/)
{
    self->field_01f4 = 0;
    self->field_01e8 = static_cast<uint32_t>(a2);

    void* globalValueObj = GetGlobalValue_521BB0();
    MenuProgressInput_UpdateGlobalState(globalValueObj, nullptr);

    // Virtual call on the server data object, vtable slot 0x16C / 4.
    void* dataObj = reinterpret_cast<void*>(self->field_01ec);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, dataObj, 0)[0x16C / 4])(dataObj, nullptr);

    if (self->field_0010 && self->field_0014 && self->field_0018 && self->field_001c &&
        self->field_0028 && self->field_002c && self->field_0030 && self->field_0034 &&
        self->field_0040 && self->field_0044 && self->field_0048)
    {
        CMenuMpServerInfo_ApplyReadyState(self, nullptr, 1);
    }

    // Virtual call on the server data object (reloaded), vtable slot 0xA8 / 4, argument 0.
    void* dataObjReloaded = reinterpret_cast<void*>(self->field_01ec);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, dataObjReloaded, 0)[0xA8 / 4])(dataObjReloaded, nullptr, 0);

    return static_cast<int>(self->field_01ec);
}

HOOK(0x00532920, CMenuMpServerInfo_SetServerEntryAndRefresh, "auto");
