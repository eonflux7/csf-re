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

// Session-side idle poll callee (__thiscall, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, sub_73EA00, 0x0073EA00, (void* self, void* edx, int a2));

// IAT slot for the Sleep-style import; called with one stack arg and no cleanup (stdcall).
CSF_GLOBAL(void*, g_SleepImportSlot, 0x0082D05C);

// Optional per-iteration callback pointer (called with no args).
CSF_GLOBAL(void*, g_IdlePollCallback, 0x008BC124);

// Completion flags.
CSF_GLOBAL(uint8_t, byte_8BC120, 0x008BC120);
CSF_GLOBAL(uint8_t, g_MenuMpCharacterSelectActive, 0x008BC121);
CSF_GLOBAL(uint8_t, byte_8A8924, 0x008A8924);
CSF_GLOBAL(uint8_t, byte_8A8925, 0x008A8925);

// Waits (polling) until the session dump completes, then publishes the completion flags.
static void __cdecl NetSession_WaitForDumpCompletion(void* self) {
    byte_8BC120 = 0;
    byte_8A8924 = 0;
    g_MenuMpCharacterSelectActive = 1;
    byte_8A8925 = 0;
    do {
        reinterpret_cast<void(__stdcall*)(uint32_t)>(g_SleepImportSlot)(50u);
        if (!byte_8BC120) {
            sub_73EA00(self, nullptr, 1028443341);
        }
        if (g_IdlePollCallback) {
            reinterpret_cast<void(__cdecl*)()>(g_IdlePollCallback)();
        }
    } while (!byte_8BC120);
    g_MenuMpCharacterSelectActive = 0;
    byte_8A8924 = 1;
    byte_8A8925 = 1;
}

HOOK(0x0073F000, NetSession_WaitForDumpCompletion, "auto");
