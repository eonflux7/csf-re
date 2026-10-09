#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, ForwardToSubObjectAt30, 0x0049EBF0, (void* self, void* , int a1));
CSF_FUNC(int, __cdecl, IsControlTypeForwardable, 0x00755880, (int a1));

static int __stdcall ApplyValueToHandleControl(int /*a1*/, int /*a2*/, int* a3)
{
    // v3 = *(*a3 + 4); v4 = *(v3 + 12); result = v3 + 4
    uint32_t v3 = CSF_AT(uint32_t, reinterpret_cast<void*>(*a3), 4);
    int32_t v4 = CSF_AT(int32_t, reinterpret_cast<void*>(v3), 12);
    int result = static_cast<int>(v3 + 4);

    if (v4 >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        void* control = reinterpret_cast<void*>(static_cast<uint32_t>(result));
        if (result)
        {
            result = IsControlTypeForwardable(CSF_AT(int, control, 0x50));
            if (static_cast<uint8_t>(result) != 0)
            {
                // v7 = *(float*)(*(a3[1] + 4) + 4), clamped to control->limit (+0x184)
                uint32_t sourceRecord = static_cast<uint32_t>(a3[1]);
                uint32_t fieldRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceRecord), 4);
                float value = CSF_AT(float, reinterpret_cast<void*>(fieldRecord), 4);
                float limit = CSF_AT(float, control, 0x184);
                if (value > limit)
                    value = limit;

                uint32_t valueBits;
                std::memcpy(&valueBits, &value, sizeof(valueBits));
                return ForwardToSubObjectAt30(control, nullptr, static_cast<int>(valueBits));
            }
            else if (CSF_AT(int, control, 0x50) == 12)
            {
                // *(control->sub34 + 0x44) = *(*(a3[1] + 4) + 4)
                uint32_t r = CSF_AT(uint32_t, reinterpret_cast<void*>(a3[1]), 4);
                uint32_t targetObj = CSF_AT(uint32_t, control, 0x34);
                CSF_AT(uint32_t, reinterpret_cast<void*>(targetObj), 0x44) =
                    CSF_AT(uint32_t, reinterpret_cast<void*>(r), 4);
                result = static_cast<int>(r);
            }
        }
    }
    return result;
}

HOOK(0x0045FB50, ApplyValueToHandleControl, "auto");
