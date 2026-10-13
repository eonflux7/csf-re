#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: CHandleTable::Resolve(scene, handle) -> object pointer or 0.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
// Bound by address: forwarder returning the object's position vector (float[3]).
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Picks the handle block the original selects: the +0x34 block when the
// signed value at +0x54 is negative, otherwise the +0x4C block.
static uint8_t* SelectHandleBlock(S_004ad990* self, int32_t selector)
{
    if (selector < 0)
        return reinterpret_cast<uint8_t*>(self) + 0x34;
    return reinterpret_cast<uint8_t*>(self) + 0x4c;
}

static void __fastcall S_004ad990_UpdateActiveCover(S_004ad990* self, void* /*edx*/, int /*unusedArg*/)
{
    if (self->field_002c == static_cast<uint32_t>(-1))
        return;

    const int32_t selector = static_cast<int32_t>(self->field_0054);

    uint8_t* firstBlock = SelectHandleBlock(self, selector);
    if (CSF_AT(int, firstBlock, 8) < 0)
        return;

    uint8_t* handleBlock = SelectHandleBlock(self, selector);
    int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(handleBlock)));

    C_008342e8* resolvedObject = static_cast<C_008342e8*>(
        reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(resolvedHandle))));
    if (!resolvedObject)
        return;

    float* positionVector = GetObjectPositionVector(resolvedObject, nullptr);
    CSF_AT(float, self, 0x40) = positionVector[0];
    CSF_AT(float, self, 0x44) = positionVector[1];
    CSF_AT(float, self, 0x48) = positionVector[2];
}

HOOK(0x004ADB70, S_004ad990_UpdateActiveCover, "auto");
