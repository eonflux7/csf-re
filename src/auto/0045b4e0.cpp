#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

// Callee bindings (original addresses).
CSF_FUNC(uint32_t*, __fastcall, InitTimedRequest, 0x004458A0,
         (void* self, void* edx, int a2, uint32_t* a3, int a4, int a5, int a6));
CSF_FUNC(uint32_t*, __fastcall, ResolveSceneRequest, 0x00447130,
         (void* self, void* edx, float* a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads the dword stored at (address + offset).
static uint32_t ReadDwordAt(uint32_t address, uint32_t offset) {
    return *CSF_AT(uint32_t*, reinterpret_cast<void*>(address), offset);
}

// Reads the float stored at (address + offset).
static float ReadFloatAt(uint32_t address, uint32_t offset) {
    return *CSF_AT(float*, reinterpret_cast<void*>(address), offset);
}

// Follows the pointer stored at +4 of the object, then reads the dword at +4 of that.
static uint32_t ReadChainedDword(uint32_t objectPointer) {
    return ReadDwordAt(ReadDwordAt(objectPointer, 4), 4);
}

static int __stdcall SubmitTimedRequestAndResolve(int a1, int a2, uint32_t* a3) {
    // Header block: three dwords copied from (*(a3[0] + 4) + 4).
    uint32_t headerBase = ReadDwordAt(a3[0], 4) + 4;
    uint32_t requestHeader[3];
    requestHeader[0] = ReadDwordAt(headerBase, 0);
    requestHeader[1] = ReadDwordAt(headerBase, 4);
    uint32_t secondArg = a3[1];
    requestHeader[2] = ReadDwordAt(headerBase, 8);
    uint32_t thirdArg = a3[2];
    uint32_t secondValue = ReadChainedDword(secondArg);
    uint32_t thirdValue = ReadChainedDword(thirdArg);

    // Float from a3[3]: *(float*)(*(a3[3] + 4) + 4); non-positive values become FLT_MAX.
    float requestLimit = ReadFloatAt(ReadDwordAt(a3[3], 4), 4);
    if (requestLimit <= 0.0f) {
        requestLimit = 3.4028235e38f;
    }

    uint32_t limitBits;
    std::memcpy(&limitBits, &requestLimit, sizeof(limitBits));

    uint8_t requestBuffer[32];
    InitTimedRequest(requestBuffer, nullptr, 1, requestHeader,
                     static_cast<int>(secondValue),
                     static_cast<int>(limitBits),
                     static_cast<int>(thirdValue));

    void* scene = g_pCurrentScene;
    void* sceneChild = *CSF_AT(void**, scene, 0xB0);
    uint32_t* resolved = ResolveSceneRequest(sceneChild, nullptr,
                                             reinterpret_cast<float*>(requestBuffer));
    return static_cast<int>(reinterpret_cast<uintptr_t>(resolved));
}

HOOK(0x0045B4E0, SubmitTimedRequestAndResolve, "auto");
