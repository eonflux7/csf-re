#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_MaxPercent, 0x0082E120);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, GfxMatrix_AreVectorsDifferent, 0x0040FB20, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, AiTaskStates_UpdatePathTracking, 0x0041EFD0, (void* self, void* edx, uint32_t* a2, void* a3));
CSF_FUNC(int*, __fastcall, AiTaskStates_ApplyNavigationMovement, 0x00422A40, (void* self, void* edx, int* arg0, char a3));
CSF_FUNC(int, __cdecl, GetGlobalNestedField14, 0x0044F3D0, ());
CSF_FUNC(int, __fastcall, GuiControl_GetPathPointRecord, 0x004A0F30, (void* self, void* edx, int a2));
CSF_FUNC(double, __cdecl, RwV3dLength, 0x00623360, (float* a1));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Navigation_SetDestination, 0x0075A340, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, Navigation_FindPath, 0x0075FC40, (void* self, void* edx, float* arg0, float* arg4, void* a4, uint32_t* a5));

static int* __fastcall C_0082f9f8_UpdateNavigation(
    C_0082f9f8* self, void* /*edx*/, float* destination, char movementFlag)
{
    auto* state = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint8_t, self, 0x7C) = 0;

    if (CSF_AT(uint8_t, state, 0x274))
    {
        float displacement[4];
        displacement[2] = CSF_AT(float, state, 0x220) - CSF_AT(float, state, 0x280);
        displacement[1] = CSF_AT(float, state, 0x21C) - CSF_AT(float, state, 0x27C);
        displacement[0] = CSF_AT(float, state, 0x218) - CSF_AT(float, state, 0x278);
        const bool nearby = RwV3dLength(displacement) < g_MaxPercent;
        CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x274) = nearby;
    }

    state = reinterpret_cast<uint8_t*>(self->field_0008);
    if (!CSF_AT(uint8_t, state, 0x274))
    {
        CSF_AT(uint32_t, state, 0x284) = CSF_AT(uint32_t, state, 0x20C);
        CSF_AT(uint32_t, state, 0x288) = CSF_AT(uint32_t, state, 0x210);
        CSF_AT(uint32_t, state, 0x28C) = CSF_AT(uint32_t, state, 0x214);

        state = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(uint32_t, state, 0x290) = CSF_AT(uint32_t, state, 0x20C);
        CSF_AT(uint32_t, state, 0x294) = CSF_AT(uint32_t, state, 0x210);
        CSF_AT(uint32_t, state, 0x298) = CSF_AT(uint32_t, state, 0x214);

        state = reinterpret_cast<uint8_t*>(self->field_0008);
        (void)g_pCurrentScene;
        void* navigationService = reinterpret_cast<void*>(GetGlobalNestedField14());
        CSF_AT(char, self, 0x7C) = Navigation_FindPath(
            navigationService, nullptr, reinterpret_cast<float*>(state + 0x218),
            destination, state + 0x25C, reinterpret_cast<uint32_t*>(state + 0x208));
    }
    else
    {
        (void)g_pCurrentScene;
        void* navigationService = reinterpret_cast<void*>(GetGlobalNestedField14());
        const char foundPath = Navigation_FindPath(
            navigationService, nullptr, reinterpret_cast<float*>(state + 0x284),
            destination, state + 0x25C, reinterpret_cast<uint32_t*>(state + 0x208));
        CSF_AT(char, self, 0x7C) = foundPath;

        if (foundPath)
        {
            if (GetCachedObjectProvider(
                    reinterpret_cast<uint8_t*>(self->field_0008) + 0x25C, nullptr))
            {
                auto* pathPoint = reinterpret_cast<uint8_t*>(GuiControl_GetPathPointRecord(
                    reinterpret_cast<uint8_t*>(self->field_0008) + 0x25C, nullptr, 0));
                state = reinterpret_cast<uint8_t*>(self->field_0008);

                if (static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
                        state + 0x218, nullptr, reinterpret_cast<float*>(state + 0x284))) &&
                    static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
                        state + 0x284, nullptr, reinterpret_cast<float*>(state + 0x290))) &&
                    static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
                        pathPoint, nullptr, reinterpret_cast<float*>(state + 0x290))))
                {
                    Navigation_SetDestination(
                        state + 0x25C, nullptr, reinterpret_cast<uint32_t*>(state + 0x284));
                }

                state = reinterpret_cast<uint8_t*>(self->field_0008);
                CSF_AT(uint32_t, state, 0x290) = CSF_AT(uint32_t, pathPoint, 0);
                CSF_AT(uint32_t, state, 0x294) = CSF_AT(uint32_t, pathPoint, 4);
                CSF_AT(uint32_t, state, 0x298) = CSF_AT(uint32_t, pathPoint, 8);
            }
            else
            {
                state = reinterpret_cast<uint8_t*>(self->field_0008);
                if (static_cast<uint8_t>(GfxMatrix_AreVectorsDifferent(
                        state + 0x218, nullptr, reinterpret_cast<float*>(state + 0x284))))
                {
                    Navigation_SetDestination(
                        state + 0x25C, nullptr, reinterpret_cast<uint32_t*>(state + 0x284));

                    state = reinterpret_cast<uint8_t*>(self->field_0008);
                    CSF_AT(uint32_t, state, 0x290) = CSF_AT(uint32_t, state, 0x284);
                    CSF_AT(uint32_t, state, 0x294) = CSF_AT(uint32_t, state, 0x288);
                    CSF_AT(uint32_t, state, 0x298) = CSF_AT(uint32_t, state, 0x28C);
                }
            }
        }
    }

    state = reinterpret_cast<uint8_t*>(self->field_0008);
    AiTaskStates_UpdatePathTracking(
        state + 0x29C, nullptr, reinterpret_cast<uint32_t*>(state + 0x218), state + 0x25C);

    CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0008), 0x274) = 1;
    state = reinterpret_cast<uint8_t*>(self->field_0008);
    CSF_AT(uint32_t, state, 0x278) = CSF_AT(uint32_t, state, 0x218);
    CSF_AT(uint32_t, state, 0x27C) = CSF_AT(uint32_t, state, 0x21C);
    CSF_AT(uint32_t, state, 0x280) = CSF_AT(uint32_t, state, 0x220);

    return AiTaskStates_ApplyNavigationMovement(
        self, nullptr, reinterpret_cast<int*>(destination), movementFlag);
}

HOOK(0x00422BF0, C_0082f9f8_UpdateNavigation, "auto");
