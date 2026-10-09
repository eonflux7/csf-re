#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, sub_491FD0, 0x00491FD0, (void* self, void* edx, int a2));

// Stdcall, 3 args (a1, a2 unused, a3) -> ret 12.
static int __stdcall UpdateTargetFlagFromHandle(int a1, int a2, uint32_t* a3)
{
    (void)a2;

    // Clear the flag byte at [[a1+4]+1] first.
    uintptr_t flagBase = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1) + 4);
    *reinterpret_cast<uint8_t*>(flagBase + 1) = 0;

    // result = *(*a3 + 4) + 4  (handle key slot address)
    uintptr_t recordAddr = static_cast<uintptr_t>(*a3);
    int result = static_cast<int>(*reinterpret_cast<uint32_t*>(recordAddr + 4) + 4);

    // Signed check on the handle key.
    if (*reinterpret_cast<int32_t*>(static_cast<uintptr_t>(result) + 8) >= 0)
    {
        int handleObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        result = handleObject;
        if (handleObject != 0)
        {
            void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(handleObject));
            result = CSF_AT(int32_t, object, 0x34);
            if (result != 0)
            {
                if (CSF_AT(int32_t, object, 0x50) == 28)
                {
                    // a3[1] -> *(a3[1] + 4) + 4
                    uintptr_t secondRecordAddr = static_cast<uintptr_t>(a3[1]);
                    uint32_t secondKeyAddr = *reinterpret_cast<uint32_t*>(secondRecordAddr + 4) + 4;
                    int secondHandleObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(secondKeyAddr));

                    // Re-read the flag base after the call, as the original does.
                    uintptr_t flagBaseAfter = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a1) + 4);
                    char flagValue = sub_491FD0(object, nullptr, secondHandleObject);
                    *reinterpret_cast<uint8_t*>(flagBaseAfter + 1) = static_cast<uint8_t>(flagValue);
                    result = static_cast<int>(flagValue);
                }
            }
        }
    }
    return result;
}

HOOK(0x45FFC0, UpdateTargetFlagFromHandle, "auto");
