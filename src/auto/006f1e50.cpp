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

// Callee bindings (by address)
CSF_FUNC(void, __fastcall, GfxObject_UpdateAttachment, 0x006E50C0, (float* self, void* edx, float deltaTime));
CSF_FUNC(char, __fastcall, C_00862490_FlushPending, 0x006F1C60, (void* self, void* edx));

// Per-frame update: refreshes the attached graphics object, flushes pending state,
// and if the attached object reports idle, sets the flag at slot 0x2C on this object.
static char __fastcall C_00862490_Update(C_00862490* self, void* /*edx*/, float deltaTime)
{
    char result = self->vCheckGlobalObjectFlag();
    if (!result)
    {
        GfxObject_UpdateAttachment(reinterpret_cast<float*>(self->field_0040), nullptr, deltaTime);

        if (self->field_0024)
            C_00862490_FlushPending(self, nullptr);

        result = static_cast<char>(self->field_0028);
        if (result)
        {
            void* attachedObject = reinterpret_cast<void*>(self->field_0040);
            result = ((char(__fastcall*)(void*, void*))CSF_AT(void**, attachedObject, 0)[0x1c / 4])(attachedObject, nullptr);
            if (!result)
            {
                result = ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x2c / 4])(self, nullptr, 1);
            }
        }
    }
    return result;
}

HOOK(0x006F1E50, C_00862490_Update, "auto");
