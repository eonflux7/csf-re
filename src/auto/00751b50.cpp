#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(double, __fastcall, GetPlayerPositionX, 0x004D1AE0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionY, 0x004D1B50, (void* self, void* edx));
CSF_FUNC(double, __fastcall, GetPlayerPositionZ, 0x004D1BC0, (void* self, void* edx));

CSF_GLOBAL(float, g_DistanceReferenceX, 0x008BC1D0);
CSF_GLOBAL(float, g_DistanceReferenceY, 0x008BC1D4);
CSF_GLOBAL(float, g_DistanceReferenceZ, 0x008BC1D8);

static int __cdecl NetSpawnMessages_ComparePlayerDistanceToReferencePosition(
    int* firstEntry, int* secondEntry)
{
    void* firstPlayer = reinterpret_cast<void*>(*firstEntry);
    void* secondPlayer = reinterpret_cast<void*>(*secondEntry);

    const float firstZ = static_cast<float>(GetPlayerPositionZ(firstPlayer, nullptr));
    const float firstY = static_cast<float>(GetPlayerPositionY(firstPlayer, nullptr));
    const float firstX = static_cast<float>(GetPlayerPositionX(firstPlayer, nullptr));

    const float secondZ = static_cast<float>(GetPlayerPositionZ(secondPlayer, nullptr));
    const float secondY = static_cast<float>(GetPlayerPositionY(secondPlayer, nullptr));
    const double secondX = GetPlayerPositionX(secondPlayer, nullptr);

    const float firstDeltaX = firstX - g_DistanceReferenceX;
    const float firstDeltaY = firstY - g_DistanceReferenceY;
    const float firstDeltaZ = firstZ - g_DistanceReferenceZ;

    const float secondDeltaX = static_cast<float>(secondX - g_DistanceReferenceX);
    const float secondDeltaY = secondY - g_DistanceReferenceY;
    const float secondDeltaZ = secondZ - g_DistanceReferenceZ;

    const float firstDistanceSquared =
        firstDeltaX * firstDeltaX + firstDeltaY * firstDeltaY +
        firstDeltaZ * firstDeltaZ;
    const float secondDistanceSquared =
        secondDeltaZ * secondDeltaZ + secondDeltaY * secondDeltaY +
        secondDeltaX * secondDeltaX;

    if (firstDistanceSquared < secondDistanceSquared)
        return -1;
    return firstDistanceSquared > secondDistanceSquared ? 1 : 0;
}

HOOK(0x00751B50, NetSpawnMessages_ComparePlayerDistanceToReferencePosition, "auto");
