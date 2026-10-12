#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, sub_40D1A0, 0x0040D1A0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_40D480, 0x0040D480, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, C_0082e520_UpdateFromEvent, 0x0041E090, (C_0082e520* self, void* , int));

// Flushes a pending update: if the dirty byte at +0x0C is set, runs the
// pre-update hook, notifies the object at +0x30 through its vtable slot 5,
// refreshes state, and clears the four-dword block at +0x20.
static char __fastcall C_0082dc88_FlushPendingUpdate(C_0082dc88* self, void* /*edx*/, float a2)
{
    char result = static_cast<char>(self->field_000c);
    if (result)
    {
        sub_40D1A0(self, nullptr);

        void* notifyTarget = reinterpret_cast<void*>(self->field_0030);
        ((void(__fastcall*)(void*, void*, float))CSF_AT(void**, notifyTarget, 0)[0x14 / 4])(notifyTarget, nullptr, a2);

        sub_40D480(self, nullptr, a2);

        int32_t eventBits;
        std::memcpy(&eventBits, &a2, sizeof(eventBits));
        C_0082e520_UpdateFromEvent(static_cast<C_0082e520*>(self), nullptr, eventBits);

        CSF_AT(uint32_t, self, 0x20) = 0;
        CSF_AT(uint32_t, self, 0x24) = 0;
        CSF_AT(uint32_t, self, 0x28) = 0;
        CSF_AT(uint32_t, self, 0x2C) = 0;
        return 0;
    }
    return result;
}

HOOK(0x0040D570, C_0082dc88_FlushPendingUpdate, "auto");
