#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(char, __fastcall, CommandoAnims_UpdateLinkedEntries, 0x004FC7E0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRGBAToPixel, 0x00625870, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_ResetSkinState, 0x006C59E0, (void* self, void*));
CSF_FUNC(void, __fastcall, ClearFlagF4, 0x006C5A30, (uint8_t* self, void*));

static char __fastcall Interface_UpdateAndClearDisplay(void* self, void* /*edx*/)
{
    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x94 / 4])(self, nullptr);

    char result = CSF_AT(char, self, 0xFC);
    bool ready = false;

    if (result != 0 &&
        CSF_AT(uint8_t, self, 0x108) != 0 &&
        CSF_AT(int, self, 0x130) == 2 &&
        CSF_AT(int, self, 0x134) == 0)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        result = ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, state, 0)[0x3C / 4])(state, nullptr);

        if (result == 0)
        {
            state = reinterpret_cast<void*>(GetGlobalState_50D190());
            result = ((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, state, 0)[0x38 / 4])(state, nullptr);
            ready = result == 0;
        }
    }

    if (!ready)
    {
        if (CSF_AT(uint8_t, self, 0x181) == 0)
            return result;
    }
    else if (CSF_AT(uint8_t, self, 0x181) == 0)
    {
        if (CSF_AT(uint8_t, self, 0x151) != 0)
        {
            GfxSkin_ResetSkinState(
                reinterpret_cast<void*>(GetGlobalValue_6C5760()), nullptr);
            CommandoAnims_UpdateLinkedEntries(
                reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr);

            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, self, 0)[0x80 / 4])(self, nullptr);

            int skin = GetGlobalValue_6C5760();
            ClearFlagF4(reinterpret_cast<uint8_t*>(skin), nullptr);
            // ClearFlagF4 leaves the getter's return value unchanged.
            return static_cast<char>(skin);
        }

        CSF_AT(uint8_t, self, 0x151) = 1;
    }

    alignas(8) uint8_t colorBytes[0x4];
    colorBytes[3] = 0xFF;
    colorBytes[2] = 0;
    colorBytes[1] = 0;
    colorBytes[0] = 0;

    void* display = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    int raster = AppMain_GetDisplayRaster(display, nullptr);
    RwRasterPushContext(raster);

    raster = AppMain_GetDisplayRaster(display, nullptr);
    uint16_t format = static_cast<uint16_t>(
        CSF_AT(uint8_t, reinterpret_cast<void*>(raster), 0x23) << 8);

    int pixel = RwRGBAToPixel(reinterpret_cast<int>(colorBytes), format);
    RwRasterClear(pixel);
    return static_cast<char>(RwRasterPopContext());
}

HOOK(0x00536160, Interface_UpdateAndClearDisplay, "auto");
