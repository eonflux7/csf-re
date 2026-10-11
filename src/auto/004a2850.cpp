#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, BuildContactFrame, 0x004020A0, (void* self, void* edx, uint32_t* a2, uint32_t* a3));
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, S_004503E0_ForwardWithScratch, 0x00452210, (S_004503e0* self, void* , C_008342e8* a2, float a3));
CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(uint32_t*, __stdcall, HudAlertIcons_RefreshEntryParticle, 0x0049FD30, (int a2));
CSF_FUNC(int, __fastcall, C_008343a4_FindMatchingEntry, 0x004A0080, (C_008343a4* self, void* , void* referenceObject));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxMatrix_ClassifyPointAgainstPlane, 0x006AD7B0, (float* self, void* , float* point));
CSF_FUNC(float*, __fastcall, GfxMatrix_EulerAnglesToDirection, 0x006AE640, (float* self, void* , float* out));
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));
CSF_FUNC(int, __cdecl, GameConfig_IsStateCodeAllowed, 0x00755810, (std::uint32_t stateCode));

CSF_GLOBAL(float, g_QuarterTurnConstant, 0x0082FAD4);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall HudAlertIcons_UpdateVisibleEntries(
    C_008343a4** self, void* /*edx*/)
{
    void* firstElement = reinterpret_cast<void*>(
        GetFirstElement(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);
    void* projection = GetReferencedObject(provider, nullptr);
    auto* context = reinterpret_cast<C_008342e8*>(
        GetSceneMenuContext(g_pCurrentScene, nullptr));

    alignas(8) uint8_t frameBytes[0x2C];
    alignas(8) uint8_t targetVectorBytes[0xC];
    alignas(8) uint8_t directionBytes[0xC];
    alignas(8) uint8_t orientationBytes[0x28];
    alignas(8) uint8_t screenBytes[0x8];
    alignas(8) uint8_t positionBytes[0x14];
    alignas(8) uint8_t colorBytes[0x14];
    alignas(8) uint8_t iconPositionBytes[0x18];

    auto* position = reinterpret_cast<float*>(positionBytes);
    auto* screen = reinterpret_cast<float*>(screenBytes);
    auto* iconPosition = reinterpret_cast<int*>(iconPositionBytes);
    [[maybe_unused]] auto* color = reinterpret_cast<float*>(colorBytes);

    uint32_t index = 0;
    uint32_t count = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
    if (count == 0)
        return 0;

    do
    {
        auto* candidates = reinterpret_cast<C_008342e8**>(
            CSF_AT(uint32_t, g_pCurrentScene, 0x20));
        C_008342e8* candidate = candidates[index];

        do
        {
            if (!candidate)
                break;
            if (!static_cast<uint8_t>(
                    GameConfig_IsStateCodeAllowed(candidate->field_0050)))
                break;

            int entryAddress = C_008343a4_FindMatchingEntry(
                *self, nullptr, &candidate->field_001c);
            if (!entryAddress)
                break;
            void* entry = reinterpret_cast<void*>(entryAddress);

            uint32_t* sourcePosition = GetObjectPositionVector(candidate, nullptr);
            CSF_AT(uint32_t, positionBytes, 0) = sourcePosition[0];
            CSF_AT(uint32_t, positionBytes, 4) = sourcePosition[1];
            CSF_AT(uint32_t, positionBytes, 8) = sourcePosition[2];

            if (!S_004503E0_ForwardWithScratch(
                    reinterpret_cast<S_004503e0*>(g_pCurrentScene),
                    nullptr, candidate, 1000000.0f))
                break;

            if (!CSF_AT(uint8_t, context, 0x228))
            {
                // The direction output is consumed by the following call,
                // not by the virtual orientation getter.
                float* angles =
                    reinterpret_cast<float*(__fastcall*)(void*, void*, void*)>(
                        context->vtbl[0x38 / 4])(
                            context, nullptr, orientationBytes);
                float* direction = GfxMatrix_EulerAnglesToDirection(
                    angles, nullptr, reinterpret_cast<float*>(directionBytes));
                uint32_t* origin = GetObjectPositionVector(context, nullptr);
                BuildContactFrame(
                    frameBytes, nullptr, origin,
                    reinterpret_cast<uint32_t*>(direction));
                if (GfxMatrix_ClassifyPointAgainstPlane(
                        reinterpret_cast<float*>(frameBytes), nullptr, position))
                    break;
            }

            void* blueConfig = reinterpret_cast<void*>(GetGlobalValue_558150());
            void* greenConfig = reinterpret_cast<void*>(GetGlobalValue_558150());
            void* redConfig = reinterpret_cast<void*>(GetGlobalValue_558150());
            uint8_t blue = CSF_AT(uint8_t, blueConfig, 0x1D4);
            uint8_t red = CSF_AT(uint8_t, redConfig, 0x1D2);
            uint8_t green = CSF_AT(uint8_t, greenConfig, 0x1D3);
            color[0] = static_cast<float>(red);
            color[1] = static_cast<float>(green);
            color[2] = static_cast<float>(blue);
            color[3] = 255.0f;

            bool show = CSF_AT(uint8_t, context, 0x3A4) != 0;
            if (!show)
            {
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                show = CSF_AT(int, state, 0xCC) != 0 &&
                       context->field_0078 == candidate->field_0078;
                if (!show && CSF_AT(int32_t, context, 0x278) >= 0)
                {
                    int resolved = CHandleTable_Resolve(
                        g_pCurrentScene, nullptr,
                        reinterpret_cast<int>(
                            reinterpret_cast<uint8_t*>(context) + 0x270));
                    show = candidate == reinterpret_cast<C_008342e8*>(resolved);
                }
            }
            if (!show)
                break;

            void* targetVector =
                reinterpret_cast<void*(__fastcall*)(void*, void*, void*)>(
                    candidate->vtbl[0xA4 / 4])(
                        candidate, nullptr, targetVectorBytes);
            position[1] = CSF_AT(float, targetVector, 4) + g_QuarterTurnConstant;

            if (CSF_AT(uint32_t, entry, 0xC))
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, float*, float*)>(
                    CSF_AT(void**, projection, 0)[0x68 / 4])(
                        projection, nullptr, screen, position);

                iconPosition[0] = static_cast<int32_t>(
                    static_cast<int64_t>(screen[0]));
                iconPosition[1] = static_cast<int32_t>(
                    static_cast<int64_t>(screen[1]));

                void* icon = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0xC));
                int* bounds = reinterpret_cast<int*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, icon, 0)[0x4C / 4])(icon, nullptr);
                iconPosition[0] += (bounds[2] - bounds[0]) / -2;

                void* iconMetrics = reinterpret_cast<void*>((*self)->field_0008);
                int verticalOffset =
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, iconMetrics, 0)[0x18 / 4])(
                            iconMetrics, nullptr, 1);
                iconPosition[1] -= verticalOffset;

                icon = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0xC));
                reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
                    CSF_AT(void**, icon, 0)[0x50 / 4])(
                        icon, nullptr, iconPosition);

                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                if (CSF_AT(int, state, 0xCC))
                {
                    uint32_t attachedAddress =
                        CSF_AT(uint32_t, candidate->pad_0018, 0);
                    if (attachedAddress)
                    {
                        uint32_t ownerAddress = attachedAddress - 1712;
                        if (ownerAddress)
                        {
                            void* owner = reinterpret_cast<void*>(ownerAddress);
                            bool isAlert = CSF_AT(uint8_t, owner, 0x443) == 1;

                            void* alternateBlue =
                                reinterpret_cast<void*>(GetGlobalValue_558150());
                            void* alternateGreen =
                                reinterpret_cast<void*>(GetGlobalValue_558150());
                            void* alternateRed =
                                reinterpret_cast<void*>(GetGlobalValue_558150());

                            if (isAlert)
                            {
                                red = CSF_AT(uint8_t, alternateRed, 0x1CC);
                                green = CSF_AT(uint8_t, alternateGreen, 0x1CD);
                                blue = CSF_AT(uint8_t, alternateBlue, 0x1CE);
                            }
                            else
                            {
                                red = CSF_AT(uint8_t, alternateRed, 0x1CF);
                                green = CSF_AT(uint8_t, alternateGreen, 0x1D0);
                                blue = CSF_AT(uint8_t, alternateBlue, 0x1D1);
                            }
                            color[0] = static_cast<float>(red);
                            color[1] = static_cast<float>(green);
                            color[2] = static_cast<float>(blue);
                            color[3] = 255.0f;
                        }
                    }
                }

                icon = reinterpret_cast<void*>(CSF_AT(uint32_t, entry, 0xC));
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, icon, 0)[0x60 / 4])(icon, nullptr);
            }

            if (CSF_AT(uint32_t, entry, 0x14))
            {
                HudAlertIcons_RefreshEntryParticle(entryAddress);
                void* controller = reinterpret_cast<void*>(
                    FxParticleManager_GetParticleController(
                        CSF_AT(int, entry, 0x14)));
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, controller, 0)[0x24 / 4])(
                        controller, nullptr, 1);
            }

            auto* visibleEntries =
                reinterpret_cast<S_0041e5f0*>(&(*self)->field_0020);
            if ((*self)->field_0028 >= (*self)->field_0024)
            {
                // The packet labels the growth routine's address as a vtable.
                (void)CSF_VTBL(0x0041E5F0);
                S_0041e5f0_GrowArrayCapacity(
                    visibleEntries, nullptr, (*self)->field_002c);
            }

            uint32_t used = visibleEntries->field_0008;
            int* destination =
                reinterpret_cast<int*>(visibleEntries->field_0000) + used;
            visibleEntries->field_0008 = used + 1;
            *destination = entryAddress;
        }
        while (false);

        count = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
        ++index;
    }
    while (index < count);

    return static_cast<int>(index);
}

HOOK(0x004A2850, HudAlertIcons_UpdateVisibleEntries, "auto");
