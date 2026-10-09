#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_44C5A0, 0x0044C5A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(void*, __fastcall, DefaultDestination_Get, 0x004FE4A0, (void* self, void* edx));

// Reads a float at a3 -> *(a3) -> +4 -> +4, truncates it to int, and passes it
// as the single argument to the destination's handler.
static int __stdcall DispatchFloatFieldToDefaultDestination(int a1, int a2, int a3)
{
    (void)a1;
    (void)a2;

    uint32_t* outerRef = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3));
    uint32_t* chainA = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(outerRef[0]));
    uint32_t* chainB = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(chainA[1]));
    float ratio = *reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(chainB) + 4);
    int truncatedRatio = static_cast<int>(ratio);

    int globalValue = GetGlobalValue_8B0568();
    void* destination = DefaultDestination_Get(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
    return sub_44C5A0(destination, nullptr, truncatedRatio);
}

HOOK(0x0045B300, DispatchFloatFieldToDefaultDestination, "auto");
