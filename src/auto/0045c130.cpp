#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadToDegFactor, 0x008313E4);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetEntityPositionRef, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(double, __fastcall, GetVectorAngleRadians, 0x006AB930, (void* self, void* edx));

static void __stdcall ComputeAngleBetweenEntitiesDegrees(void* angleHolder, int /*unusedArg*/, uint32_t* entityRefPair)
{
    // *(*(holder + 4) + 4) = 0
    uint32_t* angleRecord = reinterpret_cast<uint32_t*>(*CSF_AT(uint32_t*, angleHolder, 0x4));
    *CSF_AT(uint32_t*, angleRecord, 0x4) = 0;

    // First entity: *(*(*pair) + 4) is the handle table entry
    uint32_t* firstNode = reinterpret_cast<uint32_t*>(*entityRefPair);
    uint32_t* firstEntry = reinterpret_cast<uint32_t*>(*CSF_AT(uint32_t*, firstNode, 0x4));
    if (*CSF_AT(int32_t*, firstEntry, 0xC) >= 0)
    {
        int firstTarget = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                               reinterpret_cast<int>(CSF_AT(uint32_t*, firstEntry, 0x4)));
        if (firstTarget != 0)
        {
            // Second entity: re-read the pair after the first lookup, as the original does
            uint32_t* secondNode = reinterpret_cast<uint32_t*>(entityRefPair[1]);
            uint32_t* secondEntry = reinterpret_cast<uint32_t*>(*CSF_AT(uint32_t*, secondNode, 0x4));
            if (*CSF_AT(int32_t*, secondEntry, 0xC) >= 0)
            {
                int secondTarget = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                        reinterpret_cast<int>(CSF_AT(uint32_t*, secondEntry, 0x4)));
                if (secondTarget != 0 && firstTarget != secondTarget)
                {
                    int firstPositionRef = GetEntityPositionRef(reinterpret_cast<void*>(firstTarget), nullptr);
                    int secondPositionRef = GetEntityPositionRef(reinterpret_cast<void*>(secondTarget), nullptr);

                    // Second result is the 'this' of the vector builder; first result is the trailing argument
                    float directionVec[3];
                    BuildFacingVectorBetween(reinterpret_cast<void*>(secondPositionRef), nullptr,
                                             directionVec, reinterpret_cast<float*>(firstPositionRef));

                    double radians = GetVectorAngleRadians(directionVec, nullptr);
                    float degrees = static_cast<float>(radians * g_RadToDegFactor);

                    // Reload the holder's record pointer after the calls, then store the result
                    uint32_t* angleRecordAfter = reinterpret_cast<uint32_t*>(*CSF_AT(uint32_t*, angleHolder, 0x4));
                    *CSF_AT(float*, angleRecordAfter, 0x4) = degrees;
                }
            }
        }
    }
}

HOOK(0x0045C130, ComputeAngleBetweenEntitiesDegrees, "auto");
