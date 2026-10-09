#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, CheckGlobalObjectFlag, 0x00521C00, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, ApplyFloatToGlobalObject, 0x005221E0, (void* self, void* edx, float a2));

// Reads a float from the chain *(*a3)->[+4]->[+4], then, if CheckGlobalObjectFlag on the
// global object returns nonzero, applies that float to a second fetch of the global object.
static char __stdcall ApplyEntryFloatIfEnabled(int a1, int a2, uint32_t* a3)
{
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*a3));
    uint8_t* inner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(*reinterpret_cast<uint32_t*>(entry + 4)));
    float value = *reinterpret_cast<float*>(inner + 4);

    void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
    char result = CheckGlobalObjectFlag(globalObject, nullptr);
    if (result)
    {
        void* globalObjectForApply = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_521BB0()));
        return static_cast<char>(ApplyFloatToGlobalObject(globalObjectForApply, nullptr, value));
    }
    return result;
}

HOOK(0x0045B1D0, ApplyEntryFloatIfEnabled, "auto");
