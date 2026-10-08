#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Child text setter at 0x004370E0: __thiscall(this, char* Source), pops 4.
CSF_FUNC(int, __fastcall, CMenuMissionInfo_ChildSetText, 0x004370E0, (void* self, void* edx, char* source));

// Returns the length of the string when it is empty (0); otherwise forwards
// the string to the child object stored at +0x84 when that child exists and
// returns the child's result. If the child is missing, returns the length.
static int __fastcall CMenuMissionInfo_SetTextIfNonEmpty(void* self, void* /*edx*/, char* source)
{
    // Inline strlen (the original has no call to the CRT).
    int length = 0;
    while (source[length] != '\0') {
        ++length;
    }

    if (length != 0) {
        void* childObject = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x84));
        if (childObject != nullptr) {
            return CMenuMissionInfo_ChildSetText(childObject, nullptr, source);
        }
    }
    return length;
}

HOOK(0x0053D4B0, CMenuMissionInfo_SetTextIfNonEmpty, "auto");
