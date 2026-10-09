#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, ComputeHandleObjectValue, 0x004E9890, (uint32_t* a1));

// Reads *(*(wrapper + 4) + 4): the value reached from a wrapper argument through its node pointer.
static uint32_t ReadWrapperNodeValue(uint32_t wrapperAddr)
{
    uint32_t nodeAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(wrapperAddr), 4);
    return CSF_AT(uint32_t, reinterpret_cast<void*>(nodeAddr), 4);
}

// Resolves the scene handle held by the first argument's node. If the resolved object has a
// component at +0x34, builds a 6-int event block and forwards it to the component's vtable slot 42.
static int __stdcall DispatchSceneEventToHandleComponent(int /*unusedArg1*/, int /*unusedArg2*/, uint32_t* argPtrs)
{
    uint32_t nodeAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(argPtrs[0]), 4);
    int32_t nodeFlags = CSF_AT(int32_t, reinterpret_cast<void*>(nodeAddr), 12);
    int result = static_cast<int>(nodeAddr + 4);
    if (nodeFlags < 0)
        return result;

    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(nodeAddr + 4));
    if (result == 0)
        return result;
    void* resolvedObject = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

    result = CSF_AT(int, resolvedObject, 0x34);
    if (result == 0)
        return result;

    int eventBlock[6] = {14, 0, 0, 0, 0, 0};
    eventBlock[2] = static_cast<int>(ReadWrapperNodeValue(argPtrs[1]));
    eventBlock[3] = static_cast<int>(ReadWrapperNodeValue(argPtrs[2]));

    uint32_t floatNodeAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(argPtrs[3]), 4);
    float floatValue = CSF_AT(float, reinterpret_cast<void*>(floatNodeAddr), 4);
    eventBlock[4] = static_cast<int>(static_cast<int64_t>(floatValue));

    GetValue8B0554();
    eventBlock[5] = ComputeHandleObjectValue(reinterpret_cast<uint32_t*>(resolvedObject));

    eventBlock[1] = static_cast<int>(ReadWrapperNodeValue(argPtrs[4]));

    void* componentObject = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, resolvedObject, 0x34)));
    return ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, componentObject, 0)[42])(componentObject, nullptr, eventBlock);
}

HOOK(0x45E9A0, DispatchSceneEventToHandleComponent, "auto");
