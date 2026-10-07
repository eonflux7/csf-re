#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static std::uint8_t __fastcall CGuiControl_FireEvent(
    CGuiControl* self, void* /*edx*/, int eventId, int eventArgument)
{
    std::uint8_t result = self->m_bFiringEvent;
    if (result != 0)
        return result;

    int eventData[2];
    eventData[0] = eventId;
    self->m_bFiringEvent = 1;

    const std::uint32_t initialBegin = self->m_listenersBegin;
    eventData[1] = eventArgument;

    std::uint32_t listenerCount = 0;
    if (initialBegin != 0)
    {
        const std::uint32_t byteDifference =
            self->m_listenersEnd - initialBegin;
        listenerCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(byteDifference) >> 2);
    }

    // Without a callback, AL retains the low byte of the initial begin pointer.
    result = static_cast<std::uint8_t>(initialBegin);

    for (std::uint32_t index = 0; index < listenerCount; ++index)
    {
        void* listener =
            reinterpret_cast<void**>(self->m_listenersBegin)[index];

        result = static_cast<std::uint8_t>(
            reinterpret_cast<int (__fastcall*)(void*, void*, int*)>(
                CSF_AT(void**, listener, 0)[1])(
                    listener, nullptr, eventData));
    }

    self->m_bFiringEvent = 0;

    CGuiControl* parent =
        reinterpret_cast<CGuiControl*>(self->m_pParent);
    if (parent != nullptr)
    {
        result = static_cast<std::uint8_t>(
            reinterpret_cast<int (__fastcall*)(CGuiControl*, void*, int, int)>(
                parent->vtbl[0x218 / 4])(
                    parent, nullptr, eventId, eventArgument));
    }

    return result;
}

HOOK(0x00719A40, CGuiControl_FireEvent, "auto");
