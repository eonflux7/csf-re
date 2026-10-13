#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint64_t, __cdecl, GuiControl_RoundToNearestInteger, 0x00718C90, (float value));
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static char __fastcall CGuiControl_Update(CGuiControl* self, void* /*edx*/, float deltaTime)
{
    char result = static_cast<char>(self->m_bSelectable);
    if (!result)
        return result;

    // The original frame holds a four-component color, a two-component
    // sample, and overlapping color/integer output storage.
    alignas(8) std::uint8_t frameBytes[0x20]{};
    float* sampledVector = reinterpret_cast<float*>(frameBytes + 0x08);
    std::int32_t* roundedVector =
        reinterpret_cast<std::int32_t*>(frameBytes + 0x18);

    void* animation = reinterpret_cast<void*>(self->m_pColorAnim);
    if (animation)
    {
        bool finished =
            (reinterpret_cast<bool(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, animation, 0)[0x10 / 4]))(
                    animation, nullptr, self->m_colorAnimTime);

        if (finished)
        {
            animation = reinterpret_cast<void*>(self->m_pColorAnim);
            if (animation)
            {
                (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, animation, 0)[0 / 4]))(
                        animation, nullptr, 1);
                self->m_pColorAnim = 0;
            }
            self->m_colorAnimTime = 0.0f;
            result = static_cast<char>(self->vSendEvent(8));
        }
        else
        {
            if (self->m_colorAnimTime == g_fZero)
                self->vSendEvent(7);

            animation = reinterpret_cast<void*>(self->m_pColorAnim);
            float sampleTime = deltaTime + self->m_colorAnimTime;
            self->m_colorAnimTime = sampleTime;

            (reinterpret_cast<void(__fastcall*)(void*, void*, void*, float)>(
                CSF_AT(void**, animation, 0)[0x14 / 4]))(
                    animation, nullptr, frameBytes, sampleTime);

            std::memcpy(frameBytes + 0x10, frameBytes, 0x10);
            result = static_cast<char>(self->vSetColor(frameBytes + 0x10));
        }
    }

    animation = reinterpret_cast<void*>(self->m_pPosAnim);
    if (animation)
    {
        bool finished =
            (reinterpret_cast<bool(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, animation, 0)[0x10 / 4]))(
                    animation, nullptr, self->m_posAnimTime);

        if (!finished)
        {
            if (self->m_posAnimTime == g_fZero)
                self->vSendEvent(9);

            animation = reinterpret_cast<void*>(self->m_pPosAnim);
            float sampleTime = deltaTime + self->m_posAnimTime;
            self->m_posAnimTime = sampleTime;

            (reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
                CSF_AT(void**, animation, 0)[0x14 / 4]))(
                    animation, nullptr, sampledVector, sampleTime);
        }
        else
        {
            animation = reinterpret_cast<void*>(self->m_pPosAnim);
            (reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
                CSF_AT(void**, animation, 0)[0x14 / 4]))(
                    animation, nullptr, sampledVector, self->m_posAnimTime);
        }

        roundedVector[0] = static_cast<std::int32_t>(
            GuiControl_RoundToNearestInteger(sampledVector[0]));
        roundedVector[1] = static_cast<std::int32_t>(
            GuiControl_RoundToNearestInteger(sampledVector[1]));
        result = static_cast<char>(self->vSetPosition(roundedVector));

        if (finished)
        {
            animation = reinterpret_cast<void*>(self->m_pPosAnim);
            if (animation)
            {
                (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, animation, 0)[0 / 4]))(
                        animation, nullptr, 1);
                self->m_pPosAnim = 0;
            }
            self->m_posAnimTime = 0.0f;
            result = static_cast<char>(self->vSendEvent(10));
        }
    }

    animation = reinterpret_cast<void*>(self->m_pSizeAnim);
    if (animation)
    {
        bool finished =
            (reinterpret_cast<bool(__fastcall*)(void*, void*, float)>(
                CSF_AT(void**, animation, 0)[0x10 / 4]))(
                    animation, nullptr, self->m_sizeAnimTime);

        if (!finished)
        {
            if (self->m_sizeAnimTime == g_fZero)
                self->vSendEvent(11);

            animation = reinterpret_cast<void*>(self->m_pSizeAnim);
            float sampleTime = deltaTime + self->m_sizeAnimTime;
            self->m_sizeAnimTime = sampleTime;

            (reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
                CSF_AT(void**, animation, 0)[0x14 / 4]))(
                    animation, nullptr, sampledVector, sampleTime);
        }
        else
        {
            animation = reinterpret_cast<void*>(self->m_pSizeAnim);
            (reinterpret_cast<void(__fastcall*)(void*, void*, float*, float)>(
                CSF_AT(void**, animation, 0)[0x14 / 4]))(
                    animation, nullptr, sampledVector, self->m_sizeAnimTime);
        }

        roundedVector[0] = static_cast<std::int32_t>(
            GuiControl_RoundToNearestInteger(sampledVector[0]));
        roundedVector[1] = static_cast<std::int32_t>(
            GuiControl_RoundToNearestInteger(sampledVector[1]));
        result = static_cast<char>(self->vSetSize(roundedVector));

        if (finished)
        {
            animation = reinterpret_cast<void*>(self->m_pSizeAnim);
            if (animation)
            {
                (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, animation, 0)[0 / 4]))(
                        animation, nullptr, 1);
                self->m_pSizeAnim = 0;
            }
            self->m_sizeAnimTime = 0.0f;
            result = static_cast<char>(self->vSendEvent(12));
        }
    }

    void* effects = reinterpret_cast<void*>(self->m_pEffects);
    if (effects)
    {
        return (reinterpret_cast<char(__fastcall*)(void*, void*, float)>(
            CSF_AT(void**, effects, 0)[0x7C / 4]))(
                effects, nullptr, deltaTime);
    }

    return result;
}

HOOK(0x00719530, CGuiControl_Update, "auto");
