#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S00402350_InitTransform, 0x00402350, (S_00402350* self, void*));
CSF_FUNC(int, __cdecl, _rpAtomicResyncInterpolatedSphere, 0x0058EB40, (int a1));
CSF_FUNC(float*, __cdecl, RwBBoxCalculate, 0x0063C8B0, (float* a1, float* a2, int a3));
CSF_FUNC(int, __fastcall, SceneMissionSetup_SetBounds, 0x006AB4E0, (void* self, void* edx, int a2, int a3, int a4, int a5, int a6, int a7));
CSF_FUNC(void, __fastcall, S_006abb80_ComputeAxisAlignedBounds, 0x006AC000, (S_006abb80* self, void* , float* maxX, float* minX, float* maxY, float* minY, float* maxZ, float* minZ));
CSF_FUNC(bool, __fastcall, ObjectAttachment_CopyBounds, 0x006B80B0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GfxFrames_SetBoundsFromSphere, 0x006C0450, (void* self, void* edx, float* a2));

static uint32_t* __cdecl GfxFrames_GetWorldBounds(
    uint32_t* output, int objectAddress, float attachmentFlag)
{
    alignas(8) uint8_t transformBytes[0x3C];
    alignas(8) uint8_t boundsBytes[0x18];
    alignas(8) uint8_t sphereBytes[0x10];
    alignas(8) uint8_t extremaBytes[0x14];

    CSF_AT(uint32_t, boundsBytes, 0x0C) = 0;
    CSF_AT(uint32_t, boundsBytes, 0x10) = 0;
    CSF_AT(uint32_t, boundsBytes, 0x14) = 0;
    CSF_AT(uint32_t, boundsBytes, 0x00) = 0;
    CSF_AT(uint32_t, boundsBytes, 0x04) = 0;
    CSF_AT(uint32_t, boundsBytes, 0x08) = 0;

    void* object = reinterpret_cast<void*>(objectAddress);
    bool copiedAttachment = false;

    if (CSF_AT(uint8_t, &attachmentFlag, 0) != 0)
    {
        void* attachment = reinterpret_cast<void*>(
            GfxObjectProps_GetObjectAttachment(objectAddress));

        // The packet identifies this binding at the transform initializer call.
        (void)CSF_VTBL(0x00402350);
        S00402350_InitTransform(
            reinterpret_cast<S_00402350*>(transformBytes), nullptr);

        if (attachment &&
            ObjectAttachment_CopyBounds(
                attachment, nullptr, reinterpret_cast<uint32_t*>(transformBytes)))
        {
            S_006abb80_ComputeAxisAlignedBounds(
                reinterpret_cast<S_006abb80*>(transformBytes), nullptr,
                reinterpret_cast<float*>(extremaBytes + 0x00),
                reinterpret_cast<float*>(extremaBytes + 0x04),
                reinterpret_cast<float*>(extremaBytes + 0x08),
                reinterpret_cast<float*>(extremaBytes + 0x0C),
                reinterpret_cast<float*>(extremaBytes + 0x10),
                &attachmentFlag);

            SceneMissionSetup_SetBounds(
                boundsBytes, nullptr,
                CSF_AT(int, extremaBytes, 0x00),
                CSF_AT(int, extremaBytes, 0x04),
                CSF_AT(int, extremaBytes, 0x08),
                CSF_AT(int, extremaBytes, 0x0C),
                CSF_AT(int, extremaBytes, 0x10),
                CSF_AT(int, &attachmentFlag, 0));
            copiedAttachment = true;
        }
    }

    if (!copiedAttachment)
    {
        void* geometry = reinterpret_cast<void*>(
            CSF_AT(uint32_t, object, 0x18));
        int vertexCount = 0;
        void* morphTarget = nullptr;
        float* vertices = nullptr;

        if (geometry &&
            (vertexCount = CSF_AT(int, geometry, 0x14)) != 0 &&
            (morphTarget = reinterpret_cast<void*>(
                CSF_AT(uint32_t, geometry, 0x5C))) != nullptr &&
            (vertices = reinterpret_cast<float*>(
                CSF_AT(uint32_t, morphTarget, 0x14))) != nullptr)
        {
            RwBBoxCalculate(
                reinterpret_cast<float*>(boundsBytes), vertices, vertexCount);
        }
        else
        {
            if ((CSF_AT(uint8_t, object, 0x50) & 2) != 0)
                _rpAtomicResyncInterpolatedSphere(objectAddress);

            if (objectAddress != -28)
            {
                CSF_AT(uint32_t, sphereBytes, 0x00) =
                    CSF_AT(uint32_t, object, 0x1C);
                uint32_t radius = CSF_AT(uint32_t, object, 0x28);
                CSF_AT(uint32_t, sphereBytes, 0x04) =
                    CSF_AT(uint32_t, object, 0x20);
                CSF_AT(uint32_t, sphereBytes, 0x08) =
                    CSF_AT(uint32_t, object, 0x24);
                CSF_AT(uint32_t, sphereBytes, 0x0C) = radius;

                GfxFrames_SetBoundsFromSphere(
                    boundsBytes, nullptr, reinterpret_cast<float*>(sphereBytes));
            }
        }
    }

    output[0] = CSF_AT(uint32_t, boundsBytes, 0x00);
    output[1] = CSF_AT(uint32_t, boundsBytes, 0x04);
    output[2] = CSF_AT(uint32_t, boundsBytes, 0x08);
    output[3] = CSF_AT(uint32_t, boundsBytes, 0x0C);
    output[4] = CSF_AT(uint32_t, boundsBytes, 0x10);
    output[5] = CSF_AT(uint32_t, boundsBytes, 0x14);
    return output;
}

HOOK(0x006C0570, GfxFrames_GetWorldBounds, "auto");
