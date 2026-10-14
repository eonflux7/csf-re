#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(int, __fastcall, CMenuMpSessionsPC_SubmitSessionSettings, 0x005617D0, (void* self, void* edx));

// Submits the session settings when the game settings' flag +0xCC is set.
static int __fastcall CMenuMpSessionsPC_SubmitIfEnabled(void* self, void* /*edx*/)
{
    const int settings = GetGlobalValue_75FDF0();
    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(settings), 0xCC))
        return CMenuMpSessionsPC_SubmitSessionSettings(self, nullptr);
    return settings;
}

HOOK(0x00566010, CMenuMpSessionsPC_SubmitIfEnabled, "auto");
