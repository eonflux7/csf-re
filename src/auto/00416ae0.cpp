#include <cstdint>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, S_00526d40_TransformVector3, 0x004069F0, (S_00526d40* self, void* , uint32_t* out, int* in));
CSF_FUNC(void, __cdecl, ClampFloatToBounds, 0x00406B60, (float* value, float* lowerBound, float* upperBound));
CSF_FUNC(void, __fastcall, C_0082e090_ApplyCollisionResponse, 0x00416980, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, SceneObject_GetDrawState, 0x0049CF10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(int, __fastcall, GfxMatrix_CreateQuaternionMatrix, 0x006AE6F0, (float* self, void* , int outputAddress));
CSF_FUNC(float*, __fastcall, GfxMatrix_SetQuaternionFromAxisAngle, 0x006AE8C0, (float* self, void* , S_00526d40* axis, float angle));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_RollOscillationScale, 0x0082DC6C);
CSF_GLOBAL(float, g_CollisionVerticalScale, 0x0082E08C);

static void __fastcall C_0082e090_HandleCollision(
    C_0082e090* self, void* /*edx*/,
    const float* displacement, uint32_t* collision, int /*unused*/)
{
    self->field_0098 = 1;

    bool respond = true;
    void* hitObject = CSF_AT(void*, collision, 0x4C);
    if (hitObject) {
        const int entry = CSF_AT(int, hitObject, 0x50);
        if (static_cast<uint8_t>(GameConfig_IsResolvedEntryActive(entry)) == 0
            && entry != 12) {
            respond = self->field_0076 == 0;
            self->field_0076 = 1;
        }
    }

    alignas(8) uint8_t positionBytes[0xC];
    float* position = reinterpret_cast<float*>(positionBytes);

    if (respond) {
        self->field_0084 *= g_MissingMetricValue;
        self->field_0088 *= g_MissingMetricValue;
        CSF_AT(float, self, 0x8C) *= g_MissingMetricValue;

        alignas(8) uint8_t quaternionBytes[0x10];
        alignas(8) uint8_t transformedBytes[0xC];
        float* quaternion = reinterpret_cast<float*>(quaternionBytes);

        GfxMatrix_SetQuaternionFromAxisAngle(
            quaternion, nullptr,
            reinterpret_cast<S_00526d40*>(
                reinterpret_cast<uint8_t*>(collision) + 0x2C),
            3.1415927f);

        // The matrix output occupies the final two words of the position buffer.
        const int matrix = GfxMatrix_CreateQuaternionMatrix(
            quaternion, nullptr, reinterpret_cast<int>(positionBytes + 4));
        uint32_t* transformed = S_00526d40_TransformVector3(
            reinterpret_cast<S_00526d40*>(&self->field_0084), nullptr,
            reinterpret_cast<uint32_t*>(transformedBytes),
            reinterpret_cast<int*>(matrix));

        self->field_0084 = CSF_AT(float, transformed, 0);
        self->field_0088 = CSF_AT(float, transformed, 4);
        CSF_AT(float, self, 0x8C) = CSF_AT(float, transformed, 8);

        if (CSF_AT(uint8_t, positionBytes, 8)) {
            RwMatrixDestroy(CSF_AT(uint32_t*, positionBytes, 4));
        }

        const int drawState = SceneObject_GetDrawState(
            reinterpret_cast<void*>(self->field_0004), nullptr);
        float attenuation = CSF_AT(float, reinterpret_cast<void*>(drawState), 0x1B4);
        float lowerBound = 0.0f;
        float upperBound = 1.0f;
        ClampFloatToBounds(&attenuation, &lowerBound, &upperBound);

        const float reductionX = attenuation * self->field_0084;
        const float reductionY = attenuation * self->field_0088;
        const float reductionZ = attenuation * CSF_AT(float, self, 0x8C);
        self->field_0084 -= reductionX;
        self->field_0088 -= reductionY;
        CSF_AT(float, self, 0x8C) -= reductionZ;

        position[0] = CSF_AT(float, collision, 0x2C)
                    + CSF_AT(float, collision, 0x08);
        position[1] = CSF_AT(float, collision, 0x30)
                    + CSF_AT(float, collision, 0x0C);
        position[2] = CSF_AT(float, collision, 0x34)
                    + CSF_AT(float, collision, 0x10);
        SceneLoader_ConfigureFocusEntry(
            reinterpret_cast<void*>(self->field_0004), nullptr,
            reinterpret_cast<int*>(positionBytes));

        void* returnedObject = C_0082dd78_DispatchReturnedObject(self, nullptr);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, returnedObject, 0)[0x28 / 4])(
                returnedObject, nullptr);

        C_0082e090_ApplyCollisionResponse(self, nullptr, collision);

        const float verticalResponse = self->field_0088 * g_CollisionVerticalScale;
        self->field_00b8 = verticalResponse;
        if (std::fabs(verticalResponse) < g_RollOscillationScale
            && !CSF_AT(uint32_t, collision, 0x4C)
            && std::fabs(CSF_AT(float, collision, 0x2C))
                < CSF_AT(float, collision, 0x30)
            && std::fabs(CSF_AT(float, collision, 0x34))
                < CSF_AT(float, collision, 0x30)) {
            self->field_00b8 = -0.050000001f;
        }
    } else {
        uint32_t* currentPosition = GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr);
        position[0] = CSF_AT(float, currentPosition, 0);
        position[1] = CSF_AT(float, currentPosition, 4);
        position[2] = CSF_AT(float, currentPosition, 8);

        position[0] += displacement[0];
        position[1] += displacement[1];
        position[2] += displacement[2];
        SceneLoader_ConfigureFocusEntry(
            reinterpret_cast<void*>(self->field_0004), nullptr,
            reinterpret_cast<int*>(positionBytes));

        void* returnedObject = C_0082dd78_DispatchReturnedObject(self, nullptr);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, returnedObject, 0)[0x28 / 4])(
                returnedObject, nullptr);
    }
}

HOOK(0x00416AE0, C_0082e090_HandleCollision, "auto");
