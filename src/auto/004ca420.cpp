// Reimplementation of 0x004CA420 (ai_cover_attack_tasks).
// Resolves the linked object, zeroes two float timers at +0x3C0/+0x3C4,
// sets the flag at +0x3B8, sets both timers to 0.1f, then forwards to 0x004C82D0.
#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttack_RunPostInitStep, 0x004C82D0, (void* self, void* edx, int a1));

static int __fastcall AiCoverAttack_InitApproachTimers(void* self, void* /*edx*/, int a1)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(
        reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);

    uint8_t* target = nullptr;
    if (linkedValue != 0)
        target = reinterpret_cast<uint8_t*>(linkedValue) - 4;

    CSF_AT(int, target, 0x3C0) = 0;
    CSF_AT(int, target, 0x3C4) = 0;
    CSF_AT(int, target, 0x3B8) = 1;
    CSF_AT(int, target, 0x3C0) = 1036831949;  // 0.1f bit pattern
    CSF_AT(int, target, 0x3C4) = 1036831949;  // 0.1f bit pattern

    return AiCoverAttack_RunPostInitStep(self, nullptr, a1);
}

HOOK(0x4CA420, AiCoverAttack_InitApproachTimers, "auto");
