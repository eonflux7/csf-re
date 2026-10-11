#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_MissingMetricValue, 0x0082D4C4);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_HAnimUpdateBaseInterval, 0x00861F2C);
CSF_GLOBAL(float, g_HAnimUpdateIntervalScale, 0x00861F30);

CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentAnim, 0x0061F050, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorBlend, 0x0061F320, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, RtAnimInterpolatorSetCurrentTime, 0x0061F590, (int a1, float a2));
CSF_FUNC(int, __cdecl, GfxHAnim_PrepareHierarchy, 0x006E8150, (int a1, int a2, int a3));
CSF_FUNC(uint32_t*, __stdcall, GfxHAnim_UpdateRootBinding, 0x006E85B0, (int a1, uint32_t* a2, int a3));
CSF_FUNC(int, __stdcall, GfxHAnim_ApplyChannelBlend, 0x006E8D50, (uint32_t* a1, int a2));
CSF_FUNC(int, __fastcall, GfxHAnim_UpdateHierarchy, 0x006E9BF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GfxHAnim_FinishTransition, 0x006E9D60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxHAnim_UpdateRootMotion, 0x006EAB60, (void* self, void* edx, float a2, char a3, char a4, uint32_t* a5));
CSF_FUNC(void, __fastcall, GfxHAnim_AdvanceChannel, 0x006EADC0, (void* self, void* edx, int a2, int arg4, uint8_t* a4, float a3));
CSF_FUNC(void, __fastcall, GfxHAnim_AdvanceBlendSource, 0x006EAF30, (void* self, void* edx, int a2, float a3, float a4));
CSF_FUNC(int, __cdecl, GetValue8BBB28, 0x006F7820, ());
CSF_FUNC(char, __fastcall, GfxHAnim_ReleaseTransition, 0x006F7A70, (void* self, void* edx, void** a2));

static void __fastcall C_00861e98_UpdateAnimation(
    C_00861e98* self, void* /*edx*/, float deltaTime,
    uint32_t* rootMotionOutput, int intervalBits)
{
    const uint32_t initialFlags = CSF_AT(uint32_t, self, 0x58);
    if ((initialFlags & 4) != 0 || !CSF_AT(uint32_t, self, 0x1C))
        return;

    deltaTime *= CSF_AT(float, self, 0x54);
    if (deltaTime <= g_fZero)
        return;

    float interval;
    std::memcpy(&interval, &intervalBits, sizeof(interval));
    if (interval == g_MissingMetricValue)
    {
        for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x0C); ++index)
        {
            auto* channel = reinterpret_cast<uint8_t*>(
                CSF_AT(void*, self, 0x04)) + 36 * index;
            if (CSF_AT(float, channel, 0x08) > g_fZero)
                CSF_AT(uint32_t, channel, 0x0C) = CSF_AT(uint32_t, channel, 0x08);
        }
        CSF_AT(uint32_t, self, 0x58) &= ~2u;
    }
    else
    {
        CSF_AT(uint32_t, self, 0x58) = initialFlags | 2;
    }

    const uint32_t updateFlags = CSF_AT(uint32_t, self, 0x58);
    bool advance = (updateFlags & 8) != 0;
    if (!advance)
    {
        const float accumulated = deltaTime + CSF_AT(float, self, 0x5C);
        CSF_AT(float, self, 0x5C) = accumulated;
        if ((updateFlags & 2) != 0 &&
            accumulated >= interval * g_HAnimUpdateIntervalScale + g_HAnimUpdateBaseInterval)
        {
            advance = true;
            deltaTime = accumulated;
            CSF_AT(uint32_t, self, 0x5C) = 0;
        }

        if (CSF_AT(float, self, 0x5C) > g_fZero)
        {
            for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x0C); ++index)
            {
                if (advance)
                    break;

                auto* channel = reinterpret_cast<uint8_t*>(
                    CSF_AT(void*, self, 0x04)) + 36 * index;
                const uint32_t flags = CSF_AT(uint32_t, channel, 0x14);
                if ((flags & 4) != 0 && (flags & 1) == 0)
                {
                    void* interpolator = CSF_AT(
                        void*, CSF_AT(void*, channel, 0x00), 0x20);
                    if (CSF_AT(float, interpolator, 0x04) + CSF_AT(float, self, 0x5C) >
                        CSF_AT(float, CSF_AT(void*, interpolator, 0x00), 0x0C))
                    {
                        advance = true;
                        deltaTime = CSF_AT(float, self, 0x5C);
                        CSF_AT(uint32_t, self, 0x5C) = 0;
                    }
                }
            }
        }
    }

    CSF_AT(uint32_t, self, 0x58) =
        advance ? updateFlags | 0x10 : updateFlags & 0xFFFFFFEF;

    for (uint32_t index = 0; index < CSF_AT(uint32_t, self, 0x0C); ++index)
    {
        auto* channel = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(CSF_AT(void*, self, 0x04)) + 36 * index);
        const uint32_t flags = CSF_AT(uint32_t, channel, 0x14);

        if ((flags & 4) != 0 && advance)
        {
            const float duration = CSF_AT(float, channel, 0x08);
            ++CSF_AT(uint32_t, channel, 0x20);
            *reinterpret_cast<uint8_t*>(&intervalBits) = 0;

            // These negated comparisons also preserve the original unordered cases.
            if (!(duration <= g_fZero))
            {
                const float elapsed = deltaTime + CSF_AT(float, channel, 0x0C);
                CSF_AT(float, channel, 0x0C) = elapsed;
                if (!(elapsed < CSF_AT(float, channel, 0x08)))
                {
                    CSF_AT(uint32_t, channel, 0x08) = 0;
                    CSF_AT(uint32_t, channel, 0x0C) = 0;
                    CSF_AT(uint32_t, channel, 0x14) = flags & 0xFFFFFFF7;
                    if ((flags & 0x10) != 0)
                    {
                        CSF_AT(uint32_t, channel, 0x14) = flags & 0xFFFFFFE3;
                        GfxHAnim_FinishTransition(self, nullptr);
                    }
                    else
                    {
                        RtAnimInterpolatorSetCurrentAnim(
                            CSF_AT(uint32_t*, CSF_AT(void*, channel, 0x00), 0x20),
                            CSF_AT(uint32_t*,
                                CSF_AT(void*, CSF_AT(void*,
                                    CSF_AT(void*, channel, 0x04), 0x0C), 0x20), 0x00));
                        RtAnimInterpolatorSetCurrentTime(
                            CSF_AT(int, CSF_AT(void*, channel, 0x00), 0x20),
                            CSF_AT(float,
                                CSF_AT(void*, CSF_AT(void*,
                                    CSF_AT(void*, channel, 0x04), 0x0C), 0x20), 0x04));
                        if (!index)
                        {
                            GfxHAnim_UpdateRootBinding(
                                reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x60),
                                CSF_AT(uint32_t*, channel, 0x1C),
                                CSF_AT(int, channel, 0x00));
                        }
                        GfxHAnim_UpdateHierarchy(self, nullptr, CSF_AT(int, channel, 0x00));
                    }

                    if (CSF_AT(void*, channel, 0x04))
                    {
                        void* manager = reinterpret_cast<void*>(GetValue8BBB28());
                        GfxHAnim_ReleaseTransition(
                            manager, nullptr,
                            reinterpret_cast<void**>(reinterpret_cast<uint8_t*>(channel) + 0x04));
                        CSF_AT(uint32_t, channel, 0x04) = 0;
                    }
                    CSF_AT(uint32_t, channel, 0x08) = 0;
                    CSF_AT(uint32_t, channel, 0x0C) = 0;
                }
                else
                {
                    const float fraction = elapsed / CSF_AT(float, channel, 0x08);
                    int fractionBits;
                    std::memcpy(&fractionBits, &fraction, sizeof(fractionBits));

                    if ((flags & 8) != 0)
                    {
                        GfxHAnim_PrepareHierarchy(
                            CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x08),
                            CSF_AT(int, self, 0x14), CSF_AT(int, channel, 0x18));
                        RtAnimInterpolatorBlend(
                            CSF_AT(int, CSF_AT(void*, channel, 0x00), 0x20),
                            CSF_AT(int, CSF_AT(void*, CSF_AT(void*, channel, 0x04), 0x08), 0x20),
                            CSF_AT(int, CSF_AT(void*, CSF_AT(void*, channel, 0x04), 0x0C), 0x20),
                            fractionBits);
                        GfxHAnim_AdvanceChannel(
                            self, nullptr, reinterpret_cast<int>(channel),
                            CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C),
                            reinterpret_cast<uint8_t*>(&intervalBits), deltaTime);
                        GfxHAnim_UpdateHierarchy(
                            self, nullptr, CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C));
                    }
                    else if ((flags & 0x10) != 0)
                    {
                        GfxHAnim_PrepareHierarchy(
                            CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C),
                            CSF_AT(int, self, 0x14), CSF_AT(int, channel, 0x18));
                        GfxHAnim_ApplyChannelBlend(channel, fractionBits);
                    }
                    else if ((flags & 0x400) == 0)
                    {
                        GfxHAnim_AdvanceChannel(
                            self, nullptr, reinterpret_cast<int>(channel),
                            CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C),
                            reinterpret_cast<uint8_t*>(&intervalBits), deltaTime);
                        GfxHAnim_UpdateHierarchy(
                            self, nullptr, CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C));
                        GfxHAnim_AdvanceBlendSource(
                            self, nullptr, reinterpret_cast<int>(channel),
                            CSF_AT(float, CSF_AT(void*, channel, 0x04), 0x08), deltaTime);
                        GfxHAnim_UpdateHierarchy(
                            self, nullptr, CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x08));
                        GfxHAnim_ApplyChannelBlend(channel, fractionBits);
                    }
                    else
                    {
                        GfxHAnim_ApplyChannelBlend(channel, fractionBits);
                        GfxHAnim_AdvanceChannel(
                            self, nullptr, reinterpret_cast<int>(channel),
                            CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C),
                            reinterpret_cast<uint8_t*>(&intervalBits), deltaTime);
                        GfxHAnim_UpdateHierarchy(
                            self, nullptr, CSF_AT(int, CSF_AT(void*, channel, 0x04), 0x0C));
                    }
                }
            }
            else
            {
                GfxHAnim_AdvanceChannel(
                    self, nullptr, reinterpret_cast<int>(channel), CSF_AT(int, channel, 0x00),
                    reinterpret_cast<uint8_t*>(&intervalBits), deltaTime);
                GfxHAnim_UpdateHierarchy(self, nullptr, CSF_AT(int, channel, 0x00));
            }

            if (!index && deltaTime > g_fZero &&
                ((CSF_AT(uint32_t, channel, 0x14) & 0x40) != 0 ||
                 (CSF_AT(uint32_t, channel, 0x14) & 0x80) != 0))
            {
                float motionValue;
                std::memcpy(&motionValue, &intervalBits, sizeof(motionValue));
                GfxHAnim_UpdateRootMotion(
                    self, nullptr, motionValue,
                    (CSF_AT(uint32_t, channel, 0x14) & 0x40) != 0,
                    (CSF_AT(uint32_t, channel, 0x14) & 0x80) != 0,
                    rootMotionOutput);
            }
        }
        CSF_AT(uint32_t, channel, 0x14) |= 0x200u;
    }

    if (advance)
    {
        const int count = CSF_AT(int, CSF_AT(void*, self, 0x14), 0x04);
        for (int index = 0; index < count; ++index)
        {
            void* node = CSF_AT(void*, CSF_AT(void*, self, 0x4C), 4 * index);
            if (node)
                CSF_AT(uint8_t, node, 0x20) = 1;
        }
    }
}

HOOK(0x006EB410, C_00861e98_UpdateAnimation, "auto");
