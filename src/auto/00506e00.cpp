#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());

// Polls a virtual predicate (primary vtable slot 3, byte offset 0x0C); if it reports true,
// runs SceneLoader_NoOp and clears the flag at +0x08.
static void __fastcall CNetProfile_PollAndClearFlag(CNetProfile* self, void* /*edx*/)
{
    // Virtual call through primary vtable slot 3 (byte offset 0x0C), no arguments.
    auto pollReady = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[3]);
    int ready = pollReady(self, nullptr);

    if (static_cast<uint8_t>(ready))
    {
        SceneLoader_NoOp();
        self->field_0008 = 0;
    }
}

HOOK(0x00506E00, CNetProfile_PollAndClearFlag, "auto");
