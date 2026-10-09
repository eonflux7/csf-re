#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_UpdateRotationLimit, 0x004983C0, (void* self, void* edx, float a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Rotation_ApplyPitchDelta, 0x006AE690, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, Rotation_ApplyYawDelta, 0x006AE6B0, (void* self, void* edx, float a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_LayoutWidthAdjustment, 0x00833E44);
CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

using ObjectGetter = void* (__fastcall*)(void*, void*);
using IndexedObjectGetter = void* (__fastcall*)(void*, void*, int);
using IndexedIntGetter = int (__fastcall*)(void*, void*, int);
using MatrixGetter = std::uint32_t* (__fastcall*)(void*, void*, std::uint32_t*);
using AxisRotator = void (__fastcall*)(void*, void*, void*, float);
using ObjectSetter = void (__fastcall*)(void*, void*, void*);

static void __fastcall C_00833908_ApplyClampedRotation(
    C_00833908* self, void* /*edx*/, float* delta)
{
    float pitchDelta = delta[0] * CSF_AT(float, self, 0x390);
    float yawDelta = delta[1] * CSF_AT(float, self, 0x390);

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);
    void* referenced = GetReferencedObject(provider, nullptr);
    void* camera = reinterpret_cast<ObjectGetter>(
        CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);

    void* control = reinterpret_cast<void*>(CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        reinterpret_cast<int>(reinterpret_cast<std::uint8_t*>(self) + 0x1B4)));
    void* parent = CSF_AT(void*, control, 0x44);
    void* rotationTarget = nullptr;
    float yawLimit;
    float pitchLimit;

    if (parent) {
        void* limitHandle = reinterpret_cast<IndexedObjectGetter>(
            CSF_AT(void**, parent, 0)[0xC8 / 4])(parent, nullptr, 0);
        void* lookup = reinterpret_cast<ObjectGetter>(
            CSF_AT(void**, control, 0)[0x28 / 4])(control, nullptr);
        void* collection = reinterpret_cast<ObjectGetter>(
            CSF_AT(void**, control, 0)[0x28 / 4])(control, nullptr);
        void** lookupVtable = CSF_AT(void**, lookup, 0);
        void** collectionVtable = CSF_AT(void**, collection, 0);
        int index = reinterpret_cast<IndexedIntGetter>(
            lookupVtable[0x88 / 4])(lookup, nullptr, 203);
        rotationTarget = reinterpret_cast<IndexedObjectGetter>(
            collectionVtable[0x80 / 4])(collection, nullptr, index);

        if (rotationTarget) {
            C_00833908_UpdateRotationLimit(self, nullptr, yawDelta);
            void* limits = reinterpret_cast<void*>(
                Handle_GetValuePointer(limitHandle, nullptr));
            yawLimit = CSF_AT(float, limits, 0xF0);
            pitchLimit = CSF_AT(float, limits, 0xF4);
        } else {
            yawLimit = yawDelta;
            pitchLimit = yawDelta;
        }
    } else {
        yawLimit = g_LayoutWidthAdjustment;
        pitchLimit = g_LayoutWidthAdjustment;
    }

    if (pitchDelta + CSF_AT(float, self, 0x1B0) > pitchLimit)
        pitchDelta = pitchLimit - CSF_AT(float, self, 0x1B0);
    if (-pitchLimit > pitchDelta + CSF_AT(float, self, 0x1B0))
        pitchDelta = -(pitchLimit + CSF_AT(float, self, 0x1B0));
    if (yawDelta + CSF_AT(float, self, 0x1AC) > yawLimit)
        yawDelta = yawLimit - CSF_AT(float, self, 0x1AC);
    if (-yawLimit > yawDelta + CSF_AT(float, self, 0x1AC))
        yawDelta = -(yawLimit + CSF_AT(float, self, 0x1AC));

    if (pitchDelta != g_fZero || yawDelta != g_fZero) {
        Rotation_ApplyYawDelta(
            reinterpret_cast<std::uint8_t*>(self) + 0x1AC, nullptr, yawDelta);
        Rotation_ApplyPitchDelta(
            reinterpret_cast<std::uint8_t*>(self) + 0x1AC, nullptr, pitchDelta);
    }

    if (parent) {
        // Matrix-return wrappers store a pointer followed by an ownership byte.
        std::uint32_t matrixResult[0x1C / sizeof(std::uint32_t)] = {};

        reinterpret_cast<AxisRotator>(
            CSF_AT(void**, rotationTarget, 0)[0x48 / 4])(
                rotationTarget, nullptr, &g_AxisY, yawDelta);
        std::uint32_t* axisResult = reinterpret_cast<MatrixGetter>(
            CSF_AT(void**, rotationTarget, 0)[0x40 / 4])(
                rotationTarget, nullptr, matrixResult);
        void* axis = reinterpret_cast<void*>(*axisResult);
        reinterpret_cast<AxisRotator>(
            CSF_AT(void**, rotationTarget, 0)[0x48 / 4])(
                rotationTarget, nullptr, axis, pitchDelta);
        if (CSF_AT(std::uint8_t, matrixResult, 4))
            RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixResult[0]));

        if (CSF_AT(std::uint8_t, self, 0x3A5)) {
            void* collection = reinterpret_cast<ObjectGetter>(
                CSF_AT(void**, control, 0)[0x28 / 4])(control, nullptr);
            int index = reinterpret_cast<IndexedIntGetter>(
                CSF_AT(void**, collection, 0)[0x98 / 4])(
                    collection, nullptr, 3000);
            if (index != -1) {
                collection = reinterpret_cast<ObjectGetter>(
                    CSF_AT(void**, control, 0)[0x28 / 4])(control, nullptr);
                void* attachment = reinterpret_cast<IndexedObjectGetter>(
                    CSF_AT(void**, collection, 0)[0x8C / 4])(
                        collection, nullptr, index);
                void* transform = reinterpret_cast<ObjectGetter>(
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(attachment, nullptr);
                void** transformVtable = CSF_AT(void**, transform, 0);
                void** cameraVtable = CSF_AT(void**, camera, 0);
                void* orientation = reinterpret_cast<ObjectGetter>(
                    transformVtable[0x10 / 4])(transform, nullptr);
                reinterpret_cast<ObjectSetter>(cameraVtable[0x14 / 4])(
                    camera, nullptr, orientation);

                transform = reinterpret_cast<ObjectGetter>(
                    CSF_AT(void**, attachment, 0)[0x18 / 4])(attachment, nullptr);
                std::uint32_t* transformResult = reinterpret_cast<MatrixGetter>(
                    CSF_AT(void**, transform, 0)[0x8C / 4])(
                        transform, nullptr, matrixResult);
                void* position = reinterpret_cast<std::uint8_t*>(*transformResult) + 32;
                reinterpret_cast<ObjectSetter>(
                    CSF_AT(void**, camera, 0)[0x58 / 4])(
                        camera, nullptr, position);
                if (CSF_AT(std::uint8_t, matrixResult, 4))
                    RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixResult[0]));

                std::uint32_t* cameraResult = reinterpret_cast<MatrixGetter>(
                    CSF_AT(void**, camera, 0)[0x8C / 4])(
                        camera, nullptr, matrixResult);
                std::uint32_t* cameraPosition =
                    reinterpret_cast<std::uint32_t*>(*cameraResult + 32);
                CSF_AT(std::uint32_t, self, 0x3A8) = cameraPosition[0];
                CSF_AT(std::uint32_t, self, 0x3AC) = cameraPosition[1];
                CSF_AT(std::uint32_t, self, 0x3B0) = cameraPosition[2];
                if (CSF_AT(std::uint8_t, matrixResult, 4))
                    RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixResult[0]));
            }
        }
    }
}

HOOK(0x00498C90, C_00833908_ApplyClampedRotation, "auto");
