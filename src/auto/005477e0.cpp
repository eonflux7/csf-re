#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void, __fastcall, MenuExtras_StopGlobalPlayback, 0x004FB9F0, (void* self, void* edx, int enabled));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __stdcall, OnlineMenu_SetSharedText, 0x00521E30, (int a1));
CSF_FUNC(int, __cdecl, MemDebug_FreeDebugBlock, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, MenuRuntime_SetPageByIndex, 0x006B15D0, (C_0082d410* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GlobalSlot_ApplyAndClear, 0x006B1660, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(int, __cdecl, ResetStringRecordRange, 0x0072EF70, (int begin, int end));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CMenuExtras_ResetActiveExtra(CMenuExtras* self, void* /*edx*/)
{
    void* activeExtra = reinterpret_cast<void*>(self->field_0048);
    if (activeExtra)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, activeExtra, 0)[0])(activeExtra, nullptr, 1);
        self->field_0048 = 0;
    }

    self->field_005c = 0;
    if (self->field_0050)
    {
        ResetStringRecordRange(
            static_cast<int>(self->field_0050),
            static_cast<int>(self->field_0054));
        if (self->field_0050)
            MemDebug_FreeDebugBlock(reinterpret_cast<void*>(self->field_0050));
    }
    self->field_0050 = 0;
    self->field_0054 = 0;
    self->field_0058 = 0;

    GlobalSlot_ApplyAndClear(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    void* globalService = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, globalService, 0)[0x90 / 4])(globalService, nullptr);

    void* sceneEntry = reinterpret_cast<void*>(self->vSlot20());
    char* pageName = reinterpret_cast<char* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, sceneEntry, 0)[0x190 / 4])(sceneEntry, nullptr);

    alignas(8) uint8_t pageNameBytes[0x20];
    C_0082d410& pageString =
        *reinterpret_cast<C_0082d410*>(pageNameBytes);
    CSF_AT(uint32_t, pageNameBytes, 0x18) = 15;
    CSF_AT(uint32_t, pageNameBytes, 0x14) = 0;
    reinterpret_cast<uint8_t*>(&pageString.field_0004)[0] = 0;

    AssignStringRange_429B60(
        &pageString, nullptr, pageName,
        static_cast<unsigned int>(std::strlen(pageName)));

    MenuRuntime_SetPageByIndex(
        reinterpret_cast<C_0082d410*>(GetGlobalInt()), nullptr,
        reinterpret_cast<int>(&pageString), 2, 1065353216);

    if (CSF_AT(uint32_t, pageNameBytes, 0x18) >= 0x10 &&
        pageString.field_0004)
    {
        MemDebug_FreeDebugBlock(reinterpret_cast<void*>(pageString.field_0004));
    }

    void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* stateObject = CSF_AT(void*, state, 0x78);
    if (stateObject)
        MenuExtras_StopGlobalPlayback(stateObject, nullptr, 1);

    self->field_0044 = 1;
    self->field_0045 = 1;
    self->field_0047 = 0;

    void* menuObject = reinterpret_cast<void*>(self->field_0018);
    if (menuObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, menuObject, 0)[0xC0 / 4])(menuObject, nullptr, 1);
    }

    void* sharedState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    uint32_t capacity = CSF_AT(uint32_t, sharedState, 0xF9C);
    uint8_t* sharedString = reinterpret_cast<uint8_t*>(sharedState) + 0xF84;
    int sharedText;
    if (capacity < 0x10)
        sharedText = reinterpret_cast<int>(sharedString + 4);
    else
        sharedText = CSF_AT(int, sharedString, 4);

    GetGlobalValue_521BB0();
    return OnlineMenu_SetSharedText(sharedText);
}

HOOK(0x005477E0, CMenuExtras_ResetActiveExtra, "auto");
