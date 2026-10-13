#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_GetBoundsPlanes, 0x006AB510, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundsCenter, 0x006AB660, (void* self, void* edx, float* a2));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_GetBoundingSphere, 0x006AB7A0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, S_006cbc80_TestSpatialMatch, 0x006CBC80, (S_006cbc80* self, void* , float* a2, float* a3, int cacheAddress));
CSF_FUNC(char, __fastcall, CCamera_TestBoundsVisibility, 0x006D35E0, (void* self, void* edx, float* a2, float* a3, char a4));

// The packet also lists this direct-call address as a vtable reference.
static const auto kSpatialMatchReference = CSF_VTBL(0x006CBC80);

static char __fastcall CCamera_TestSpatialVisibility(
    CCamera* self, void* /*edx*/, int objectAddress, int cacheAddress, int flags)
{
    const uint32_t combinedFlags = flags | self->field_0058;
    char visible = 1;
    void* object = reinterpret_cast<void*>(objectAddress);

    alignas(8) uint8_t boundsBytes[0x60];
    alignas(8) uint8_t sphereBytes[0x10];

    if ((combinedFlags & 8) == 0)
    {
        std::memset(boundsBytes, 0, sizeof(boundsBytes));
        SceneMissionSetup_GetBoundsPlanes(
            object, nullptr, reinterpret_cast<uint32_t*>(boundsBytes));

        float* center = SceneMissionSetup_GetBoundsCenter(
            object, nullptr, reinterpret_cast<float*>(sphereBytes + 4));

        visible = CCamera_TestBoundsVisibility(
            self, nullptr, center, reinterpret_cast<float*>(boundsBytes),
            static_cast<char>(flags));
        if (!visible)
            return visible;
    }

    if ((combinedFlags & 2) == 0 && !self->field_004c)
    {
        std::memset(sphereBytes, 0, sizeof(sphereBytes));
        SceneMissionSetup_GetBoundingSphere(
            object, nullptr, reinterpret_cast<float*>(sphereBytes));

        const float radius = CSF_AT(float, sphereBytes, 0x0C);
        const float radiusSquared = radius * radius;
        if (radiusSquared < self->field_00e0)
        {
            const float dx = CSF_AT(float, sphereBytes, 0x00) - self->field_0068;
            const float dy = CSF_AT(float, sphereBytes, 0x04) - self->field_006c;
            const float dz = CSF_AT(float, sphereBytes, 0x08) - self->field_0070;
            const float distanceSquared = dz * dz + dy * dy + dx * dx;

            if (!(radiusSquared / distanceSquared > self->field_00dc))
                return 0;
            visible = 1;
        }
    }

    if ((combinedFlags & 4) != 0)
        return visible;

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    S_006cbc80* controller = reinterpret_cast<S_006cbc80*>(
        SceneLoader_GetSceneController(global, nullptr));

    return S_006cbc80_TestSpatialMatch(
        controller, nullptr,
        reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(object) + 12),
        reinterpret_cast<float*>(object), cacheAddress) == 0;
}

HOOK(0x006D3990, CCamera_TestSpatialVisibility, "auto");
