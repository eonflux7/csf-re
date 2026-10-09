#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original sub_5018D0 (bound as in the packet). Called with ecx = this, no stack args.
CSF_FUNC(unsigned int, __fastcall, sub_5018D0, 0x005018D0, (int a1, int a2));

// Applies a load request to a scene loader: stores the request at +0x100, runs the
// loader refresh routine, then records the loader state (1 for request type 0,
// 2 for request type 2). Other request types are ignored.
static void __fastcall SceneLoader_ApplyLoadRequest(void* self, void* /*edx*/, int a2)
{
    if (a2) {
        int requestType = CSF_AT(int, reinterpret_cast<void*>(a2), 4);
        if (requestType == 0) {
            CSF_AT(int, self, 0x100) = a2;
            sub_5018D0(reinterpret_cast<int>(self), 0);
            CSF_AT(int, self, 0x24) = 1;
        } else if (requestType == 2) {
            CSF_AT(int, self, 0x100) = a2;
            sub_5018D0(reinterpret_cast<int>(self), 0);
            CSF_AT(int, self, 0x24) = 2;
        }
    }
}

HOOK(0x00501940, SceneLoader_ApplyLoadRequest, "auto");
