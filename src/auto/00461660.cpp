#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_485A20, 0x00485A20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_48FE90, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_49BF00, 0x0049BF00, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, sub_755810, 0x00755810, (unsigned int a1));

static int __stdcall ProcessHandleActionMessage(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    uint32_t firstLevel = *CSF_AT(uint32_t*, a3, 0);
    uint32_t secondLevel = *CSF_AT(uint32_t*, firstLevel, 4);
    int32_t slotIndex = *CSF_AT(int32_t*, secondLevel, 12);
    int32_t handleValue = static_cast<int32_t>(secondLevel + 4);

    if (slotIndex >= 0)
    {
        int handleObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);
        uint8_t* obj = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(handleObject)));

        if (obj == nullptr
            || (static_cast<uint8_t>(sub_755810(*CSF_AT(uint32_t*, obj, 0x50))) == 0)
            || *CSF_AT(uint32_t*, obj, 0x50) != 0
            || sub_485A20(reinterpret_cast<void*>(static_cast<uintptr_t>(*CSF_AT(uint32_t*, obj, 0x44))),
                          nullptr, 28) == -1)
        {
            return sub_48FE90(obj, nullptr);
        }

        uint32_t ecxVal = *CSF_AT(uint32_t*, a3, 4);
        uint32_t edxVal = *CSF_AT(uint32_t*, ecxVal, 4);
        uint32_t eaxVal = *CSF_AT(uint32_t*, edxVal, 4);
        return sub_49BF00(obj, nullptr, static_cast<int>(eaxVal));
    }

    return handleValue;
}

HOOK(0x00461660, ProcessHandleActionMessage, "auto");
