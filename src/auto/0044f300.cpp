#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Resets the scene loader's per-scene preparation state to its idle defaults.
static int __fastcall SceneLoader_ResetPreparation(S_0044f300* self, void* /*edx*/) {
    self->field_0070 = 0xFFFFFFFFu;
    self->field_00f8 = 0;
    int result = CSF_AT(int, GetGlobalValue_8BC1E0(), 0xA88);
    self->field_0124 = 0;
    self->field_0180 = 0;
    self->field_0144 = 0;
    self->field_0158 = 0;
    self->field_016c = 0;
    self->field_0128 = 0;
    self->field_0084 = 0;
    self->field_01a8 = 0;
    self->field_01a9 = 0;
    self->field_011c = static_cast<uint32_t>(result);
    self->field_0120 = 0xBF800000u;  // float -1.0f bit pattern
    self->field_01a4 = 0xFFFFFFFFu;
    self->field_0008 = 0;
    self->field_01ac = 0;
    self->field_01b0 = 0;
    return result;
}

HOOK(0x0044F300, SceneLoader_ResetPreparation, "auto");
