#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee at 0x790F30: tests a channel handle and returns a bool (checked with test al, al).
CSF_FUNC(bool, __fastcall, ChannelIsOpen, 0x00790F30, (void* self, void* edx));

// Sets the 'activated' flag once the state is one of the ready values and the channel is not yet open.
static char __fastcall C_0085fe28_TryActivateSession(C_0085fe28* self, void* /*edx*/)
{
    uint32_t state = self->field_0008;
    uint8_t resultByte = static_cast<uint8_t>(state);

    if (state == 2 || state == 3 || state == 7)
    {
        // Returns the low byte of the call's result; the original tests al.
        resultByte = ChannelIsOpen(reinterpret_cast<void*>(self->field_0028), nullptr) ? 1 : 0;

        if (resultByte == 0)
        {
            // Virtual slot 0x44 (index 17), called with a single stack argument 0.
            ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, self, 0)[0x44 / 4]))(self, nullptr, 0);
            self->field_0338 = 1;
        }
    }

    return static_cast<char>(resultByte);
}

HOOK(0x006A7A70, C_0085fe28_TryActivateSession, "auto");
