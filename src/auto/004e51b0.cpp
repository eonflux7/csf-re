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

// Stores the activation flag at +0x4C. When the flag is cleared, the object's
// vtable slot 0xB0 (slot 44) is invoked and its result is returned.
static char __fastcall SceneEntity_Activate(uint8_t* self, void* /*edx*/, char a2)
{
    char result = a2;
    CSF_AT(char, self, 0x4C) = a2;
    if (!a2) {
        return ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xB0 / 4])(self, nullptr);
    }
    return result;
}

HOOK(0x004E51B0, SceneEntity_Activate, "auto");
