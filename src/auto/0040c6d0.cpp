#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* /*edx*/, int a2));

// Forwards an event to the child object at +0x10 when the enable flag at +0x0C is set,
// then runs the local update. Returns the enable flag value (0 when disabled).
static char __fastcall C_0082db10_ForwardEventIfEnabled(C_0082db10* self, void* /*edx*/, int a2)
{
    uint8_t result = self->field_000c;
    if (result)
    {
        void* child = reinterpret_cast<void*>(self->field_0010);
        // Slot +0x14 of the child's vtable, called __thiscall with one stack argument.
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[5])(child, nullptr, a2);
        return C_0082e520_UpdateFromEvent(reinterpret_cast<C_0082e520*>(self), nullptr, a2);
    }
    return static_cast<char>(result);
}

HOOK(0x0040C6D0, C_0082db10_ForwardEventIfEnabled, "auto");
