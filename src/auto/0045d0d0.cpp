#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, IsObjectKindFlagSet, 0x00755880, (int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the handle stored in a3's chain, and if the object is of kind 4..6 or 19
// and passes IsObjectKindFlagSet, stores the byte result of a child's vtable slot 208
// into the byte at a1->field_4 + 1.
static int __stdcall UpdateControlFlagFromHandle(int a1, int a2, int a3)
{
    // Entry address derived from a3: *(*(int*)a3 + 4) + 4
    int result = *reinterpret_cast<int*>(*reinterpret_cast<int*>(a3) + 4) + 4;

    uint8_t* ctx = reinterpret_cast<uint8_t*>(a1);
    CSF_AT(uint8_t*, ctx, 4)[1] = 0;

    if (*reinterpret_cast<int*>(result + 8) >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        int handleObject = result;
        if (handleObject != 0)
        {
            result = IsObjectKindFlagSet(*reinterpret_cast<int*>(handleObject + 80));
            if (static_cast<uint8_t>(result) != 0)
            {
                result = *reinterpret_cast<int*>(handleObject + 80);
                if (result >= 4 && (result <= 6 || result == 19))
                {
                    int childObject = *reinterpret_cast<int*>(handleObject + 52);
                    uint8_t* owner = CSF_AT(uint8_t*, ctx, 4);
                    void* childPtr = reinterpret_cast<void*>(childObject);
                    result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, childPtr, 0)[208 / 4])(childPtr, nullptr);
                    owner[1] = static_cast<uint8_t>(result);
                }
            }
        }
    }
    return result;
}

HOOK(0x0045D0D0, UpdateControlFlagFromHandle, "auto");
