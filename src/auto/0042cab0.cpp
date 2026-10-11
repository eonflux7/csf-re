#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Binds to the original callees by address.
CSF_FUNC(int, __fastcall, C_00830578_ReleaseMenuField, 0x00428820, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_00830578_RebindDefaultController, 0x0042CA40, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());

// Resets the menu controller of a CMenuScreen-derived multiplayer pause menu.
// Releases the menu's field object, then reads the global object's flag at +0x20.
// If that flag is zero, it tail-jumps into the default controller rebind routine;
// otherwise it returns the low byte of the global object pointer.
static char __fastcall C_00830578_ResetMenuController(C_00830578* self, void* /*edx*/)
{
    C_00830578_ReleaseMenuField(reinterpret_cast<void*>(self->field_0014), nullptr);

    int globalObjectValue = GetGlobalValue_8B0568();
    int flagAt20 = CSF_AT(int, reinterpret_cast<void*>(globalObjectValue), 0x20);
    if (flagAt20 == 0) {
        return C_00830578_RebindDefaultController(self, nullptr);
    }

    return static_cast<char>(globalObjectValue);
}

HOOK(0x0042CAB0, C_00830578_ResetMenuController, "auto");
