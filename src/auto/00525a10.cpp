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

// Per-element handler: returns nonzero when the sound was handled.
CSF_FUNC(int, __fastcall, sub_7707E0, 0x007707E0, (void* self, void* edx, float* a2));

// Walks a list of sound-instance pointers (list[0] = element array, list[2] = count)
// and passes the context object to each element until one reports it was handled.
static char __cdecl AudioSound_DispatchUntilHandled(S_00770760* context, uint32_t* list)
{
    char result = 0;
    if (list)
    {
        unsigned int count = list[2];
        S_00770760** elements = reinterpret_cast<S_00770760**>(list[0]);
        for (unsigned int i = 0; i < count; ++i)
        {
            if (result)
                break;
            S_00770760* element = elements[i];
            if (element)
                result = static_cast<char>(sub_7707E0(element, nullptr, reinterpret_cast<float*>(context)));
        }
    }
    return result;
}

HOOK(0x525A10, AudioSound_DispatchUntilHandled, "auto");
