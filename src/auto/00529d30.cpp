#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwRasterRenderScaled, 0x00627470, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwRasterPushContext, 0x006275B0, (int raster));
CSF_FUNC(int, __cdecl, RwRasterPopContext, 0x00627650, ());
CSF_FUNC(int, __cdecl, RwRasterCreate, 0x00627770, (int width, int height, int depth, int flags));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static int __fastcall CMenuMessageBox_ContinueUpdate(
    CMenuMessageBox* self, void* /*edx*/)
{
    alignas(8) uint8_t rectangleBytes[0x10];
    int* rectangle = reinterpret_cast<int*>(rectangleBytes);

    void* application = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);

    if (!self->m_raster)
    {
        int height =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, application, 0)[0x24 / 4])(
                    application, nullptr);
        int width =
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, application, 0)[0x20 / 4])(
                    application, nullptr);

        int raster = RwRasterCreate(width, height, 24, 5);
        self->m_raster = static_cast<uint32_t>(raster);
        if (raster)
        {
            rectangle[0] = 0;
            rectangle[1] = 0;
            rectangle[2] = CSF_AT(int, reinterpret_cast<void*>(raster), 0xC);
            rectangle[3] = CSF_AT(int, reinterpret_cast<void*>(raster), 0x10);

            RwRasterPushContext(raster);
            int displayRaster = AppMain_GetDisplayRaster(application, nullptr);
            RwRasterRenderScaled(displayRaster, reinterpret_cast<int>(rectangle));
            RwRasterPopContext();
        }
    }

    void* menuControls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, menuControls, 0)[0x74 / 4])(
                menuControls, nullptr, 0))
    {
        int raster = static_cast<int>(self->m_raster);
        if (raster)
        {
            rectangle[0] = 0;
            rectangle[1] = 0;
            rectangle[2] = CSF_AT(int, reinterpret_cast<void*>(raster), 0xC);
            rectangle[3] = CSF_AT(int, reinterpret_cast<void*>(raster), 0x10);

            int displayRaster = AppMain_GetDisplayRaster(application, nullptr);
            RwRasterPushContext(displayRaster);
            RwRasterRenderScaled(
                static_cast<int>(self->m_raster),
                reinterpret_cast<int>(rectangle));
            RwRasterPopContext();
        }

        uint32_t* controlFields[3] = {
            &self->field_006c,
            &self->field_0070,
            &self->field_0074
        };
        for (int i = 0; i < 3; ++i)
        {
            void* control = reinterpret_cast<void*>(*controlFields[i]);
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, control, 0)[0x134 / 4])(
                    control, nullptr);
        }

        menuControls = MenuControls_ResolveObject(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, menuControls, 0)[0x78 / 4])(
                menuControls, nullptr);
    }

    void* currentApplication = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    return reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, currentApplication, 0)[0x2C / 4])(
            currentApplication, nullptr);
}

HOOK(0x00529D30, CMenuMessageBox_ContinueUpdate, "auto");
