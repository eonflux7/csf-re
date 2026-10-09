#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, ApplyStringToResolvedObject, 0x0048E730, (void* self, void* edx, char* a2));

static uint8_t* PtrAt(uint32_t address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

static C_008342e8* __stdcall ResolveHandleAndApplyString(int /*unusedA1*/, int /*unusedA2*/, uint32_t* handleRef)
{
    // innerBlock = *(*handleRef + 4)
    uint8_t* innerBlock = PtrAt(CSF_AT(uint32_t, PtrAt(*handleRef), 0x04));
    int32_t innerKind = CSF_AT(int32_t, innerBlock, 0x0C);

    C_008342e8* result = reinterpret_cast<C_008342e8*>(innerBlock + 4);
    if (innerKind >= 0)
    {
        int resolved = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(innerBlock + 4)));
        result = reinterpret_cast<C_008342e8*>(static_cast<uintptr_t>(resolved));
        if (result)
        {
            // Second argument's block: *(handleRef[1] + 4)
            uint8_t* sourceBlock = PtrAt(CSF_AT(uint32_t, PtrAt(handleRef[1]), 0x04));
            uint32_t sourceLength = CSF_AT(uint32_t, sourceBlock, 0x1C);
            uint8_t* sourceData = sourceBlock + 4;

            if (sourceLength < 0x10)
            {
                // Short string stored inline.
                return reinterpret_cast<C_008342e8*>(static_cast<uintptr_t>(
                    ApplyStringToResolvedObject(result, nullptr, reinterpret_cast<char*>(sourceData + 4))));
            }
            // Long string stored behind a pointer.
            return reinterpret_cast<C_008342e8*>(static_cast<uintptr_t>(
                ApplyStringToResolvedObject(result, nullptr, CSF_AT(char*, sourceData, 4))));
        }
    }
    return result;
}

HOOK(0x00461480, ResolveHandleAndApplyString, "auto");
