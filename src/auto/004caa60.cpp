#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callees (bound by address)
CSF_FUNC(int, __stdcall, CoverTask_InitBase, 0x004C95B0, (int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));

// Returns the linked object's base address (linked value - 4), or 0 when no link exists.
// GetLinkedObjectValue is called on every use, as the original re-reads it each time.
static int CoverAttack_LinkedBaseAddress(int a1)
{
    int linked = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a1)), nullptr);
    if (linked)
        return linked - 4;
    return 0;
}

static uint8_t* CoverAttack_AsPointer(int address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

static int __stdcall CoverAttack_ResetAimTimers(int a1)
{
    CoverTask_InitBase(a1);

    // Sub-object at +0x258 of the linked base: call its vtable slot 10 (offset 0x28).
    int subBase = CoverAttack_LinkedBaseAddress(a1);
    uint8_t* subObject = CoverAttack_AsPointer(subBase) + 0x258;
    auto sliceCall = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, subObject, 0)[0x28 / 4]);
    sliceCall(subObject, nullptr, a1);

    // Clear the flag byte at +0x26D.
    int flagBase = CoverAttack_LinkedBaseAddress(a1);
    uint8_t* flagObject = CoverAttack_AsPointer(flagBase);
    CSF_AT(uint8_t, flagObject, 0x26D) = 0;

    // Set both timers at +0x264 and +0x268 to 0.1f.
    int timerBase = CoverAttack_LinkedBaseAddress(a1);
    uint8_t* timerObject = CoverAttack_AsPointer(timerBase);
    CSF_AT(float, timerObject, 0x264) = 0.1f;
    CSF_AT(float, timerObject, 0x268) = 0.1f;

    return timerBase;
}

HOOK(0x004CAA60, CoverAttack_ResetAimTimers, "auto");
