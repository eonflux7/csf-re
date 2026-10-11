#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, ForwardInputEventToDelegate, 0x00491FB0, (void* self, void* , char event, int value));
CSF_FUNC(char, __fastcall, GetLinkedObjectStateForwarder, 0x00491FC0, (void* self, void*));
CSF_FUNC(void, __fastcall, sub_4A8C30, 0x004A8C30, (void* self, void* edx, int a2));

// Dispatches an input event record: type 40 forwards (event, value) to the delegate unless
// forwarding is linked; type 41 links the forwarder and sets the flag; anything else goes to sub_4A8C30.
static void __fastcall C_0083b3c0_HandleEvent(C_0083b3c0* self, void* /*edx*/, int a2)
{
    uint8_t* eventRecord = reinterpret_cast<uint8_t*>(a2);
    const int eventType = CSF_AT(int, eventRecord, 0);

    if (eventType == 40)
    {
        if (!self->field_0044)
        {
            const char eventCode = static_cast<char>(CSF_AT(uint8_t, eventRecord, 4));
            const int eventValue = CSF_AT(uint8_t, eventRecord, 5);
            ForwardInputEventToDelegate(reinterpret_cast<void*>(self->field_0014), nullptr, eventCode, eventValue);
        }
    }
    else if (eventType == 41)
    {
        GetLinkedObjectStateForwarder(reinterpret_cast<void*>(self->field_0014), nullptr);
        self->field_0044 = 1;
    }
    else
    {
        sub_4A8C30(self, nullptr, a2);
    }
}

HOOK(0x004CF6D0, C_0083b3c0_HandleEvent, "auto");
