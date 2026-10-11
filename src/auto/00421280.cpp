#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4DB110, 0x004DB110, (void* self, void* edx));

// Runs the base update step, then asks the owner's state holder (owner+0x14 -> +0x48)
// to dispatch. If that returns the owner itself, forwards the owner to its vtable slot 15.
static int __fastcall TFncGirar_RunUpdateStep(void* self, void* /*edx*/)
{
    sub_4DB110(self, nullptr);

    uint32_t owner = CSF_AT(uint32_t, self, 0x08);
    void* ownerObj = reinterpret_cast<void*>(owner);

    void* stateHolder = CSF_AT(void*, ownerObj, 0x14);
    void* dispatchTask = CSF_AT(void*, stateHolder, 0x48);

    // Vtable slot 0x40 / 4 = 16: returns a value compared against the owner.
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, dispatchTask, 0)[16])(dispatchTask, nullptr);

    if (result == static_cast<int>(owner)) {
        void* forwardHolder = CSF_AT(void*, ownerObj, 0x14);
        void* forwardTask = CSF_AT(void*, forwardHolder, 0x48);
        // Vtable slot 0x3C / 4 = 15: takes the owner as its single argument.
        return ((int(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, forwardTask, 0)[15])(forwardTask, nullptr, owner);
    }
    return result;
}

HOOK(0x00421280, TFncGirar_RunUpdateStep, "auto");
