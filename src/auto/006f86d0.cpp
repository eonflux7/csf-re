#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, GfxScreenEffects_ProcessEntry, 0x006F8680, (void* self, void* edx, int a2));

static uint32_t* __stdcall GfxScreenEffects_ProcessEntryIfNonNull(
    uint32_t* entry, uint32_t* argument)
{
    if (entry)
        return GfxScreenEffects_ProcessEntry(
            entry, nullptr, static_cast<int>(reinterpret_cast<intptr_t>(argument)));
    return nullptr;
}

HOOK(0x006F86D0, GfxScreenEffects_ProcessEntryIfNonNull, "auto");
