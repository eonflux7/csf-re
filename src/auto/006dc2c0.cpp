#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AppMain_GetDisplayRaster, 0x00534FA0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, RwImageCreate, 0x00625060, (int a1, int a2, int a3));
CSF_FUNC(uint32_t*, __cdecl, RwImageAllocatePixels, 0x006250C0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwImageDestroy, 0x00625F10, (uint32_t* a1));
CSF_FUNC(uint32_t*, __cdecl, RwImageSetFromRaster, 0x00633010, (uint32_t* a1, int a2));

using GetSystemMetricsFn = int (__stdcall*)(int);
using GetActiveWindowFn = void* (__stdcall*)();
using ClientToScreenFn = int (__stdcall*)(void*, void*);

CSF_GLOBAL(GetSystemMetricsFn, g_GetSystemMetrics, 0x0082D2BC);
CSF_GLOBAL(GetActiveWindowFn, g_GetActiveWindow, 0x0082D27C);
CSF_GLOBAL(ClientToScreenFn, g_ClientToScreen, 0x0082D280);

static uint32_t* __fastcall C_00861460_CaptureDisplayImage(
    C_00861460* self, void* /*edx*/)
{
    uint32_t* image = nullptr;
    bool succeeded = false;

    if (static_cast<int32_t>(self->field_001c) > 0)
    {
        const auto getSystemMetrics = g_GetSystemMetrics;
        int width = self->field_0018
            ? static_cast<int32_t>(self->field_0004)
            : getSystemMetrics(0);
        int height = self->field_0018
            ? static_cast<int32_t>(self->field_0008)
            : getSystemMetrics(1);

        alignas(8) uint8_t textureBytes[0x8];
        alignas(8) uint8_t surfaceBytes[0x8];
        CSF_AT(void*, textureBytes, 0) = nullptr;
        CSF_AT(void*, surfaceBytes, 0) = nullptr;

        void* device = reinterpret_cast<void*>(self->field_005c);
        if (((int (__stdcall*)(void*, int, int, int, uint32_t, int,
                               int, void**, uint32_t))
                 CSF_AT(void**, device, 0)[0x5C / 4])(
                device, width, height, 1, 0, 21, 2,
                &CSF_AT(void*, textureBytes, 0), 0) >= 0)
        {
            void* texture = CSF_AT(void*, textureBytes, 0);
            if (((int (__stdcall*)(void*, uint32_t, void**))
                     CSF_AT(void**, texture, 0)[0x48 / 4])(
                    texture, 0, &CSF_AT(void*, surfaceBytes, 0)) >= 0)
            {
                device = reinterpret_cast<void*>(self->field_005c);
                if (((int (__stdcall*)(void*, uint32_t, void*))
                         CSF_AT(void**, device, 0)[0x84 / 4])(
                        device, 0, CSF_AT(void*, surfaceBytes, 0)) >= 0)
                {
                    alignas(8) uint8_t rectangleBytes[0x10];
                    alignas(8) uint8_t pointBytes[0x20];
                    alignas(8) uint8_t lockedRectangleBytes[0x8];

                    CSF_AT(int32_t, rectangleBytes, 0x0) = 0;
                    CSF_AT(int32_t, rectangleBytes, 0x4) = 0;
                    CSF_AT(int32_t, rectangleBytes, 0x8) = width;
                    CSF_AT(int32_t, rectangleBytes, 0xC) = height;

                    if (!self->field_0018)
                    {
                        CSF_AT(int32_t, pointBytes, 0x0) = 0;
                        CSF_AT(int32_t, pointBytes, 0x4) = 0;
                        void* window = g_GetActiveWindow();
                        g_ClientToScreen(window, pointBytes);
                        width = static_cast<int32_t>(self->field_0004);
                        height = static_cast<int32_t>(self->field_0008);
                        CSF_AT(int32_t, rectangleBytes, 0x0) =
                            CSF_AT(int32_t, pointBytes, 0x0);
                        CSF_AT(int32_t, rectangleBytes, 0x4) =
                            CSF_AT(int32_t, pointBytes, 0x4);
                        CSF_AT(uint32_t, rectangleBytes, 0x8) =
                            static_cast<uint32_t>(width) +
                            CSF_AT(uint32_t, pointBytes, 0x0);
                        CSF_AT(uint32_t, rectangleBytes, 0xC) =
                            static_cast<uint32_t>(height) +
                            CSF_AT(uint32_t, pointBytes, 0x4);
                    }

                    image = RwImageCreate(width, height, 32);
                    if (image)
                    {
                        succeeded = RwImageAllocatePixels(image) != nullptr;
                        if (succeeded)
                        {
                            auto* destination =
                                reinterpret_cast<uint8_t*>(
                                    CSF_AT(uint32_t, image, 0x14));
                            if (destination)
                            {
                                const int destinationStride =
                                    CSF_AT(int32_t, image, 0x10);
                                void* surface = CSF_AT(void*, surfaceBytes, 0);
                                if (((int (__stdcall*)(void*, void*, void*, uint32_t))
                                         CSF_AT(void**, surface, 0)[0x34 / 4])(
                                        surface, lockedRectangleBytes,
                                        rectangleBytes, 0) < 0)
                                {
                                    succeeded = false;
                                }
                                else
                                {
                                    auto* source = CSF_AT(uint8_t*, lockedRectangleBytes, 0x4);
                                    for (int row = 0; row < height; ++row)
                                    {
                                        uint8_t* outputPixel = destination;
                                        uint8_t* inputPixel = source;
                                        for (int column = 0; column < width; ++column)
                                        {
                                            outputPixel[0] = inputPixel[2];
                                            outputPixel[1] = inputPixel[1];
                                            outputPixel[2] = inputPixel[0];
                                            outputPixel[3] = inputPixel[3];
                                            outputPixel += 4;
                                            inputPixel += 4;
                                        }
                                        destination += destinationStride;
                                        source += CSF_AT(int32_t, lockedRectangleBytes, 0);
                                    }

                                    surface = CSF_AT(void*, surfaceBytes, 0);
                                    ((void (__stdcall*)(void*))
                                         CSF_AT(void**, surface, 0)[0x38 / 4])(surface);
                                }
                            }
                        }
                    }
                }
            }
        }

        void* surface = CSF_AT(void*, surfaceBytes, 0);
        if (surface)
            ((void (__stdcall*)(void*))
                 CSF_AT(void**, surface, 0)[0x8 / 4])(surface);

        void* texture = CSF_AT(void*, textureBytes, 0);
        if (texture)
            ((void (__stdcall*)(void*))
                 CSF_AT(void**, texture, 0)[0x8 / 4])(texture);
    }
    else
    {
        const int raster = AppMain_GetDisplayRaster(self, nullptr);
        if (!raster)
            return nullptr;

        image = RwImageCreate(
            static_cast<int32_t>(self->field_0004),
            static_cast<int32_t>(self->field_0008), 32);
        if (!image)
            return image;

        succeeded = RwImageAllocatePixels(image) != nullptr;
        if (succeeded)
            succeeded = RwImageSetFromRaster(image, raster) != nullptr;
    }

    if (image && !succeeded)
        RwImageDestroy(image);
    return image;
}

HOOK(0x006DC2C0, C_00861460_CaptureDisplayImage, "auto");
