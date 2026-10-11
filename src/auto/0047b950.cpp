#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Virtual slot 9 (+0x24) on the firing-state object: returns a byte flag, takes one int argument.
// Virtual slot 35 (+0x8C) on the owner host: fire/start call with five stack arguments (ret 0x14).
// Virtual slot 9 (+0x24) on the aim sub-object at self[3]+0x88: returns an int, takes one int argument.
// Virtual slot 45 (+0xB4) on the aim target: returns an int, takes one int argument.
static int __fastcall AiAttackTasks_UpdateFiringAndAim(int* self, void* /*edx*/)
{
    // self[2] is the owning task-state object.
    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x8)));

    // owner->host (+0x14), then host->firing object (+0x48).
    uint8_t* ownerHost = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, owner, 0x14)));
    void* firingState = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ownerHost, 0x48)));

    // Query the firing state (slot 9) with argument 2 and store the byte result in owner->+0x2EC.
    uint8_t canFire = ((uint8_t(__fastcall*)(void*, void*, int))CSF_AT(void**, firingState, 0)[9])(
        firingState, nullptr, 2);
    CSF_AT(uint8_t, owner, 0x2EC) = canFire;

    if (CSF_AT(uint8_t, owner, 0x2EC) != 0)
    {
        // Start firing on the host (slot 35): four stack arguments. The `push edi` before them is the
        // callee-save of edi (popped at the end), not a fifth argument.
        ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, ownerHost, 0)[35])(
            ownerHost, nullptr, 0, 0, -1, 0);

        // self[3] is the aim-side object.
        uint8_t* aimBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0xC)));

        // host->+0x30 is the aim target object.
        uint8_t* aimTarget = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ownerHost, 0x30)));

        // Sub-object at aimBase+0x88: query slot 9 with argument 0.
        void* aimSub = aimBase + 0x88;
        int aimValue = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, aimSub, 0)[9])(
            aimSub, nullptr, 0);

        // Set the aim value on the target (slot 45) and return its result.
        return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, aimTarget, 0)[45])(
            aimTarget, nullptr, aimValue);
    }

    // Not ready to fire: the original returns the owner pointer it loaded at entry.
    return CSF_AT(int, self, 0x8);
}

HOOK(0x0047B950, AiAttackTasks_UpdateFiringAndAim, "auto");
