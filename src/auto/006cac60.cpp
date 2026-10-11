#include <cstdint>
#include "globals.h"
#include "hook.h"

// Calls the subsystem's update routine when the subsystem pointer at +0xBC is set.
CSF_FUNC(char, __fastcall, GfxScene_RunSubsystemUpdate, 0x006D5A40, (void* self, void* edx, int a2));

static char __fastcall GfxScene_ProcessSubsystemIfPresent(void* self, void* /*edx*/, int a2)
{
    void* subsystem = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0xBC)));
    if (subsystem != nullptr)
        GfxScene_RunSubsystemUpdate(subsystem, nullptr, a2);
    return 1;
}

HOOK(0x006CAC60, GfxScene_ProcessSubsystemIfPresent, "auto");
