#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound callees (original addresses).
CSF_FUNC(bool, __fastcall, MenuControls_QueryBinding, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));

// Returns true if either the binding query or the flag query for a2 succeeds.
// Both queries use the context object stored at this->field_0014.
static int __fastcall C_00830578_TestBindingOrFlag(C_00830578* self, void* /*edx*/, int a2)
{
    if (MenuControls_QueryBinding(reinterpret_cast<void*>(self->field_0014), nullptr, a2))
        return 1;
    return MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, a2) ? 1 : 0;
}

HOOK(0x0042B2A0, C_00830578_TestBindingOrFlag, "auto");
