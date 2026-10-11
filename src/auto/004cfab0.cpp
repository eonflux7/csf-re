#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Fills a 3-dword record with the vtable of C_0082d448 and two fields of this object.
// Returns the record pointer (a2), as the original does.
static uint32_t* __fastcall HudAlertIcons_GetTargetAssignment(C_0083b4a0* self, void* /*edx*/, uint32_t* a2) {
    *reinterpret_cast<void***>(a2) = CSF_VTBL(0x0082D448);
    a2[1] = self->field_004c;
    a2[2] = self->field_0050;
    return a2;
}

HOOK(0x4CFAB0, HudAlertIcons_GetTargetAssignment, "auto");
