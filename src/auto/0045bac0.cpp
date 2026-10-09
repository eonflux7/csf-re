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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __fastcall, ApplyFloatToGlobalState, 0x0050DC30, (void* self, void* edx, char a2, float a3));

// Reads a float two pointer hops down from the handle at a3, then passes it
// along with the constant 1 to the global-state applier.
static int __stdcall ForwardHandleFloatToGlobalState(int a1, int a2, int a3)
{
    // a3 -> pointer to outer object; outer[+4] -> inner object; inner[+4] -> float value.
    uint32_t* outerObject = reinterpret_cast<uint32_t*>(*reinterpret_cast<uint32_t*>(a3));
    uint32_t* innerObject = reinterpret_cast<uint32_t*>(outerObject[1]);
    float valueFloat = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(innerObject) + 4);

    GetGlobalState_50D190();
    ApplyFloatToGlobalState(nullptr, nullptr, 1, valueFloat);
    return 0;
}

HOOK(0x0045BAC0, ForwardHandleFloatToGlobalState, "auto");
