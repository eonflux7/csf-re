#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, IsField1BCNonNegative, 0x00492B10, (void* self, void*));
CSF_FUNC(int, __fastcall, C_Unknown_GetField_01c0, 0x00492B30, (void* self, void*));
CSF_FUNC(int, __cdecl, TestTypeMaskBit, 0x00755810, (unsigned int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

// Original: unsigned int __stdcall sub_45C260(int a1, int a2, int a3); ret 12
static unsigned int __stdcall RefreshHandleFieldFloat(void* a1, void* /*a2*/, void* a3)
{
    // *(*(a1 + 4) + 4) = -1.0f
    void* stateBlock = CSF_AT(void*, a1, 4);
    CSF_AT(float, stateBlock, 4) = -1.0f;

    // v3 = *(*(a3) + 4); v4 = *(v3 + 12)
    uint32_t recordAddr = CSF_AT(uint32_t, CSF_AT(void*, a3, 0), 4);
    void* record = reinterpret_cast<void*>(static_cast<uintptr_t>(recordAddr));
    int32_t slotIndex = CSF_AT(int32_t, record, 12);

    unsigned int result = recordAddr + 4;

    if (slotIndex >= 0)
    {
        unsigned int resolved = static_cast<unsigned int>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(result)));
        result = resolved;

        if (resolved != 0)
        {
            void* entity = reinterpret_cast<void*>(static_cast<uintptr_t>(resolved));

            result = static_cast<unsigned int>(TestTypeMaskBit(CSF_AT(uint32_t, entity, 0x50)));
            if (static_cast<uint8_t>(result) != 0)
            {
                result = static_cast<unsigned int>(IsField1BCNonNegative(entity, nullptr));
                if (static_cast<uint8_t>(result) != 0)
                {
                    // Target object is re-read from a1 + 4 before the final getter call.
                    void* targetObject = CSF_AT(void*, a1, 4);

                    result = static_cast<unsigned int>(C_Unknown_GetField_01c0(entity, nullptr));

                    // Signed value; negative values are treated as unsigned (add 2^32 bias).
                    int32_t fieldValue = static_cast<int32_t>(result);
                    float fieldAsFloat = static_cast<float>(fieldValue);
                    if (fieldValue < 0)
                    {
                        fieldAsFloat += g_UnsignedIntegerBias;
                    }
                    CSF_AT(float, targetObject, 4) = fieldAsFloat;
                }
            }
        }
    }

    return result;
}

HOOK(0x45C260, RefreshHandleFieldFloat, "auto");
