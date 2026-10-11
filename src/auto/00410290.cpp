#include <cstdint>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SpatialQuery_PositionsMatch, 0x004069B0, (void* self, void* edx, float* a2));
CSF_FUNC(float*, __cdecl, SpatialQuery_SelectHeight, 0x0040A640, (float* a1, float* a2));
CSF_FUNC(uint32_t*, __fastcall, SpatialQuery_InitializeQueryInfo, 0x0040E070, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, CharActionFsm_InitializeHitInfo, 0x0040ED90, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, SpatialQuery_BuildContext, 0x0040FDA0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(char, __fastcall, SpatialQuery_FindHeight, 0x0040FF70, (void* self, void* edx, int a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, C_0082dd78_DispatchReturnedObject, 0x0041E0C0, (C_0082dd78* self, void*));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(double, __fastcall, SceneObj_GetQueryRate, 0x0048E4F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);
CSF_GLOBAL(float, g_MinimumQueryHeight, 0x0082DF18);
CSF_GLOBAL(float, g_QueryIntervalScale, 0x0082DF1C);

static char __fastcall C_0082deb8_UpdateGroundQuery(
    C_0082deb8* self, void* /*edx*/)
{
    void* object = reinterpret_cast<void*>(self->field_0004);
    void* state =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);

    bool active = false;
    if (state) {
        active = !reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0xA8 / 4])(state, nullptr);
    }

    object = reinterpret_cast<void*>(self->field_0004);
    state = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr);

    if (CSF_AT(uint32_t, state, 0x78) == 0xBF800000) {
        self->field_0034 = 0xBF800000;
        return static_cast<char>(reinterpret_cast<uintptr_t>(state));
    }

    if (self->field_0034 == 0xBF800000)
        self->field_003c = 1;

    if (!active || !self->field_003d) {
        self->field_0034 = 0xBF800000;
    } else {
        // The two small interface objects precede the shared hit-result storage.
        struct QueryScratch {
            uint32_t hitInfo;
            uint32_t queryInfo;
            float result[25];
        } scratch{};

        float position[4]{};
        auto* source = reinterpret_cast<float*>(GetObjectPositionVector(
            reinterpret_cast<void*>(self->field_0004), nullptr));
        std::memcpy(position, source, 12);
        const float originalX = position[0];

        if (self->field_0034 != 0xBF800000 &&
            static_cast<uint8_t>(SpatialQuery_PositionsMatch(
                position, nullptr, &self->field_0018)) &&
            !self->field_003e) {
            return 0;
        }

        SpatialQuery_InitializeQueryInfo(&scratch.queryInfo, nullptr);
        void* global = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));
        void* collection =
            reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, controller, 0)[0x3C / 4])(controller, nullptr);
        CSF_AT(void*, scratch.result, 0x60) = collection;

        const uint32_t heightOverride = self->field_0038;
        self->field_003e = 0;
        if (heightOverride != 0xBF800000)
            std::memcpy(&position[1], &heightOverride, sizeof(float));

        object = reinterpret_cast<void*>(self->field_0004);
        void* objectData = CSF_AT(void*, object, 0x2C);
        float direction[3] = {
            position[0],
            position[1] + CSF_AT(float, objectData, 0xE4),
            position[2]
        };

        float startX;
        float startZ;
        if (self->vIsSet() ||
            static_cast<uint8_t>(SceneObj_IsInactive(
                reinterpret_cast<void*>(self->field_0004), nullptr))) {
            startX = direction[0];
            startZ = direction[2];
        } else {
            startX = originalX;
            startZ = position[2];
            direction[1] = position[1];
            const double extent =
                reinterpret_cast<double(__fastcall*)(void*, void*)>(
                    self->vtbl[0x5C / 4])(self, nullptr);
            direction[1] = static_cast<float>(extent + position[1]);
        }

        const double extent =
            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                self->vtbl[0x5C / 4])(self, nullptr);

        alignas(4) uint32_t endpoint[4]{};
        float endpointY = static_cast<float>(position[1] - extent);
        std::memcpy(&endpoint[0], &position[0], 4);
        std::memcpy(&endpoint[1], &endpointY, 4);
        std::memcpy(&endpoint[2], &position[2], 4);

        const double rate = SceneObj_GetQueryRate(
            reinterpret_cast<void*>(self->field_0004), nullptr);
        self->field_0080 = rate > g_fZero
            ? static_cast<uint32_t>(
                static_cast<int64_t>(g_QueryIntervalScale / rate))
            : static_cast<uint32_t>(-1);

        float segment[6] = {
            startX, direction[1], startZ,
            position[0], endpointY, position[2]
        };
        float queriedHeight = -100000000.0f;

        if (CSF_AT(uint8_t,
                   reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0)) {
            const float dx = segment[3] - segment[0];
            const float dy = segment[4] - segment[1];
            const float dz = segment[5] - segment[2];
            const float inverseLength =
                g_TransitionOne / std::sqrt(dx * dx + dy * dy + dz * dz);
            direction[0] = dx * inverseLength;
            direction[1] = dy * inverseLength;
            direction[2] = dz * inverseLength;

            uint32_t context[3]{};
            SpatialQuery_BuildContext(
                context, nullptr, segment, reinterpret_cast<int>(direction));
            endpoint[1] = 0;
            if (SpatialQuery_FindHeight(
                    self, nullptr, reinterpret_cast<int>(context), endpoint)) {
                std::memcpy(&queriedHeight, &endpoint[1], 4);
            }
        }

        void* hit = reinterpret_cast<void*(__fastcall*)(
            void*, void*, uint8_t*, float*, uint32_t*, int)>(
                CSF_AT(void**, collection, 0)[0x34 / 4])(
                    collection, nullptr, &self->field_0044,
                    segment, &self->field_0030, 3);

        if (hit) {
            CharActionFsm_InitializeHitInfo(&scratch.hitInfo, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, hit, 0)[0x0C / 4])(
                    hit, nullptr, &scratch.hitInfo);

            std::memcpy(&self->field_0044,
                        reinterpret_cast<uint8_t*>(scratch.result) + 0x20,
                        0x40);

            if (CSF_AT(uint8_t,
                       reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0)) {
                source = reinterpret_cast<float*>(GetObjectPositionVector(
                    reinterpret_cast<void*>(self->field_0004), nullptr));
                std::memcpy(endpoint, source, 12);
                float* selectedHeight =
                    SpatialQuery_SelectHeight(scratch.result, &queriedHeight);
                std::memcpy(&endpoint[1], selectedHeight, 4);
                SceneLoader_ConfigureFocusEntry(
                    reinterpret_cast<void*>(self->field_0004), nullptr,
                    reinterpret_cast<int*>(endpoint));
            } else {
                std::memcpy(&self->field_0034, &scratch.result[0], 4);
            }

            object = reinterpret_cast<void*>(self->field_0004);
            reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, object, 0)[0x2C / 4])(
                    object, nullptr,
                    CSF_AT(uint32_t, scratch.result, 0x18));
            self->field_0040 = CSF_AT(uint32_t, scratch.result, 0x14);

            void* returnedObject = reinterpret_cast<void*>(
                C_0082dd78_DispatchReturnedObject(
                    reinterpret_cast<C_0082dd78*>(self), nullptr));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, returnedObject, 0)[0x28 / 4])(
                    returnedObject, nullptr);
        } else if (
            CSF_AT(uint8_t,
                   reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) &&
            queriedHeight > g_MinimumQueryHeight) {
            source = reinterpret_cast<float*>(GetObjectPositionVector(
                reinterpret_cast<void*>(self->field_0004), nullptr));
            std::memcpy(endpoint, source, 12);
            std::memcpy(&endpoint[1], &queriedHeight, 4);
            SceneLoader_ConfigureFocusEntry(
                reinterpret_cast<void*>(self->field_0004), nullptr,
                reinterpret_cast<int*>(endpoint));
        } else {
            self->field_0034 = 0xBF800000;
            self->field_003c = 1;
            self->field_0040 = static_cast<uint32_t>(-1);
        }
    }

    auto* position = reinterpret_cast<uint32_t*>(GetObjectPositionVector(
        reinterpret_cast<void*>(self->field_0004), nullptr));
    std::memcpy(&self->field_0018, &position[0], 4);
    std::memcpy(&self->field_001c, &position[1], 4);
    const uint32_t z = position[2];
    std::memcpy(&self->field_0020, &z, 4);
    return static_cast<char>(z);
}

HOOK(0x00410290, C_0082deb8_UpdateGroundQuery, "auto");

