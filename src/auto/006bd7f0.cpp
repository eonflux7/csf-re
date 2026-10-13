#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (by original address).
CSF_FUNC(char, __fastcall, SceneObject_ApplyFlag1Pass, 0x006BCC20, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, SceneObject_ApplyFlag2Pass, 0x006BD5E0, (void* self, void* edx));

// Renders the world for a scene object. Returns the low byte of the value the
// original leaves in eax (al): field_0004 when no pass runs, otherwise the
// result of the last pass executed.
static char __fastcall S_006bc310_RenderWorld(S_006bc310* self, void* /*edx*/)
{
    if (self->field_0091 == 0) {
        return 0;
    }
    if (self->field_0004 == 0) {
        return 0;
    }

    char result = static_cast<char>(self->field_0004);
    bool hasFlag1 = (self->field_008c & 1) != 0;
    self->field_00b0 = 0;
    if (hasFlag1) {
        result = SceneObject_ApplyFlag1Pass(self, nullptr);
    }
    if ((self->field_008c & 2) != 0) {
        // Original tail-jumps here with ecx = this; its result is the return value.
        return static_cast<char>(SceneObject_ApplyFlag2Pass(self, nullptr));
    }
    return result;
}

HOOK(0x006BD7F0, S_006bc310_RenderWorld, "auto");
