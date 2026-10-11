#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void* edx, int value));

// Prepares the char task, decrements the counter held by the owner's
// controller object (owner+0x14 -> +0x204), then clears the owner's
// flag byte at +0x33C.
static int __fastcall TCharTask_PrepareAndDecrementCounter(void* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    uint32_t ownerValue = CSF_AT(uint32_t, self, 0x8);
    void* owner = reinterpret_cast<void*>(ownerValue);

    uint32_t controllerValue = CSF_AT(uint32_t, owner, 0x14);
    void* controller = reinterpret_cast<void*>(controllerValue);

    uint32_t counterOwnerValue = CSF_AT(uint32_t, controller, 0x204);
    void* counterOwner = reinterpret_cast<void*>(counterOwnerValue);

    int result = C_DecrementCounterAt30(counterOwner, nullptr, static_cast<int>(ownerValue));

    uint8_t* ownerAfter = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    CSF_AT(uint8_t, ownerAfter, 0x33C) = 0;

    return result;
}

HOOK(0x004D7220, TCharTask_PrepareAndDecrementCounter, "auto");
