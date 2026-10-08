#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(void, __fastcall, CGuiStatic_CopyPropertiesToObject, 0x007181E0,
         (void* self, void* edx, int* target));
CSF_FUNC(void, __fastcall, CGuiStatic_CopyStateToTarget, 0x00718230,
         (void* self, void* edx, int* target));
CSF_FUNC(int, __fastcall, CGuiControl_SendEvent, 0x00718280,
         (void* self, void* edx, int event));
CSF_FUNC(int, __fastcall, CGuiSlider_ForwardSelfToParent, 0x00718660,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, CGuiButton_StartTextColorEffect, 0x00725740,
         (void* self, void* edx));

static void CGuiButton_CopyBoundsToQuad(
    CGuiButton* self, const uint32_t& quadField, void* boundsBuffer)
{
    void* quad = reinterpret_cast<void*>(quadField);
    if (!quad)
        return;

    // Save the original quad's table, but reload the quad after obtaining bounds.
    void** quadTable = CSF_AT(void**, quad, 0);
    void* bounds =
        ((void*(__fastcall*)(void*, void*, void*))
             CSF_AT(void**, self, 0)[0x228 / 4])(
            self, nullptr, boundsBuffer);

    ((void(__fastcall*)(void*, void*, void*))quadTable[0x68 / 4])(
        reinterpret_cast<void*>(quadField), nullptr, bounds);
}

static int __fastcall CGuiButton_HandleEvent(
    CGuiButton* self, void* /*edx*/, int event)
{
    if (!self->field_01ec &&
        (event == 15 || (event > 17 && event <= 20)))
    {
        return CGuiControl_SendEvent(self, nullptr, event);
    }

    switch (event)
    {
    case 2:
    case 3:
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pIdleQuad));
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pHoverQuad));
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pPressedQuad));
        CGuiStatic_CopyPropertiesToObject(
            self, nullptr, reinterpret_cast<int*>(self->m_pPermanentQuad));
        CGuiStatic_CopyStateToTarget(
            self, nullptr, reinterpret_cast<int*>(self->m_pText));
        break;

    case 6:
    {
        uint32_t idleBounds[4];
        uint32_t hoverBounds[4];
        uint32_t pressedBounds[4];
        uint32_t permanentBounds[4];

        CGuiButton_CopyBoundsToQuad(self, self->m_pIdleQuad, idleBounds);
        CGuiButton_CopyBoundsToQuad(self, self->m_pHoverQuad, hoverBounds);
        CGuiButton_CopyBoundsToQuad(self, self->m_pPressedQuad, pressedBounds);
        CGuiButton_CopyBoundsToQuad(self, self->m_pPermanentQuad, permanentBounds);
        break;
    }

    case 15:
    {
        void* owner =
            ((void*(__fastcall*)(void*, void*))
                 CSF_AT(void**, self, 0)[0x10C / 4])(self, nullptr);
        void* activeControl =
            ((void*(__fastcall*)(void*, void*))
                 CSF_AT(void**, owner, 0)[0x104 / 4])(owner, nullptr);

        if (activeControl &&
            ((uint8_t(__fastcall*)(void*, void*))
                 CSF_AT(void**, activeControl, 0)[0x188 / 4])(
                activeControl, nullptr))
        {
            break;
        }

        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) &
             1) == 0)
        {
            break;
        }

        CGuiSlider_ForwardSelfToParent(self, nullptr);

        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) &
             0x10) == 0)
        {
            void* links = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* target = CSF_AT(void*, links, 4);
            if (target)
            {
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, target, 0)[0x24 / 4])(
                    target, nullptr, 0);
            }
        }
        break;
    }

    case 18:
    case 20:
        if (((int(__fastcall*)(void*, void*))
                 CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) &
            1)
        {
            CGuiSlider_ForwardSelfToParent(self, nullptr);
            self->field_0148 = 2;
        }
        break;

    case 19:
        if ((((int(__fastcall*)(void*, void*))
                  CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr) &
             1) &&
            self->field_0148 == 2)
        {
            void* links = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0010), 0);
            void* target = CSF_AT(void*, links, 0);
            if (target)
            {
                ((void(__fastcall*)(void*, void*, int))
                     CSF_AT(void**, target, 0)[0x24 / 4])(
                    target, nullptr, 0);
            }

            ((int(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, self, 0)[0x138 / 4])(
                self, nullptr, 13);
            CGuiButton_StartTextColorEffect(self, nullptr);
        }
        self->field_0148 = 0;
        break;

    default:
        break;
    }

    return CGuiControl_SendEvent(self, nullptr, event);
}

HOOK(0x00725DA0, CGuiButton_HandleEvent, "auto");
