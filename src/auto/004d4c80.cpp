#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Bound by address: scene handle table lookup (__fastcall, one stack arg).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Bound by address: forwarder that returns the object's position vector pointer (no stack args).
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_Const200_0f, 0x0082DDEC);

// Returns true when the object referenced by the owner's handle at +0x3BC
// is within 200 units of the owner's linked object (+0x14).
static bool __fastcall AiGiveObject_IsTargetWithinRange(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x08);

    if (CSF_AT(int, owner, 0x3C4) < 0)
        return false;

    // Handle is the address of the owner's embedded handle field (owner + 0x3BC).
    int handle = (int)((uint8_t*)owner + 0x3BC);
    void* targetObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!targetObject)
        return false;

    // Owner pointer is re-read after the call, as the original does.
    void* ownerAgain = CSF_AT(void*, self, 0x08);
    void* ownerObject = CSF_AT(void*, ownerAgain, 0x14);

    float* ownerPos = GetObjectPositionVector(ownerObject, nullptr);
    float* targetPos = GetObjectPositionVector(targetObject, nullptr);

    float dx = targetPos[0] - ownerPos[0];
    float dy = targetPos[1] - ownerPos[1];
    float dz = targetPos[2] - ownerPos[2];

    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    return distance < g_Const200_0f;
}

HOOK(0x4D4C80, AiGiveObject_IsTargetWithinRange, "auto");
