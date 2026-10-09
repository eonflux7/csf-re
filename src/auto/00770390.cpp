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

// Callee at 0x007702E0: thiscall with 18 stack dwords (72 bytes), bound as __fastcall
// (self in ecx, edx unused). Name chosen here; signature kept from the packet.
CSF_FUNC(char, __fastcall, InputBindings_ApplyActionEntry, 0x007702E0,
         (void* self, void* edx, unsigned int a2, char a3, int a4, int a5, int a6,
          int a7, int a8, int a9, void* Block, int a11, int a12, int a13, int a14,
          unsigned int a15, int a16, int a17, int a18, int a19));

// Registers an input action (ATACAR/DISPARAR/RECARGAR/AGACHARSE/STRAFE family) with its
// default key/mouse configuration by forwarding a fixed parameter block to
// InputBindings_ApplyActionEntry. Only the action id (a2) varies by caller.
static char __fastcall InputBindings_BindActionWithDefaults(S_0076a4f0* self, void* /*edx*/, unsigned int actionId)
{
    return InputBindings_ApplyActionEntry(self, nullptr, actionId,
                                          -1, 0, 5376, 22, 0, 23, 0,
                                          nullptr, 0, 0, 0, 0,
                                          0xFu, 20, 0, 0, 0);
}

HOOK(0x00770390, InputBindings_BindActionWithDefaults, "auto");
