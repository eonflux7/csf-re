#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, GetContainerDataPtr, 0x00721270,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90,
         (std::uint32_t* self, void* edx, std::int16_t* text));

static void __fastcall CMenuTextInput_HandleButtonEvent(
    CMenuTextInput* self, void* /*edx*/, std::uint32_t* event)
{
    if (event[0] != 13)
        return;

    void* parent;
    const std::uint32_t acceptButton = self->field_0028;
    if (acceptButton != 0 && acceptButton == event[1])
    {
        void* textContainer = reinterpret_cast<void*>(self->field_0024);
        self->field_0030 = 1;

        if (textContainer != nullptr)
        {
            std::uint32_t* text = GetContainerDataPtr(textContainer, nullptr);
            MenuProgressInput_AssignText(
                reinterpret_cast<std::uint32_t*>(
                    reinterpret_cast<std::uint8_t*>(self) + 0x34),
                nullptr, reinterpret_cast<std::int16_t*>(text));
        }

        parent = reinterpret_cast<void*>(self->field_0010);
    }
    else
    {
        const std::uint32_t cancelButton = self->field_002c;
        if (cancelButton == 0 || cancelButton != event[1])
            return;

        parent = reinterpret_cast<void*>(self->field_0010);
        self->field_0030 = 0;
    }

    if (parent != nullptr)
    {
        self->field_0054 = 0;
        reinterpret_cast<void (__fastcall*)(void*, void*, CMenuTextInput*, int)>(
            CSF_AT(void**, parent, 0)[0x60 / 4])(
                parent, nullptr, self, 0);
    }
}

HOOK(0x0052BEA0, CMenuTextInput_HandleButtonEvent, "auto");
