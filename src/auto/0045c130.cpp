#include <cstdint>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_RadToDegFactor, 0x008313E4);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(float*, __fastcall, BuildFacingVectorBetween, 0x00402030, (void* self, void* edx, float* a2, float* a3));
CSF_FUNC(double, __fastcall, GetVectorAngleRadians, 0x006AB930, (void* self, void* edx));

// ctxAddr->+4 is the output holder; entityArgs[0] / entityArgs[1] are the two
// argument nodes; node->+4 is a record whose +12 is a signed flag and whose
// (record+4) address is the handle passed to CHandleTable::Resolve.
static void __stdcall ScriptCommands_ComputeAngleBetweenEntitiesDegrees(uint32_t ctxAddr, int /*unusedArg*/, uint32_t* entityArgs)
{
    void* ctx = reinterpret_cast<void*>(ctxAddr);

    // Clear the output angle before anything else can bail out.
    void* angleHolder = reinterpret_cast<void*>(CSF_AT(uint32_t, ctx, 4));
    CSF_AT(uint32_t, angleHolder, 4) = 0;

    // First entity.
    void* firstNode = reinterpret_cast<void*>(entityArgs[0]);
    uint32_t firstRecordAddr = CSF_AT(uint32_t, firstNode, 4);
    void* firstRecord = reinterpret_cast<void*>(firstRecordAddr);
    int32_t firstRecordFlag = CSF_AT(int32_t, firstRecord, 12);
    if (firstRecordFlag < 0)
        return;

    int firstEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(firstRecordAddr + 4));
    if (firstEntity == 0)
        return;

    // Second entity.
    void* secondNode = reinterpret_cast<void*>(entityArgs[1]);
    uint32_t secondRecordAddr = CSF_AT(uint32_t, secondNode, 4);
    void* secondRecord = reinterpret_cast<void*>(secondRecordAddr);
    int32_t secondRecordFlag = CSF_AT(int32_t, secondRecord, 12);
    if (secondRecordFlag < 0)
        return;

    int secondEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(secondRecordAddr + 4));
    if (secondEntity == 0)
        return;

    if (firstEntity == secondEntity)
        return;

    // Build the facing vector from the two entities' position references.
    int firstPosition = GetObjectPositionVector(reinterpret_cast<void*>(firstEntity), nullptr);
    float facingVector[3];
    int secondPosition = GetObjectPositionVector(reinterpret_cast<void*>(secondEntity), nullptr);
    BuildFacingVectorBetween(reinterpret_cast<void*>(secondPosition), nullptr, facingVector, reinterpret_cast<float*>(firstPosition));

    // Store the angle in degrees into holder->+4, re-reading the holder pointer.
    double angleRadians = GetVectorAngleRadians(facingVector, nullptr);
    void* holderNow = reinterpret_cast<void*>(CSF_AT(uint32_t, ctx, 4));
    CSF_AT(float, holderNow, 4) = static_cast<float>(angleRadians * g_RadToDegFactor);
}

HOOK(0x0045C130, ScriptCommands_ComputeAngleBetweenEntitiesDegrees, "auto");
