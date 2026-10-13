#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraFrustumTestSphere, 0x00628EF0, (int a1, float* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxCamera_BuildBoxPlanes, 0x006AB510, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, GfxScene_HasSphereOverlap, 0x006CBD70, (void* self, void* , float* sphere, std::uint32_t* cache));
CSF_FUNC(char, __fastcall, CCamera_TestSphereAgainstPlanes, 0x006D35E0, (void* self, void* edx, float* a2, float* a3, char a4));

static bool __fastcall CCamera_IsSphereVisible(
    CCamera* self, void* /*edx*/, float* sphere,
    std::uint32_t* cache, int flags)
{
    const std::uint32_t combinedFlags = flags | self->field_0058;
    char visible = 1;

    if ((combinedFlags & 8) == 0)
    {
        if ((combinedFlags & 1) != 0)
        {
            alignas(8) std::uint8_t planeBytes[0x60];
            alignas(8) std::uint8_t boundsBytes[0x30];
            auto* bounds = reinterpret_cast<float*>(boundsBytes);

            const float radius = sphere[3];
            std::memset(planeBytes, 0, 96);

            bounds[9] = sphere[0] - radius;
            bounds[10] = sphere[1] - radius;
            bounds[11] = sphere[2] - radius;

            bounds[6] = radius + sphere[0];
            bounds[0] = bounds[6];
            bounds[3] = bounds[9];
            bounds[7] = radius + sphere[1];
            bounds[1] = bounds[7];
            bounds[8] = radius + sphere[2];
            bounds[2] = bounds[8];
            bounds[5] = bounds[11];
            bounds[4] = bounds[10];

            GfxCamera_BuildBoxPlanes(
                boundsBytes, nullptr,
                reinterpret_cast<std::uint32_t*>(planeBytes));
            visible = CCamera_TestSphereAgainstPlanes(
                self, nullptr, sphere,
                reinterpret_cast<float*>(planeBytes),
                static_cast<char>(flags));
        }
        else
        {
            visible = RwCameraFrustumTestSphere(
                static_cast<int>(self->field_000c), sphere) != 0;
        }

        if (!visible)
            return false;
    }

    if ((combinedFlags & 2) == 0 && !self->field_004c)
    {
        const float radiusSquared = sphere[3] * sphere[3];
        if (radiusSquared < self->field_00e0)
        {
            const float dx = sphere[0] - self->field_0068;
            const float dy = sphere[1] - self->field_006c;
            const float dz = sphere[2] - self->field_0070;
            const float distanceSquared = dz * dz + dy * dy + dx * dx;

            if (!(radiusSquared / distanceSquared > self->field_00dc))
                return false;

            visible = 1;
        }
    }

    if ((combinedFlags & 4) != 0)
        return visible != 0;

    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* scene = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));
    return GfxScene_HasSphereOverlap(scene, nullptr, sphere, cache) == 0;
}

HOOK(0x006D3760, CCamera_IsSphereVisible, "auto");
