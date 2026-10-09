#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtIntersectionLineTriangle, 0x00620640,
         (float* a1, float* a2, float* a3, float* a4, float* a5, float* a6));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall S_004fe580_ProcessTransitionCrossing(
    S_004fe580* self, void* /*edx*/, float* position)
{
    float previous[3] = {
        CSF_AT(float, self, 0x4C),
        CSF_AT(float, self, 0x50),
        CSF_AT(float, self, 0x54)
    };

    CSF_AT(float, self, 0x4C) = position[0];
    CSF_AT(float, self, 0x50) = position[1];
    CSF_AT(float, self, 0x54) = position[2];

    if (!self->field_0058) {
        self->field_0058 = 1;
        previous[0] = position[0];
        previous[1] = position[1];
        previous[2] = position[2];
    }

    float start[3] = {previous[0], previous[1], previous[2]};
    float forward[9] = {
        position[0] - previous[0],
        position[1] - previous[1],
        position[2] - previous[2]
    };
    float end[3] = {position[0], position[1], position[2]};
    float reverse[3] = {
        previous[0] - position[0],
        previous[1] - position[1],
        previous[2] - position[2]
    };
    float intersectionFraction = 0.0f;

    if (!self->field_0044)
        return 0;

    std::uint32_t index = 0;
    std::uint32_t offset = 0;
    for (;;) {
        auto* transition =
            reinterpret_cast<std::uint8_t*>(self->field_003c) + offset;

        if (CSF_AT(std::uint8_t, transition, 0)) {
            auto* vertex0 = reinterpret_cast<float*>(transition + 8);
            auto* vertex1 = reinterpret_cast<float*>(transition + 20);
            auto* vertex2 = reinterpret_cast<float*>(transition + 32);
            auto* vertex3 = reinterpret_cast<float*>(transition + 44);

            if (RtIntersectionLineTriangle(
                    start, forward, vertex0, vertex1, vertex2,
                    &intersectionFraction) ||
                RtIntersectionLineTriangle(
                    end, reverse, vertex0, vertex1, vertex2,
                    &intersectionFraction) ||
                RtIntersectionLineTriangle(
                    start, forward, vertex0, vertex2, vertex3,
                    &intersectionFraction) ||
                RtIntersectionLineTriangle(
                    end, reverse, vertex0, vertex2, vertex3,
                    &intersectionFraction)) {
                void* globalState = reinterpret_cast<void*>(GetGlobalInt());
                void* controller = reinterpret_cast<void*>(
                    SceneLoader_GetSceneController(globalState, nullptr));

                reinterpret_cast<void (__fastcall*)(
                    void*, void*, std::uint32_t)>(
                    CSF_AT(void**, controller, 0)[148 / 4])(
                        controller, nullptr,
                        CSF_AT(std::uint32_t, transition, 60));

                return reinterpret_cast<int>(transition);
            }
        }

        ++index;
        offset += 68;
        if (!(index < self->field_0044))
            return 0;
    }
}

HOOK(0x004FE580, S_004fe580_ProcessTransitionCrossing, "auto");
