#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, GetEntryPositionVector, 0x00461B60, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, FindActiveEntryIndexByKey, 0x004DC5B0, (void* self, void* , void* keyObject));
CSF_FUNC(int, __fastcall, GetActiveEntryRecord, 0x004DC6E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, FindHitIndexAtPosition, 0x0075CE00, (void* self, void* edx, float* a2, uint32_t* a3));
CSF_FUNC(int, __fastcall, GetHitRecordByIndex, 0x0075CF00, (void* self, void* edx, int a2));

static int __stdcall ResolveEntryHitFromRequest(int a1, int /*unusedArg*/, int a3)
{
    uint8_t* outRecord = reinterpret_cast<uint8_t*>(a1);
    uint8_t* requestCell = reinterpret_cast<uint8_t*>(a3);

    // Zero the output position. [a1+4] is loaded once; the three vector stores share that base.
    uint8_t* outHitInfo = CSF_AT(uint8_t*, outRecord, 4);
    CSF_AT(float, outHitInfo, 4) = g_ZeroVectorX;
    CSF_AT(float, outHitInfo, 8) = g_ZeroVectorY;
    CSF_AT(float, outHitInfo, 12) = g_ZeroVectorZ;

    // The original reloads [a1+4] before each id store.
    CSF_AT(int, CSF_AT(uint8_t*, outRecord, 4), 16) = -1;
    CSF_AT(int, CSF_AT(uint8_t*, outRecord, 4), 20) = -1;

    // requestEntry = *(*a3 + 4); slot = requestEntry->[12]; returnValue = requestEntry + 4
    uint8_t* requestEntry = CSF_AT(uint8_t*, CSF_AT(uint8_t*, requestCell, 0), 4);
    int requestEntrySlot = CSF_AT(int, requestEntry, 12);
    int returnValue = static_cast<int>(reinterpret_cast<uintptr_t>(requestEntry) + 4);

    if (requestEntrySlot >= 0)
    {
        returnValue = CHandleTable_Resolve(g_pCurrentScene, nullptr, returnValue);
        if (returnValue != 0)
        {
            uint8_t* sceneRoot = reinterpret_cast<uint8_t*>(GetValue8B0554());
            C_0083e0d8* hitTable = CSF_AT(C_0083e0d8*, sceneRoot, 0x1FD8);
            uint8_t* entryTable = reinterpret_cast<uint8_t*>(hitTable) + 0x34;

            // The key is re-read from *a3 here, as the original does after the Resolve call.
            returnValue = FindActiveEntryIndexByKey(
                entryTable, nullptr, CSF_AT(uint8_t*, CSF_AT(uint8_t*, requestCell, 0), 4) + 4);
            if (returnValue != -1)
            {
                int activeEntryRecord = GetActiveEntryRecord(entryTable, nullptr, returnValue);

                // callScratch[0..3] is the scratch object passed as `this`;
                // callScratch[4..6] is the position vector read back afterwards.
                uint32_t callScratch[7];
                float* positionVec = reinterpret_cast<float*>(&callScratch[4]);
                GetEntryPositionVector(callScratch, nullptr, reinterpret_cast<uint32_t*>(activeEntryRecord));

                // Copy the position into the output; [a1+4] is reloaded here as in the original.
                uint8_t* outHitInfoAfterCall = CSF_AT(uint8_t*, outRecord, 4);
                CSF_AT(float, outHitInfoAfterCall, 4) = positionVec[0];
                CSF_AT(float, outHitInfoAfterCall, 8) = positionVec[1];
                CSF_AT(float, outHitInfoAfterCall, 12) = positionVec[2];

                // Two contiguous DWORDs, both -1: hitOut[0] = hit index, hitOut[1] = sub-index.
                uint32_t hitOut[2] = { 0xFFFFFFFFu, 0xFFFFFFFFu };
                char hitFound = FindHitIndexAtPosition(hitTable, nullptr, positionVec, hitOut);

                if (hitFound == 0)
                {
                    // The original returns the callee's eax here. The bool is in AL, so only the low byte is defined.
                    return static_cast<int>(static_cast<uint8_t>(hitFound));
                }

                uint8_t* hitRecord = reinterpret_cast<uint8_t*>(
                    GetHitRecordByIndex(hitTable, nullptr, static_cast<int>(hitOut[0])));

                int hitRecordKey = CSF_AT(int, hitRecord, 0);
                CSF_AT(int, CSF_AT(uint8_t*, outRecord, 4), 16) = hitRecordKey;

                // The sub-index is read after the GetHitRecordByIndex call, as in the original.
                uint8_t* hitEntryArray = CSF_AT(uint8_t*, hitRecord, 0x38);
                uint32_t entryByteOffset = hitOut[1] * 0x2Cu;
                int entryValue = *reinterpret_cast<int*>(
                    reinterpret_cast<uintptr_t>(hitEntryArray) + entryByteOffset);
                CSF_AT(int, CSF_AT(uint8_t*, outRecord, 4), 20) = entryValue;

                return entryValue;
            }
        }
    }

    return returnValue;
}

HOOK(0x00463060, ResolveEntryHitFromRequest, "auto");
