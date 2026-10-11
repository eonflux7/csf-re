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

CSF_FUNC(bool, __fastcall, sub_44E1C0, 0x0044E1C0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_4E52A0, 0x004E52A0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4E58E0, 0x004E58E0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, PairLink_TestEndpoints, 0x004E5B60, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083eb70_TestPairLink(C_0083eb70* self, void* /*edx*/, int a2)
{
    alignas(8) uint8_t endpointsBytes[8];
    alignas(8) uint8_t keyBytes[12];
    uint8_t* arg = reinterpret_cast<uint8_t*>(a2);
    char result;

    self->vSlotA4();
    ++self->field_001c;

    uint32_t arg8 = CSF_AT(uint32_t, arg, 8);
    CSF_AT(uint32_t, keyBytes, 4) = CSF_AT(uint32_t, arg, 4);
    CSF_AT(int, endpointsBytes, 0) = -1;
    CSF_AT(int, endpointsBytes, 4) = -1;
    CSF_AT(uint32_t, keyBytes, 0) = (uint32_t)(uintptr_t)CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, keyBytes, 8) = arg8;

    if (sub_4E58E0(self, nullptr, (int)(uintptr_t)keyBytes) == -1)
    {
        result = 0;
        if (sub_4E52A0(self, nullptr, a2))
        {
            C_0083e9d0* owner = reinterpret_cast<C_0083e9d0*>(self->field_0008);
            bool ok = false;
            if (CSF_AT(uint32_t, arg, 4) == owner->field_0020)
            {
                ok = true;
            }
            else
            {
                int obj = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
                // the original tests only AL of the result
                if (obj != 0 && !(uint8_t)SceneObj_IsInactive((void*)(uintptr_t)obj, nullptr))
                {
                    void* mgr = CSF_AT(void*, CSF_AT(void*, g_pCurrentScene, 0xB0), 0x114);
                    C_0083e9d0* owner2 = reinterpret_cast<C_0083e9d0*>(self->field_0008);
                    int addr = (int)(uintptr_t)((uint8_t*)owner2 + 0x1C);
                    if (sub_44E1C0(mgr, nullptr, addr, a2, -1))
                        ok = true;
                }
            }
            if (ok)
                result = 1;
        }
    }
    else
    {
        result = PairLink_TestEndpoints(self, nullptr, (int)(uintptr_t)endpointsBytes);
    }

    uint8_t counter = self->field_001c;
    if (counter)
        self->field_001c = counter - 1;
    return result;
}

HOOK(0x004E60B0, C_0083eb70_TestPairLink, "auto");
