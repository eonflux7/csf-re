#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraGetWorld, 0x0058DDF0, (int a1));
CSF_FUNC(int, __cdecl, RpWorldRemoveCamera, 0x0058DDC0, (int a1, int a2));
CSF_FUNC(int, __cdecl, _rwObjectHasFrameSetFrame, 0x0063CE30, (int object, int frame));
CSF_FUNC(int, __cdecl, RwRasterDestroy, 0x00627570, (uint32_t raster));
CSF_FUNC(int, __cdecl, RwCameraDestroy, 0x00629070, (int camera));

// Releases the RenderWare camera owned by this CCamera (field_000c): unlinks it
// from its world, clears its frame, destroys the two rasters it holds and then
// destroys the camera itself. Returns the RwCameraDestroy result, or 0 if there
// was no camera.
static int __fastcall CCamera_DestroyRwCamera(CCamera* self, void* /*edx*/) {
    int result = static_cast<int>(self->field_000c);
    if (result) {
        // Detach the camera from its world if it is attached to one.
        if (RwCameraGetWorld(static_cast<int>(self->field_000c))) {
            int camera = static_cast<int>(self->field_000c);
            int world = RwCameraGetWorld(camera);
            RpWorldRemoveCamera(world, camera);
        }

        // Clear the camera's frame if it has one.
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x04)) {
            _rwObjectHasFrameSetFrame(static_cast<int>(self->field_000c), 0);
        }

        // Destroy raster at +0x60.
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x60)) {
            uint32_t raster = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x60);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x60) = 0;
            RwRasterDestroy(raster);
        }

        // Destroy raster at +0x64.
        if (CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x64)) {
            uint32_t raster = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x64);
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_000c), 0x64) = 0;
            RwRasterDestroy(raster);
        }

        result = RwCameraDestroy(static_cast<int>(self->field_000c));
        self->field_000c = 0;
    }
    return result;
}

HOOK(0x006D2BF0, CCamera_DestroyRwCamera, "auto");
