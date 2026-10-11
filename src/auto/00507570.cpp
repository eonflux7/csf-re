#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRasterClear, 0x006274B0, (int a1));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());
CSF_FUNC(int, __cdecl, RwRasterShowRaster, 0x006276E0, (int a1, int a2, int a3));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BB4CC, 0x006C8B80, ());
CSF_FUNC(int, __fastcall, C_008655e8_EndCameraUpdate, 0x006D2BE0, (C_008655e8* self, void*));
CSF_FUNC(bool, __fastcall, GfxSkin_BeginCameraUpdate, 0x006D3010, (void* self, void* edx, int arg0, float a3, float a2, float a5, int a6));
CSF_FUNC(int, __cdecl, _vsprintf, 0x007AFCAA, (char* buffer, const char* format, __builtin_va_list args));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

static int __cdecl AppMain_AppendPageNameAndRefreshDisplay(int owner, char* format, ...)
{
    alignas(8) uint8_t bufferBytes[0x400];
    char* buffer = reinterpret_cast<char*>(bufferBytes);

    __builtin_va_list args;
    __builtin_va_start(args, format);
    _vsprintf(buffer, format, args);
    __builtin_va_end(args);

    alignas(8) uint8_t nameBytes[0x1C];
    C_0082d410& name = *reinterpret_cast<C_0082d410*>(nameBytes);
    CSF_AT(uint32_t, nameBytes, 0x18) = 15;
    CSF_AT(uint32_t, nameBytes, 0x14) = 0;
    reinterpret_cast<char*>(&name.field_0004)[0] = 0;
    AssignStringRange_429B60(&name, nullptr, buffer,
                            static_cast<unsigned int>(std::strlen(buffer)));

    uint8_t* pageNames = reinterpret_cast<uint8_t*>(owner) + 36;
    MenuManager_AppendPageName(reinterpret_cast<S_00470ab0*>(pageNames),
                               nullptr, &name);

    if (CSF_AT(uint32_t, nameBytes, 0x18) >= 0x10 && name.field_0004)
        Mem_FreeDebug(reinterpret_cast<void*>(name.field_0004));

    void* registry = reinterpret_cast<void*>(GetGlobalInt());
    void* display = LookupObjectById(registry, nullptr);
    const int raster = AppMain_GetDisplayRaster(display, nullptr);
    if (!raster)
        return 0;

    int remaining = 2;
    do
    {
        for (unsigned int index = 0; ; ++index)
        {
            const uint32_t begin = CSF_AT(uint32_t, pageNames, 0x4);
            if (!begin)
                break;

            const int32_t byteCount = static_cast<int32_t>(
                CSF_AT(uint32_t, pageNames, 0x8) - begin);
            if (index >= static_cast<unsigned int>(byteCount / 28))
                break;

            GetGlobalValue_8BB4CC();
        }

        RwRasterPushContext(raster);
        RwRasterClear(0);
        RwRasterPopContext();

        void* collection = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
        void* first = reinterpret_cast<void*>(GetFirstElement(collection, nullptr));
        void* provider = GetCachedObjectProvider(first, nullptr);
        C_008655e8* camera = reinterpret_cast<C_008655e8*>(
            GetReferencedObject(provider, nullptr));

        int alphaBits;
        std::memcpy(&alphaBits, &g_ColorBlack_A, sizeof(alphaBits));
        GfxSkin_BeginCameraUpdate(camera, nullptr, 0, g_ColorBlack,
                                 g_ColorBlack_G, g_ColorBlack_B, alphaBits);
        GetGlobalValue_8BB4CC();
        C_008655e8_EndCameraUpdate(camera, nullptr);
        RwRasterShowRaster(raster, 0, 1);
    }
    while (--remaining);

    return remaining;
}

HOOK(0x00507570, AppMain_AppendPageNameAndRefreshDisplay, "auto");
