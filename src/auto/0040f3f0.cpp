#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082ddf8_HandleFlag19Action, 0x0040EC10, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082ddf8_HandleFlag1aAction, 0x0040EED0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0082ddf8_PrepareDispatch, 0x0040F320, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* edx, int a2));

// Forwards the event to the child object, updates from the event, then runs the
// flag-driven follow-up actions when the child reports ready (byte +0x10).
static char __fastcall C_0082ddf8_DispatchEventAndUpdate(C_0082ddf8* self, void* /*edx*/, int a2)
{
    char result = static_cast<char>(self->field_000c);
    if (result)
    {
        void* child = reinterpret_cast<void*>(self->field_001c);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[5])(child, nullptr, a2);

        C_0082e520_UpdateFromEvent(reinterpret_cast<C_0082e520*>(self), nullptr, a2);

        result = static_cast<char>(CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_001c), 0x10));
        if (result)
        {
            if (self->field_0019 || self->field_001a)
                C_0082ddf8_PrepareDispatch(self, nullptr);

            if (self->field_0019)
                C_0082ddf8_HandleFlag19Action(self, nullptr);

            result = static_cast<char>(self->field_001a);
            if (result)
                return C_0082ddf8_HandleFlag1aAction(self, nullptr);
        }
    }
    return result;
}

HOOK(0x0040F3F0, C_0082ddf8_DispatchEventAndUpdate, "auto");
