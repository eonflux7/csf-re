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

// Stores the activation flag at +0x40. When the flag is cleared, the object's
// vtable slot 0x8C (slot 35) is invoked and its result is returned.
static char __fastcall SetSecondaryEntityActive(uint8_t* self, void* /*edx*/, char a2)
{
    char result = a2;
    CSF_AT(char, self, 0x40) = a2;
    if (!a2) {
        return ((char(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x8C / 4])(self, nullptr);
    }
    return result;
}

HOOK(0x004F0C30, SetSecondaryEntityActive, "auto");
