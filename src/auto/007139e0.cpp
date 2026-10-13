#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int16_t, __fastcall, CInputDirectInput_PollEvents, 0x007137E0,
         (void* self, void* edx, int a2));

static int __fastcall CInputDirectInput_DispatchEvents(
    CInputDirectInput* self, void* /*edx*/, int listenerAddress)
{
    CInputDirectInput_PollEvents(self, nullptr, listenerAddress);

    void* listener = reinterpret_cast<void*>(listenerAddress);
    auto* bytes = reinterpret_cast<uint8_t*>(self);
    uint8_t* keyStates = bytes + 0x230 + 0x900;

    using KeyCallback = void (__fastcall*)(void*, void*, int, uint8_t*);
    using EventCallback = void (__fastcall*)(void*, void*, uint8_t*);

    int keyEventIndex = 0;
    if (static_cast<int32_t>(self->field_0028) > 0)
    {
        uint8_t* keyEvent = bytes + 0x2C;
        do
        {
            CSF_AT(uint8_t, keyStates, CSF_AT(uint32_t, keyEvent, 0)) =
                CSF_AT(uint8_t, keyEvent, 4);

            if (CSF_AT(uint8_t, keyEvent, 4))
            {
                reinterpret_cast<KeyCallback>(CSF_AT(void**, listener, 0)[2])(
                    listener, nullptr, CSF_AT(int32_t, keyEvent, 0), keyStates);
            }
            else
            {
                reinterpret_cast<KeyCallback>(CSF_AT(void**, listener, 0)[3])(
                    listener, nullptr, CSF_AT(int32_t, keyEvent, 0), keyStates);
            }

            ++keyEventIndex;
            keyEvent += 8;
        }
        while (keyEventIndex < static_cast<int32_t>(self->field_0028));
    }

    int result = static_cast<int32_t>(self->field_022c);
    int eventIndex = 0;
    if (result > 0)
    {
        uint8_t* eventPayload = bytes + 0x234;
        do
        {
            switch (CSF_AT(uint32_t, eventPayload, -4))
            {
            case 0:
                if (CSF_AT(uint8_t, eventPayload, 0x1C))
                {
                    reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[5])(
                        listener, nullptr, eventPayload);
                }
                else
                {
                    reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[6])(
                        listener, nullptr, eventPayload);
                }
                break;

            case 1:
                if (CSF_AT(uint8_t, eventPayload, 0x1D))
                {
                    reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[7])(
                        listener, nullptr, eventPayload);
                }
                else
                {
                    reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[8])(
                        listener, nullptr, eventPayload);
                }
                break;

            case 2:
                if (CSF_AT(uint8_t, eventPayload, 0x1E))
                {
                    reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[9])(
                        listener, nullptr, eventPayload);
                }
                else
                {
                    reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[10])(
                        listener, nullptr, eventPayload);
                }
                break;

            case 3:
                reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[12])(
                    listener, nullptr, eventPayload);
                break;

            case 4:
                reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[13])(
                    listener, nullptr, eventPayload);
                break;

            case 5:
                reinterpret_cast<EventCallback>(CSF_AT(void**, listener, 0)[11])(
                    listener, nullptr, eventPayload);
                break;

            default:
                break;
            }

            result = eventIndex + 1;
            ++eventIndex;
            eventPayload += 0x24;
        }
        while (eventIndex < static_cast<int32_t>(self->field_022c));
    }

    return result;
}

HOOK(0x007139E0, CInputDirectInput_DispatchEvents, "auto");
