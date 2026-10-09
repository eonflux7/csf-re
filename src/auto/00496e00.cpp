#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetControlEntryByIndex, 0x0049ED10, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_GetControlFlagsAddress, 0x0049ED30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_008342e8_CopyControlPosition, 0x0049ED80, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(double, __fastcall, CalculateSquaredVectorLength, 0x0068E300, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Bounds_ContainsPoint, 0x006AC9A0, (void* self, void* edx, float* a2));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_HalfDurationScale, 0x0082D444);
CSF_GLOBAL(float, g_SearchBoundsPadding, 0x00833E20);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_008342e8* __fastcall CPlayerCharacter_FindNearestEligibleControl(
    CPlayerCharacter* self, void* /*edx*/, unsigned int* controlIndex)
{
    if (static_cast<int32_t>(self->field_0248) < 0)
        return nullptr;

    auto* target = reinterpret_cast<C_008342e8*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(&self->field_0240)));
    if (!target)
        return nullptr;

    if (static_cast<uint8_t>(
            IsResolvedEntryActive(static_cast<int>(target->field_0050))) == 0)
        return nullptr;

    void* firstSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    void* secondSettings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());

    // The last three words share the original position-output buffer.
    uint32_t candidateData[6] = {};
    CSF_AT(float, candidateData, 0x14) =
        CSF_AT(float, secondSettings, 0xB18) *
        CSF_AT(float, firstSettings, 0xB18);

    CSF_AT(float, candidateData, 0x04) =
        CSF_AT(float, self, 0x1D4) * g_HalfDurationScale;

    void* position = reinterpret_cast<void*>(
        GetObjectPositionVector(self, nullptr));
    float origin[3];
    origin[0] = CSF_AT(float, position, 0x00);
    origin[1] = CSF_AT(float, candidateData, 0x04) +
                CSF_AT(float, position, 0x04);
    origin[2] = CSF_AT(float, position, 0x08);

    GetObjectPositionVector(target, nullptr);
    void* dispatched = reinterpret_cast<void*>(
        C_0082dd78_DispatchReturnedObject(
            reinterpret_cast<C_0082dd78*>(target->field_004c), nullptr));

    using GetObject = void* (__fastcall*)(void*, void*);
    using FillBounds = void (__fastcall*)(void*, void*, void*);
    void* boundsSource = reinterpret_cast<GetObject>(
        CSF_AT(void**, dispatched, 0)[0x34 / 4])(dispatched, nullptr);

    alignas(4) uint8_t bounds[0x3C] = {};
    reinterpret_cast<FillBounds>(
        CSF_AT(void**, boundsSource, 0)[0x1C / 4])(
            boundsSource, nullptr, bounds);

    CSF_AT(float, candidateData, 0x00) =
        CSF_AT(float, bounds, 0x0C) + g_SearchBoundsPadding;
    CSF_AT(float, bounds, 0x0C) = CSF_AT(float, candidateData, 0x00);
    CSF_AT(float, candidateData, 0x04) =
        CSF_AT(float, bounds, 0x10) + g_SearchBoundsPadding;
    CSF_AT(float, bounds, 0x10) = CSF_AT(float, candidateData, 0x04);
    CSF_AT(float, candidateData, 0x08) =
        CSF_AT(float, bounds, 0x14) + g_SearchBoundsPadding;
    CSF_AT(float, bounds, 0x14) = CSF_AT(float, candidateData, 0x08);

    if (static_cast<uint8_t>(SceneObj_IsInactive(target, nullptr)) != 0)
        return nullptr;
    if (!Bounds_ContainsPoint(bounds, nullptr, origin))
        return nullptr;
    if (!target->field_01a0)
        return nullptr;

    using FindByHeight = int (__fastcall*)(void*, void*, float);
    using GetIndexedObject = void* (__fastcall*)(void*, void*, int);

    uint32_t index = 0;
    do
    {
        void* entry = reinterpret_cast<void*>(
            GetControlEntryByIndex(target, nullptr, static_cast<int>(index)));
        if (CSF_AT(int32_t, entry, 0x08) < 0)
        {
            void* flags = reinterpret_cast<void*>(
                C_008342e8_GetControlFlagsAddress(
                    target, nullptr, static_cast<int>(index)));
            if ((CSF_AT(uint8_t, flags, 0) & 4) != 0)
            {
                C_008342e8_CopyControlPosition(
                    target, nullptr, static_cast<int>(index), candidateData);

                void* controller = reinterpret_cast<void*>(
                    target->vGetField016c());
                int objectIndex = reinterpret_cast<FindByHeight>(
                    CSF_AT(void**, controller, 0)[0x98 / 4])(
                        controller, nullptr,
                        CSF_AT(float, candidateData, 0x04));
                if (objectIndex != -1)
                {
                    controller = reinterpret_cast<void*>(
                        target->vGetField016c());
                    void* object = reinterpret_cast<GetIndexedObject>(
                        CSF_AT(void**, controller, 0)[0x8C / 4])(
                            controller, nullptr, objectIndex);
                    void* inner = reinterpret_cast<GetObject>(
                        CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
                    float* objectPosition = reinterpret_cast<float*>(
                        reinterpret_cast<GetObject>(
                            CSF_AT(void**, inner, 0)[0x10 / 4])(inner, nullptr));

                    float displacement[3];
                    float* vector = BuildFacingVectorBetween(
                        origin, nullptr, displacement, objectPosition);
                    double distanceSquared =
                        CalculateSquaredVectorLength(vector, nullptr);
                    if (distanceSquared < CSF_AT(float, candidateData, 0x14))
                    {
                        CSF_AT(float, candidateData, 0x14) =
                            static_cast<float>(distanceSquared);
                        CSF_AT(C_008342e8*, candidateData, 0x10) = target;
                        candidateData[3] = index;
                    }
                }
            }
        }
        ++index;
    } while (index < target->field_01a0);

    C_008342e8* nearest = CSF_AT(C_008342e8*, candidateData, 0x10);
    if (nearest && controlIndex)
        *controlIndex = candidateData[3];
    return nearest;
}

HOOK(0x00496E00, CPlayerCharacter_FindNearestEligibleControl, "auto");
