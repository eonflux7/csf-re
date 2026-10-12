#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, S_006abb80_WriteBoxCorners, 0x006ABC40, (S_006abb80* self, void* , float* corners));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(float*, __fastcall, GfxMatrix_GetBoundingSphere, 0x006AC530, (float* self, void* , float* output));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, S_006cbc80_TestSpatialMatch, 0x006CBC80, (S_006cbc80* self, void* , float* a2, float* a3, int cacheAddress));
CSF_FUNC(char, __fastcall, CCamera_TestBoxFrustum, 0x006D35E0, (void* self, void* edx, float* a2, float* a3, char a4));

// Retain the binding explicitly listed in the packet.
static const auto kSpatialMatchVtable = CSF_VTBL(0x006CBC80);

static char __fastcall CCamera_IsBoxVisible(
    CCamera* self, void* /*edx*/, S_006abb80* box,
    int cacheAddress, int flags)
{
    const uint32_t combinedFlags = self->field_0058 | flags;
    char visible = 1;

    if ((combinedFlags & 8) == 0)
    {
        alignas(8) uint8_t cornerBytes[0x60];
        std::memset(cornerBytes, 0, sizeof(cornerBytes));
        float* corners = reinterpret_cast<float*>(cornerBytes);
        S_006abb80_WriteBoxCorners(box, nullptr, corners);
        visible = CCamera_TestBoxFrustum(
            self, nullptr, reinterpret_cast<float*>(&box->field_0000),
            corners, static_cast<char>(flags));
        if (!visible)
            return visible;
    }

    alignas(8) uint8_t scratchBytes[0x1C];
    float* scratch = reinterpret_cast<float*>(scratchBytes);

    if ((combinedFlags & 2) == 0 && !self->field_004c)
    {
        std::memset(scratchBytes, 0, 0x10);
        GfxMatrix_GetBoundingSphere(
            reinterpret_cast<float*>(box), nullptr, scratch);

        const float radiusSquared = scratch[3] * scratch[3];
        if (radiusSquared < self->field_00e0)
        {
            const float dx = scratch[0] - self->field_0068;
            const float dy = scratch[1] - self->field_006c;
            const float dz = scratch[2] - self->field_0070;
            const float sizeRatio =
                radiusSquared / (dz * dz + dy * dy + dx * dx);
            if (!(sizeRatio > self->field_00dc))
                return 0;
            visible = 1;
        }
    }

    if ((combinedFlags & 4) != 0)
        return visible;

    std::memset(scratchBytes + 0x04, 0, 0x18);
    void* globals = reinterpret_cast<void*>(GetGlobalInt());
    S_006cbc80* sceneController =
        reinterpret_cast<S_006cbc80*>(
            SceneLoader_GetSceneController(globals, nullptr));

    float* minimum = scratch + 1;
    float* maximum = scratch + 4;
    S_006abb80_ComputeAxisAlignedBounds(
        box, nullptr,
        maximum, minimum,
        maximum + 1, minimum + 1,
        maximum + 2, minimum + 2);

    return S_006cbc80_TestSpatialMatch(
        sceneController, nullptr, minimum, maximum, cacheAddress) == 0;
}

HOOK(0x006D3B80, CCamera_IsBoxVisible, "auto");
