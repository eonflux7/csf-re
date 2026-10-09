#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_49EC00, 0x0049EC00, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, sub_755880, 0x00755880, (int a1));

static uint8_t* PtrFromWord(uint32_t word)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(word));
}

static int __stdcall ClampAndApplyValueFromArgs(int /*unusedA1*/, int /*unusedA2*/, uint32_t* a3)
{
    // v3 = *(*a3 + 4)
    uint32_t blockWord = *reinterpret_cast<uint32_t*>(PtrFromWord(a3[0]) + 4);
    // v4 = *(v3 + 12), signed test
    int32_t blockFlag = CSF_AT(int32_t, PtrFromWord(blockWord), 0x0C);
    // result = v3 + 4
    int result = static_cast<int>(blockWord + 4);

    if (blockFlag >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        int resolvedHandle = result;
        if (resolvedHandle != 0)
        {
            uint8_t* control = PtrFromWord(static_cast<uint32_t>(resolvedHandle));
            result = sub_755880(static_cast<int>(CSF_AT(uint32_t, control, 0x50)));
            if (static_cast<uint8_t>(result) != 0)
            {
                // v7 = *(float*)(*(a3[1] + 4) + 4)
                uint32_t innerWord = *reinterpret_cast<uint32_t*>(PtrFromWord(a3[1]) + 4);
                float requested = *reinterpret_cast<float*>(PtrFromWord(innerWord) + 4);

                // if (v7 > *(float*)(control + 0x18C)) v7 = *(float*)(control + 0x18C)
                if (requested > CSF_AT(float, control, 0x18C))
                {
                    requested = CSF_AT(float, control, 0x18C);
                }

                uint32_t requestedBits = 0;
                std::memcpy(&requestedBits, &requested, sizeof(requestedBits));
                return sub_49EC00(control, nullptr, static_cast<int>(requestedBits));
            }
        }
    }
    return result;
}

HOOK(0x0045FBE0, ClampAndApplyValueFromArgs, "auto");
