#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationDefinition, 0x004F07F0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, SceneMissionSetup_EvaluateAnimationTime, 0x0050DD80,
         (void* self, void* edx, float a2));
CSF_FUNC(int, __fastcall, SceneSubsystem_LookupEntry, 0x00519AF0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, SceneMissionSetup_CollectAnimatedSceneObjects, 0x006CCD70,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120,
         (void* self, void* edx, int a2));

static int __fastcall SceneMissionSetup_ApplySceneAnimations(
    void* self, void* /*edx*/)
{
    auto* controller = reinterpret_cast<C_00860bb8*>(
        SceneLoader_GetSceneController(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

    const auto definitionCount = static_cast<std::uint32_t>(
        reinterpret_cast<std::uintptr_t>(
            GetCachedObjectProvider(CSF_AT(void*, self, 0x1FE0), nullptr)));

    for (std::uint32_t index = 0; index < definitionCount; ++index)
    {
        void* definition = reinterpret_cast<void*>(
            SceneMissionSetup_GetAnimationDefinition(
                CSF_AT(void*, self, 0x1FE0), nullptr,
                static_cast<int>(index)));

        if (CSF_AT(std::uint32_t, definition, 0x14) == 0)
            continue;

        alignas(4) std::uint8_t matches[0x28] = {};
        CSF_AT(int, matches, 0x0C) = 10;
        controller->vSlot7C(definition, matches);

        if (CSF_AT(std::uint32_t, matches, 0x08) != 0)
        {
            float startTime = 0.0f;
            if (CSF_AT(int, definition, 0x20) == 1)
            {
                const float timeArgument = CSF_AT(float, definition, 0x24);
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                startTime = static_cast<float>(
                    SceneMissionSetup_EvaluateAnimationTime(
                        state, nullptr, timeArgument));
            }

            void* subsystem = CSF_AT(void*, self, 0x1FC4);
            const int entryIndex =
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, subsystem, 0)[0x08 / 4])(
                    subsystem, nullptr, CSF_AT(int, definition, 0x1C));

            if (entryIndex != -1)
            {
                const int animationEntry = SceneSubsystem_LookupEntry(
                    CSF_AT(void*, self, 0x1FC4), nullptr, entryIndex);

                if (animationEntry != -1)
                {
                    void* entry = reinterpret_cast<void*>(
                        SceneMissionSetup_GetAnimationEntry(
                            CSF_AT(void*, self, 0x1FC4), nullptr, entryIndex));

                    alignas(4) std::uint8_t parameters[0x20] = {};
                    CSF_AT(float, parameters, 0x10) = 1.0f;
                    CSF_AT(float, parameters, 0x14) = 1.0f;
                    CSF_AT(float, parameters, 0x18) = 1.0f;
                    CSF_AT(std::uint8_t, parameters, 0x1D) = 1;

                    CSF_AT(std::uint8_t, parameters, 0x0C) =
                        CSF_AT(std::uint8_t, entry, 0x58);
                    CSF_AT(std::uint8_t, parameters, 0x0E) =
                        CSF_AT(std::uint8_t, entry, 0x5A);
                    CSF_AT(std::uint8_t, parameters, 0x0D) =
                        CSF_AT(std::uint8_t, entry, 0x59);
                    CSF_AT(std::uint8_t, parameters, 0x1C) = 0;
                    CSF_AT(std::uint8_t, parameters, 0x1D) = 0;
                    CSF_AT(float, parameters, 0x00) = startTime;
                    CSF_AT(int, parameters, 0x04) = CSF_AT(int, entry, 0x34);
                    CSF_AT(int, parameters, 0x08) = 0;

                    std::uint32_t matchIndex = 0;
                    if (CSF_AT(std::uint32_t, matches, 0x08) != 0)
                    {
                        do
                        {
                            void* object = reinterpret_cast<void*>(
                                controller->vSlot70(
                                    CSF_AT(int*, matches, 0)[matchIndex]));

                            if (CSF_AT(int, definition, 0x20) == 2)
                            {
                                const float timeArgument =
                                    CSF_AT(float, definition, 0x24);
                                void* state = reinterpret_cast<void*>(
                                    GetGlobalState_50D190());
                                CSF_AT(float, parameters, 0x00) =
                                    static_cast<float>(
                                        SceneMissionSetup_EvaluateAnimationTime(
                                            state, nullptr, timeArgument));
                            }

                            reinterpret_cast<void(__fastcall*)(
                                void*, void*, int, void*)>(
                                CSF_AT(void**, object, 0)[0x40 / 4])(
                                    object, nullptr, animationEntry, parameters);

                            ++matchIndex;
                        }
                        while (matchIndex <
                               CSF_AT(std::uint32_t, matches, 0x08));
                    }
                }
            }
        }

        if (CSF_AT(void*, matches, 0) != nullptr)
        {
            Mem_Free(CSF_AT(void*, matches, 0));
            CSF_AT(void*, matches, 0) = nullptr;
        }
    }

    return SceneMissionSetup_CollectAnimatedSceneObjects(controller, nullptr);
}

HOOK(0x004E9370, SceneMissionSetup_ApplySceneAnimations, "auto");
