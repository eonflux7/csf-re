#include <cstdint>
#include "globals.h"
#include "core/memory.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatches an event to a sub-object first, then to a child array.
// Returns 1 if the event was handled by the sub-object or any child, 0 otherwise.
static char __fastcall C_00861d48_DispatchEventToChildren(C_00861d48* self, void* /*edx*/, int a2, int a3, int a4)
{
    // Virtual slot 0x58, no arguments.
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x58 / 4])(self, nullptr);

    // Primary sub-object at +0x24: its slot 0x10 returns whether it accepted the event.
    void* primary = CSF_AT(void*, self, 0x24);
    bool primaryAccepted = ((bool (__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, primary, 0)[0x10 / 4])(primary, nullptr, a2, a3, a4);
    if (!primaryAccepted)
        return 0;

    // Virtual slot 0x5c, no arguments.
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x5c / 4])(self, nullptr);

    uint32_t childCount = CSF_AT(uint32_t, self, 0x30);
    if (childCount == 0)
        return 0;

    char handled = 0;
    uint32_t index = 0;
    do {
        // Re-read the child array base each iteration, as the original does.
        uint8_t* childArray = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x28));
        void* child = CSF_AT(void*, childArray, index * 16);
        bool childHandled = ((bool (__fastcall*)(void*, void*, int, int, int))CSF_AT(void**, child, 0)[0x10 / 4])(child, nullptr, a2, a3, a4);
        handled = childHandled ? 1 : 0;
        ++index;
    } while (index < CSF_AT(uint32_t, self, 0x30) && !handled);

    return handled;
}

HOOK(0x6E6CD0, C_00861d48_DispatchEventToChildren, "auto");
