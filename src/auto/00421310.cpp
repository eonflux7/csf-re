#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, sub_48E5D0, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));

// Reacts to a task-state notification: when the linked owner reports active, either
// forwards the event to the owner's child object (flag set) or, when the flag is set,
// tries to clear the pending state through sub_48E5D0 (flag cleared).
static void __fastcall C_0082f9f8_UpdateTaskStateFlags(C_0082f9f8* self, void* /*edx*/, int a2)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
    if (owner == nullptr)
        return;
    if (((int(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[32])(owner, nullptr) == 0)
        return;
    if (*reinterpret_cast<C_0082f9f8**>(a2) == self)
        return;

    if (CSF_AT(uint8_t, reinterpret_cast<uint8_t*>(a2), 4) != 0) {
        uint8_t* taskBase = reinterpret_cast<uint8_t*>(self->field_0014);
        void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, taskBase, 0x48));
        if (child != nullptr) {
            ((void(__fastcall*)(void*, void*, C_0082f9f8*))CSF_AT(void**, child, 0)[8])(child, nullptr, self);
        }
        CSF_AT(uint8_t, self, 0x2b8) = 1;
    } else if (CSF_AT(uint8_t, self, 0x2b8) != 0) {
        void* taskObj = reinterpret_cast<void*>(self->field_0014);
        if (sub_48E5D0(taskObj, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(self)), 2, 1) != 0) {
            CSF_AT(uint8_t, self, 0x2b8) = 0;
            CSF_AT(uint32_t, self, 0x238) = 0;
        }
    }
}

HOOK(0x00421310, C_0082f9f8_UpdateTaskStateFlags, "auto");
