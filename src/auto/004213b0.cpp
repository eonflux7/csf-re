#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual slot 0x20 / 4 = 8 on the object stored at linked_object+0x48.
// Called as __thiscall: ecx = linked object, one stack arg = this task.
static int __fastcall C_0082f9f8_InvokeLinkedObjectHandler(C_0082f9f8* self, void* /*edx*/) {
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0014);
    uint32_t handler_raw = CSF_AT(uint32_t, owner, 0x48);
    void* handler = reinterpret_cast<void*>(handler_raw);

    int result = static_cast<int>(handler_raw);
    if (result != 0) {
        using HandlerFn = int(__fastcall*)(void* self, void* edx, C_0082f9f8* task);
        HandlerFn fn = reinterpret_cast<HandlerFn>(CSF_AT(void**, handler, 0)[8]);
        return fn(handler, nullptr, self);
    }
    return result;
}

HOOK(0x004213B0, C_0082f9f8_InvokeLinkedObjectHandler, "auto");
