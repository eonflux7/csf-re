#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: 0x0064B250, __thiscall-style (ecx = this), no stack args.
CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));

// Original: 0x00745430, __thiscall with two stack args (ret 8).
CSF_FUNC(char, __fastcall, CMenuScreen_ApplyPlayerStateRestore, 0x00745430,
         (void* self, void* edx, char a2, char a3));

// Restores the saved multiplayer player state into this stats screen.
// If the state check (Sub_64B250) passes, clears the pending flags, stores a3
// as the state value, resets the selection index to -1, and forwards the call
// to the base screen routine. Otherwise it only clears the flag at +0x749 and
// zeroes the four counters at +0x830..+0x83C.
static int __fastcall CMenuMpStats_RestorePlayerState(CMenuMpStats* self, void* /*edx*/,
                                                      char a2, int a3)
{
    if (Sub_64B250(self, nullptr))
    {
        CSF_AT(uint8_t, self, 0x6F2) = 0;
        CSF_AT(uint8_t, self, 0x749) = 0;
        CSF_AT(int, self, 0x6EC) = a3;
        CSF_AT(uint8_t, self, 0x6F0) = 1;

        int currentIndex = CSF_AT(int, self, 0x668);
        if (currentIndex != -1)
            CSF_AT(int, self, 0x668) = -1;

        char result = CMenuScreen_ApplyPlayerStateRestore(self, nullptr, a2, static_cast<char>(a3));
        return static_cast<int>(result);
    }

    CSF_AT(uint8_t, self, 0x749) = 0;
    CSF_AT(int, self, 0x830) = 0;
    CSF_AT(int, self, 0x834) = 0;
    CSF_AT(int, self, 0x838) = 0;
    CSF_AT(int, self, 0x83C) = 0;
    return 0;
}

HOOK(0x740B90, CMenuMpStats_RestorePlayerState, "auto");
