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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, FallbackForNegativeFieldObject, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __cdecl, IsCategoryFlagSet, 0x00755810, (unsigned int a1));

// Integer address -> pointer (32-bit game addresses).
static void* IntToPtr(int address)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(address)));
}

// __thiscall(this, a2, a3, ownerRef) -> ret 12.
// ownerRef points to a slot holding a record pointer; record+4 holds a payload block
// whose +12 is a signed status and whose +4 (handle address) is used as a handle.
static int __fastcall UpdateTargetPointFromHandle(void* self, void* /*edx*/, int a2, int a3, int ownerRef)
{
    (void)a2;
    (void)a3;

    int ownerPtr = *static_cast<int*>(IntToPtr(ownerRef));
    int payloadAddr = *static_cast<int*>(IntToPtr(ownerPtr + 4));
    int result = CSF_AT(int, IntToPtr(payloadAddr), 12);

    if (result >= 0)
    {
        int handleValue = payloadAddr + 4;
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);

        C_00833908* target = static_cast<C_00833908*>(IntToPtr(result));
        if (target)
        {
            result = static_cast<int>(reinterpret_cast<uintptr_t>(target->field_0034));
            if (result)
            {
                result = IsCategoryFlagSet((unsigned int)target->field_0050);
                if (static_cast<uint8_t>(result))
                {
                    if (static_cast<uint8_t>(IsField1BCNonNegative(target, nullptr)))
                    {
                        C_008672e4* renderer = reinterpret_cast<C_008672e4*>(target->field_0034);
                        ownerRef = 28;
                        // vtable slot at byte offset 0xA8 -> index 42
                        result = ((int(__fastcall*)(void*, void*, int*))(CSF_AT(void**, renderer, 0)[42]))(
                            renderer, nullptr, &ownerRef);

                        CSF_AT(int, self, 0x60) = CSF_AT(int, IntToPtr(handleValue), 4);
                        CSF_AT(int, self, 0x64) = CSF_AT(int, IntToPtr(handleValue), 8);
                        CSF_AT(uint8_t, self, 0x82) = 0;
                    }
                    else
                    {
                        return FallbackForNegativeFieldObject(target, nullptr);
                    }
                }
            }
        }
    }

    return result;
}

HOOK(0x0045FC50, UpdateTargetPointFromHandle, "auto");
