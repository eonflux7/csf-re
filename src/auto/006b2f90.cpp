#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, GfxObject_DestroyTransform, 0x006BF180,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObject_SetTransformSource, 0x006BF380,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, GfxObject_TransformSphereCenter, 0x006BFC10,
         (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, GfxObject_InitializeTransform, 0x006BFD00,
         (void* self, void* edx));

CSF_GLOBAL(float, g_ControlHoldThreshold, 0x0083BB18);

static int __fastcall C_00860288_UpdateBoundingSphere(
    C_00860288* self, void* /*edx*/)
{
    alignas(8) uint8_t transformBytes[0x20];
    alignas(8) uint8_t centerBytes[0x28];
    float radius = 0.0f;

    if (self->field_001c != 0)
    {
        uint32_t index = 0;
        do
        {
            auto* children =
                reinterpret_cast<C_00860288**>(self->field_0018);
            C_00860288* child = children[index];
            if (child)
            {
                GfxObject_InitializeTransform(transformBytes, nullptr);

                void* source = reinterpret_cast<void*>(child->field_000c);
                GfxObject_SetTransformSource(
                    transformBytes, nullptr, CSF_AT(int, source, 0x4));

                char* childSphere = child->vSlot18();
                auto* transformedCenter = reinterpret_cast<float*>(
                    GfxObject_TransformSphereCenter(
                        transformBytes, nullptr,
                        reinterpret_cast<uint32_t*>(centerBytes),
                        reinterpret_cast<int>(childSphere)));

                const float childX = transformedCenter[0];
                const float childY = transformedCenter[1];
                const float childZ = transformedCenter[2];

                void* centerObject =
                    reinterpret_cast<uint8_t*>(self) + 0x30;
                auto* center =
                    ((float*(__fastcall*)(void*, void*))
                         CSF_AT(void**, centerObject, 0)[0x10 / 4])(
                        centerObject, nullptr);

                const float dx = center[0] - childX;
                const float dy = center[1] - childY;
                const float dz = center[2] - childZ;

                childSphere = child->vSlot18();
                const float extent =
                    std::sqrt(dz * dz + dy * dy + dx * dx) +
                    CSF_AT(float, childSphere, 0xC);
                if (extent > radius)
                    radius = extent;

                GfxObject_DestroyTransform(transformBytes, nullptr);
            }

            ++index;
        } while (index < self->field_001c);
    }

    if (self->field_0028)
        radius *= g_ControlHoldThreshold;

    void* centerObject = reinterpret_cast<uint8_t*>(self) + 0x30;
    auto* center =
        ((uint32_t*(__fastcall*)(void*, void*))
             CSF_AT(void**, centerObject, 0)[0x10 / 4])(
            centerObject, nullptr);

    self->field_0050 = center[0];
    CSF_AT(uint32_t, self, 0x54) = center[1];
    const uint32_t result = center[2];
    CSF_AT(uint32_t, self, 0x58) = result;
    CSF_AT(float, self, 0x5C) = radius;
    return static_cast<int>(result);
}

HOOK(0x006B2F90, C_00860288_UpdateBoundingSphere, "auto");
