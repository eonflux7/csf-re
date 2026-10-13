#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void, __fastcall, MenuExtras_StopGlobalPlayback, 0x004FB9F0, (void* self, void* edx, int value));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, FlushGlobalValueState, 0x00521F80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuExtras_LoadMakingOfSubtitles, 0x00548430, (void* self, void*));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetValue_8BAF98, 0x0069C370, ());
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, MenuRuntime_SetPageByIndex, 0x006B15D0, (C_0082d410* self, void* , int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, GlobalSlot_ApplyAndClear, 0x006B1660, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static char __fastcall S_00548430_PerformAlternateAction(
    S_00548430* self, void* /*edx*/)
{
    char result = static_cast<char>(self->field_0047);
    if (result)
        return result;

    self->field_0044 = 1;
    self->field_0045 = 0;
    self->field_0047 = 1;

    // The packet labels this callable address as a vtable as well.
    (void)CSF_VTBL(0x00548430);
    MenuExtras_LoadMakingOfSubtitles(self, nullptr);

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    void* playback = CSF_AT(void*, globalState, 0x78);
    if (playback)
        MenuExtras_StopGlobalPlayback(playback, nullptr, 0);

    GlobalSlot_ApplyAndClear(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    FlushGlobalValueState(
        reinterpret_cast<void*>(GetGlobalValue_521BB0()), nullptr);

    void* runtime = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, runtime, 0)[0x8C / 4])(runtime, nullptr);

    void* menu = reinterpret_cast<void*>(self->field_0018);
    if (menu)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, menu, 0)[0xC0 / 4])(menu, nullptr, 0);
    }

    int language = GetValue_8BAF98();
    int pageIndex = 1;
    if (language == 10 || language == 14)
        pageIndex = 5;

    alignas(8) uint8_t videoPathBytes[0x1C];
    C_0082d410& videoPath =
        *reinterpret_cast<C_0082d410*>(videoPathBytes);

    CSF_AT(uint32_t, &videoPath, 0x18) = 15;
    CSF_AT(uint32_t, &videoPath, 0x14) = 0;
    *reinterpret_cast<uint8_t*>(&videoPath.field_0004) = 0;
    AssignStringRange_429B60(
        &videoPath, nullptr,
        const_cast<char*>(CSF_STR(0x00843D50, "videos/making.bik")), 17);

    auto* menuRuntime = reinterpret_cast<C_0082d410*>(GetGlobalInt());
    result = MenuRuntime_SetPageByIndex(
        menuRuntime, nullptr, reinterpret_cast<int>(&videoPath),
        pageIndex, 1065353216);

    if (CSF_AT(uint32_t, &videoPath, 0x18) >= 0x10)
    {
        uint32_t allocation = videoPath.field_0004;
        result = static_cast<char>(allocation);
        if (allocation)
            return static_cast<char>(
                Mem_FreeDebug(reinterpret_cast<void*>(allocation)));
    }
    return result;
}

HOOK(0x00548650, S_00548430_PerformAlternateAction, "auto");
