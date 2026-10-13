#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, C_0085ec18_ResetSession, 0x004F9990, (C_0085ec18* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_FindParticipant, 0x0073E420, (int (__stdcall *participantHandle)(int)));

// Ticks down the float timer at +0xFC by deltaSeconds. When it has expired, it is
// re-armed to 0.5 and the flag at +0xE0 is toggled. Then the participant handle at
// +0x104 is looked up; if it is missing, the session is reset.
static char __fastcall ApplyFloatValue_4F9B00(C_0085ec18* self, void* /*edx*/, float deltaSeconds)
{
    if (CSF_AT(uint8_t, self, 0x10) == 0)
        return 0;

    if (CSF_AT(float, self, 0xFC) <= g_fZero)
    {
        uint8_t toggledFlag = CSF_AT(uint8_t, self, 0xE0);
        CSF_AT(float, self, 0xFC) = 0.5f;
        CSF_AT(uint8_t, self, 0xE0) = (toggledFlag == 0) ? 1 : 0;
    }
    else
    {
        CSF_AT(float, self, 0xFC) = CSF_AT(float, self, 0xFC) - deltaSeconds;
    }

    int handle = CSF_AT(int, self, 0x104);
    int participant = handle;
    if (handle != -1)
    {
        GetGlobalDword_73C020();
        participant = NetSession_FindParticipant(
            reinterpret_cast<int (__stdcall*)(int)>(static_cast<uintptr_t>(handle)));
        if (participant == 0)
            participant = C_0085ec18_ResetSession(self, nullptr);
    }
    return static_cast<char>(participant);
}

HOOK(0x004F9B00, ApplyFloatValue_4F9B00, "auto");
