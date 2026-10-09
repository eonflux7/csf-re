#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CGuiControl_HandleMouseMovement,
         0x00719400, (CGuiControl* self, void* edx, int updateArgument));
CSF_FUNC(char, __fastcall, CGuiSlider_UpdateConsoleArrowSelectability,
         0x0072CEF0, (CGuiSlider* self, void* edx));

// This helper takes its operand in ST(0), not on the C++ argument stack.
// Its conversion is reproduced below without inventing a stack argument.
CSF_FUNC(std::int64_t, __cdecl, TruncateX87Operand,
         0x007AF86C, ());

static CGuiControl* GetControlPointer(std::uint32_t address)
{
    return reinterpret_cast<CGuiControl*>(address);
}

static std::uint32_t* CGuiControl_GetPositionPair(CGuiControl* self)
{
    return reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x9C / 4])(self, nullptr);
}

static std::uint32_t* CGuiControl_GetSizePair(CGuiControl* self)
{
    return reinterpret_cast<std::uint32_t* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0xA4 / 4])(self, nullptr);
}

static int CGuiControl_UpdatePart(CGuiControl* self, int updateArgument)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, self, 0)[0x34 / 4])(
            self, nullptr, updateArgument);
}

static float FloatFromBits(std::uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

// Round the signed integer ratio directly to binary32.
static float RoundSignedRatioToFloat(std::int32_t numerator,
                                    std::int32_t denominator)
{
    const bool negative = (numerator < 0) != (denominator < 0);
    const std::uint32_t sign = negative ? 0x80000000u : 0u;

    if (denominator == 0)
    {
        if (numerator == 0)
            return FloatFromBits(0xFFC00000u);
        return FloatFromBits(sign | 0x7F800000u);
    }

    if (numerator == 0)
        return FloatFromBits(sign);

    std::uint64_t magnitudeNumerator =
        numerator < 0 ? 0u - static_cast<std::uint32_t>(numerator)
                      : static_cast<std::uint32_t>(numerator);
    std::uint64_t magnitudeDenominator =
        denominator < 0 ? 0u - static_cast<std::uint32_t>(denominator)
                        : static_cast<std::uint32_t>(denominator);

    int exponent = 0;
    while (magnitudeNumerator < magnitudeDenominator)
    {
        magnitudeNumerator <<= 1;
        --exponent;
    }
    while (magnitudeNumerator >= (magnitudeDenominator << 1))
    {
        magnitudeDenominator <<= 1;
        ++exponent;
    }

    const std::uint64_t scaledNumerator = magnitudeNumerator << 23;
    std::uint32_t significand = static_cast<std::uint32_t>(
        scaledNumerator / magnitudeDenominator);
    const std::uint64_t remainder =
        scaledNumerator % magnitudeDenominator;

    if ((remainder << 1) > magnitudeDenominator ||
        ((remainder << 1) == magnitudeDenominator &&
         (significand & 1u) != 0))
    {
        ++significand;
    }

    if (significand == 0x01000000u)
    {
        significand >>= 1;
        ++exponent;
    }

    // The normalized significand has exactly one implicit leading bit.
    const std::uint32_t fraction = significand - (1u << 23);
    return FloatFromBits(
        sign |
        (static_cast<std::uint32_t>(exponent + 127) << 23) |
        fraction);
}

static std::uint32_t TruncateSignedRatioToLowWord(
    std::int32_t numerator, std::int32_t denominator)
{
    (void)TruncateX87Operand;

    // __ftol2's indefinite INT64_MIN result has a zero low word.
    if (denominator == 0)
        return 0;

    return static_cast<std::uint32_t>(
        static_cast<std::int64_t>(numerator) /
        static_cast<std::int64_t>(denominator));
}

static int __fastcall CGuiSlider_UpdateDraggedThumb(
    CGuiSlider* self, void* /*edx*/, int updateArgument)
{
    CGuiControl_HandleMouseMovement(self, nullptr, updateArgument);

    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);

    if ((static_cast<std::uint32_t>(result) & 0xFFu) == 0)
        return result;

    if (self->m_pThumb != 0)
    {
        std::uint32_t* initialPosition =
            CGuiControl_GetPositionPair(GetControlPointer(self->m_pThumb));
        const std::uint32_t initialX = CSF_AT(std::uint32_t, initialPosition, 0);

        if (self->m_bHorizontal == 0)
        {
            CGuiControl_UpdatePart(
                GetControlPointer(self->m_pThumb), updateArgument);
        }

        CGuiControl* thumb = GetControlPointer(self->m_pThumb);
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, thumb, 0)[0x140 / 4])(thumb, nullptr);

        if ((static_cast<std::uint32_t>(result) & 0x10u) != 0)
        {
            std::uint32_t* currentPosition =
                CGuiControl_GetPositionPair(
                    GetControlPointer(self->m_pThumb));
            std::uint32_t position[2];
            position[0] = CSF_AT(std::uint32_t, currentPosition, 0);
            std::uint32_t thumbY = CSF_AT(std::uint32_t, currentPosition, 4);

            result = static_cast<int>(self->m_bHorizontal);
            if (result == 0)
            {
                CGuiControl* upperArrow =
                    GetControlPointer(self->m_pArrowUp);
                std::uint32_t* upperSize =
                    CGuiControl_GetSizePair(upperArrow);
                std::uint32_t* upperPosition =
                    CGuiControl_GetPositionPair(upperArrow);
                const std::uint32_t trackStart =
                    CSF_AT(std::uint32_t, upperSize, 4) +
                    CSF_AT(std::uint32_t, upperPosition, 4);

                std::uint32_t* lowerPosition =
                    CGuiControl_GetPositionPair(
                        GetControlPointer(self->m_pArrowDown));
                const std::uint32_t trackEnd =
                    CSF_AT(std::uint32_t, lowerPosition, 4);

                if (static_cast<std::int32_t>(thumbY) <
                    static_cast<std::int32_t>(trackStart))
                {
                    thumbY = trackStart;
                }

                std::uint32_t* thumbSize =
                    CGuiControl_GetSizePair(
                        GetControlPointer(self->m_pThumb));
                if (static_cast<std::int32_t>(
                        thumbY + CSF_AT(std::uint32_t, thumbSize, 4)) >
                    static_cast<std::int32_t>(trackEnd))
                {
                    thumbSize = CGuiControl_GetSizePair(
                        GetControlPointer(self->m_pThumb));
                    thumbY =
                        trackEnd - CSF_AT(std::uint32_t, thumbSize, 4);
                }

                self->field_0160 = initialX;
                const std::uint8_t useStoredPosition = self->field_0168;
                self->field_0164 = thumbY;

                if (useStoredPosition != 0)
                {
                    position[0] = self->field_0158;
                    position[1] = self->field_015c;
                }
                else
                {
                    position[0] = self->field_0160;
                    position[1] = self->field_0164;
                }

                thumb = GetControlPointer(self->m_pThumb);
                reinterpret_cast<void (__fastcall*)(
                    void*, void*, std::uint32_t*)>(
                    CSF_AT(void**, thumb, 0)[0x98 / 4])(
                        thumb, nullptr, position);

                const std::uint32_t trackLength = trackEnd - trackStart;
                thumbSize = CGuiControl_GetSizePair(
                    GetControlPointer(self->m_pThumb));

                if (trackLength ==
                    CSF_AT(std::uint32_t, thumbSize, 4))
                {
                    self->m_fractionalPosition = 0.0f;
                    self->m_pos = 0;
                }
                else
                {
                    const std::int32_t numerator =
                        static_cast<std::int32_t>(
                            (position[1] - trackStart) * self->m_count);
                    const std::int32_t denominator =
                        static_cast<std::int32_t>(trackLength);

                    self->m_fractionalPosition =
                        RoundSignedRatioToFloat(numerator, denominator);
                    self->m_pos =
                        TruncateSignedRatioToLowWord(numerator, denominator);
                }

                const std::uint32_t count = self->m_count;
                if (static_cast<std::int32_t>(self->m_pos) >=
                    static_cast<std::int32_t>(count))
                {
                    const std::uint32_t lastPosition = count - 1u;
                    self->m_pos = lastPosition;
                    self->m_fractionalPosition = static_cast<float>(
                        static_cast<std::int32_t>(lastPosition));
                }

                CGuiSlider_UpdateConsoleArrowSelectability(self, nullptr);
                result =
                    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, self, 0)[0x138 / 4])(
                            self, nullptr, 13);
            }
        }
    }

    if (self->m_pArrowUp != 0)
    {
        result = CGuiControl_UpdatePart(
            GetControlPointer(self->m_pArrowUp), updateArgument);
    }

    if (self->m_pArrowDown != 0)
    {
        result = CGuiControl_UpdatePart(
            GetControlPointer(self->m_pArrowDown), updateArgument);
    }

    return result;
}

HOOK(0x0072DE90, CGuiSlider_UpdateDraggedThumb, "auto");
