#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(char, __fastcall, CGuiControl_ProcessSelectionInput_718360, 0x00718360,
         (CGuiControl* self, void* edx, int input));
CSF_FUNC(int, __fastcall, CGuiSlider_GetPositionFromCoordinate, 0x0072D2B0,
         (CGuiSlider* self, void* edx, int coordinate));

static int __fastcall CGuiSlider_ProcessInput(CGuiSlider* self, void* /*edx*/, int input)
{
    int result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    CGuiControl_ProcessSelectionInput_718360(self, nullptr, input);
    bool arrowHandled = false;

    void* arrow = reinterpret_cast<void*>(self->m_pArrowUp);
    if (arrow)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, arrow, 0)[0x18 / 4])(arrow, nullptr, input);

        arrow = reinterpret_cast<void*>(self->m_pArrowUp);
        int state = ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, arrow, 0)[0x140 / 4])(arrow, nullptr);
        if (state & 5)
        {
            uint32_t position = self->m_pos - uint32_t{1};
            ((void(__fastcall*)(void*, void*, int, int))
                CSF_AT(void**, self, 0)[0x26C / 4])(
                    self, nullptr, static_cast<int32_t>(position), 1);

            arrow = reinterpret_cast<void*>(self->m_pArrowUp);
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, arrow, 0)[0x18 / 4])(arrow, nullptr, input);

            arrowHandled = true;
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
        }
    }

    arrow = reinterpret_cast<void*>(self->m_pArrowDown);
    if (arrow)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, arrow, 0)[0x18 / 4])(arrow, nullptr, input);

        if (!arrowHandled)
        {
            arrow = reinterpret_cast<void*>(self->m_pArrowDown);
            int state = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, arrow, 0)[0x140 / 4])(arrow, nullptr);
            if (state & 5)
            {
                uint32_t position = self->m_pos + uint32_t{1};
                ((void(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, self, 0)[0x26C / 4])(
                        self, nullptr, static_cast<int32_t>(position), 1);

                arrow = reinterpret_cast<void*>(self->m_pArrowDown);
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, arrow, 0)[0x18 / 4])(arrow, nullptr, input);

                arrowHandled = true;
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
            }
        }
    }

    result = static_cast<int32_t>(self->m_pThumb);
    if (!result)
        return result;

    void* thumb = reinterpret_cast<void*>(static_cast<uint32_t>(result));
    result = ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, thumb, 0)[0x18 / 4])(thumb, nullptr, input);
    if (arrowHandled)
        return result;

    result = ((int(__fastcall*)(void*, void*))
        CSF_AT(void**, self, 0)[0x140 / 4])(self, nullptr);
    if (!(result & 5))
        return result;

    uint32_t origin[2];
    ((void(__fastcall*)(void*, void*, uint32_t*))
        CSF_AT(void**, self, 0)[0x160 / 4])(self, nullptr, origin);

    uint32_t orientation = self->m_bHorizontal;
    if (orientation == 0 || orientation == 1)
    {
        void* inputData = reinterpret_cast<void*>(static_cast<uint32_t>(input));
        uint32_t coordinate;
        if (orientation == 0)
            coordinate = CSF_AT(uint32_t, inputData, 0x18) - origin[1];
        else
            coordinate = CSF_AT(uint32_t, inputData, 0x14) - origin[0];

        // The original captures this vtable before the coordinate conversion.
        void** savedVtable = CSF_AT(void**, self, 0);
        int position = CGuiSlider_GetPositionFromCoordinate(
            self, nullptr, static_cast<int32_t>(coordinate));
        ((void(__fastcall*)(void*, void*, int, int))
            savedVtable[0x26C / 4])(self, nullptr, position, 0);

        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, self, 0)[0x138 / 4])(self, nullptr, 13);
    }

    result = static_cast<int32_t>(self->field_0010);
    void* ownerLink = reinterpret_cast<void*>(static_cast<uint32_t>(result));
    void* ownerData = CSF_AT(void*, ownerLink, 0);
    void* owner = CSF_AT(void*, ownerData, 4);
    if (owner)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, owner, 0)[0x24 / 4])(owner, nullptr, 0);
    }
    return result;
}

HOOK(0x0072DB40, CGuiSlider_ProcessInput, "auto");
