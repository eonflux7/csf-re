#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void* SceneHandle_AddressAsPointer(uint32_t address)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

static int __stdcall SceneHandle_ResolveAndDispatchCode21(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    // v3 = *(*a3 + 4)
    uint32_t handleOwner = CSF_AT(uint32_t, SceneHandle_AddressAsPointer(a3[0]), 4);
    // v4 = *(v3 + 12)
    int32_t handleFlag = static_cast<int32_t>(CSF_AT(uint32_t, SceneHandle_AddressAsPointer(handleOwner), 12));
    // result = v3 + 4
    int result = static_cast<int>(handleOwner + 4);

    if (handleFlag >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        int resolved = result;
        if (resolved != 0)
        {
            uint32_t controlObj = CSF_AT(uint32_t, SceneHandle_AddressAsPointer(static_cast<uint32_t>(resolved)), 52);
            if (controlObj != 0)
            {
                // vtbl slot 0xB0 / 4 = 44
                void* controlPtr = SceneHandle_AddressAsPointer(controlObj);
                int busyFlag = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, controlPtr, 0)[44])(controlPtr, nullptr);
                result = busyFlag;

                if (static_cast<uint8_t>(busyFlag) == 0)
                {
                    uint32_t requestSource = a3[1];
                    uint32_t requestInner = CSF_AT(uint32_t, SceneHandle_AddressAsPointer(requestSource), 4);
                    uint32_t requestValue = CSF_AT(uint32_t, SceneHandle_AddressAsPointer(requestInner), 4);

                    int request[2];
                    request[0] = 21;
                    uint32_t controlObjAgain = CSF_AT(uint32_t, SceneHandle_AddressAsPointer(static_cast<uint32_t>(resolved)), 52);
                    request[1] = static_cast<int>(requestValue);

                    // vtbl slot 0xA8 / 4 = 42
                    void* dispatchPtr = SceneHandle_AddressAsPointer(controlObjAgain);
                    return reinterpret_cast<int (__fastcall*)(void*, void*, int*)>(
                        CSF_AT(void**, dispatchPtr, 0)[42])(dispatchPtr, nullptr, request);
                }
            }
        }
    }
    return result;
}

HOOK(0x0045F2B0, SceneHandle_ResolveAndDispatchCode21, "auto");
