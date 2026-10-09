#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint8_t, __fastcall, CGuiControl_HandleMouseMovement, 0x00719400, (CGuiControl* self, void* , void* mouseEvent));
CSF_FUNC(char, __fastcall, CGuiSlider_UpdateConsoleArrowSelectability, 0x0072CEF0, (CGuiSlider* self, void*));

static int __fastcall CGuiSlider_HandleMouseMovement(
    CGuiSlider* self, void* /*edx*/, void* mouseEvent)
{
    CGuiControl_HandleMouseMovement(self, nullptr, mouseEvent);

    int result = self->vIsShown();
    if (static_cast<std::uint8_t>(result) == 0)
        return result;

    auto* thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
    if (thumb)
    {
        char* initialPositionData = thumb->vGetEmbeddedA8();
        const std::int32_t initialPosition[2] = {
            CSF_AT(std::int32_t, initialPositionData, 0),
            CSF_AT(std::int32_t, initialPositionData, 4)
        };

        if (self->m_bHorizontal == 1)
        {
            thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
            ((int (__fastcall*)(void*, void*, void*))thumb->vtbl[0x30 / 4])(
                thumb, nullptr, mouseEvent);
        }

        thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
        result = static_cast<int>(
            reinterpret_cast<std::intptr_t>(thumb->vSlot140()));

        if ((result & 0x10) != 0)
        {
            thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
            char* movedPositionData = thumb->vGetEmbeddedA8();
            result = static_cast<int>(
                reinterpret_cast<std::intptr_t>(movedPositionData));

            std::int32_t position[2];
            position[1] = CSF_AT(std::int32_t, movedPositionData, 4);
            position[0] = CSF_AT(std::int32_t, movedPositionData, 0);

            if (self->m_bHorizontal == 1)
            {
                void* arrowUpSize =
                    reinterpret_cast<CGuiControl*>(self->m_pArrowUp)->vGetSize();
                char* arrowUpPosition =
                    reinterpret_cast<CGuiControl*>(self->m_pArrowUp)->vGetEmbeddedA8();

                const std::uint32_t arrowUpX =
                    CSF_AT(std::uint32_t, arrowUpPosition, 0);
                const std::uint32_t trackStart =
                    arrowUpX + CSF_AT(std::uint32_t, arrowUpSize, 0);

                char* arrowDownPosition =
                    reinterpret_cast<CGuiControl*>(self->m_pArrowDown)->vGetEmbeddedA8();
                const std::uint32_t trackEnd =
                    CSF_AT(std::uint32_t, arrowDownPosition, 0);

                std::uint32_t thumbX = static_cast<std::uint32_t>(position[0]);
                if (static_cast<std::int32_t>(thumbX) <
                    static_cast<std::int32_t>(trackStart))
                {
                    thumbX = trackStart;
                }

                thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
                void* thumbSize = thumb->vGetSize();
                if (static_cast<std::int32_t>(
                        thumbX + CSF_AT(std::uint32_t, thumbSize, 0)) >
                    static_cast<std::int32_t>(trackEnd))
                {
                    thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
                    thumbSize = thumb->vGetSize();
                    thumbX = trackEnd - CSF_AT(std::uint32_t, thumbSize, 0);
                }

                CSF_AT(std::int32_t, self, 0x164) = initialPosition[1];
                const std::uint8_t useStoredPosition = self->field_0168;
                self->field_0160 = thumbX;

                if (useStoredPosition)
                {
                    position[0] = static_cast<std::int32_t>(self->field_0158);
                    position[1] = CSF_AT(std::int32_t, self, 0x15C);
                }
                else
                {
                    position[0] = static_cast<std::int32_t>(self->field_0160);
                    position[1] = CSF_AT(std::int32_t, self, 0x164);
                }

                thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
                thumb->vSetPosition(position);

                const std::int32_t trackLength =
                    static_cast<std::int32_t>(trackEnd - trackStart);
                thumb = reinterpret_cast<CGuiControl*>(self->m_pThumb);
                thumbSize = thumb->vGetSize();

                if (trackLength == CSF_AT(std::int32_t, thumbSize, 0))
                {
                    self->m_fractionalPosition = 0.0f;
                    self->m_pos = 0;
                }
                else
                {
                    const std::uint32_t offset =
                        static_cast<std::uint32_t>(position[0]) - trackStart;
                    const std::int32_t scaledOffset =
                        static_cast<std::int32_t>(offset * self->m_count);
                    const double fractionalPosition =
                        static_cast<double>(scaledOffset) /
                        static_cast<double>(trackLength);

                    self->m_fractionalPosition = static_cast<float>(fractionalPosition);
                    self->m_pos = static_cast<std::uint32_t>(
                        static_cast<std::int64_t>(fractionalPosition));
                }

                const std::int32_t count =
                    static_cast<std::int32_t>(self->m_count);
                if (static_cast<std::int32_t>(self->m_pos) >= count)
                {
                    const std::uint32_t lastPosition =
                        static_cast<std::uint32_t>(count) - 1;
                    self->m_pos = lastPosition;
                    self->m_fractionalPosition =
                        static_cast<float>(static_cast<std::int32_t>(lastPosition));
                }

                CGuiSlider_UpdateConsoleArrowSelectability(self, nullptr);
                result = self->vSendEvent(13);
            }
        }
    }

    auto* arrowUp = reinterpret_cast<CGuiControl*>(self->m_pArrowUp);
    if (arrowUp)
    {
        result =
            ((int (__fastcall*)(void*, void*, void*))arrowUp->vtbl[0x30 / 4])(
                arrowUp, nullptr, mouseEvent);
    }

    auto* arrowDown = reinterpret_cast<CGuiControl*>(self->m_pArrowDown);
    if (arrowDown)
    {
        return
            ((int (__fastcall*)(void*, void*, void*))arrowDown->vtbl[0x30 / 4])(
                arrowDown, nullptr, mouseEvent);
    }

    return result;
}

HOOK(0x72DCA0, CGuiSlider_HandleMouseMovement, "auto");
