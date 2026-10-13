#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, AiCore_BuildTargetReference, 0x004C40F0, (void* self, void* edx, uint32_t* arg0, int a3, int a4));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_004bd310_UpdateCoverTargetReference(
    S_004bd310* self, void* /*edx*/, int targetAddress)
{
    auto* target = reinterpret_cast<uint8_t*>(targetAddress);

    if (CSF_AT(int, target, 0x14) < 0)
        return CSF_AT(int, target, 0x2C) >= 0;

    const int resolvedAddress = CHandleTable_Resolve(
        g_pCurrentScene, nullptr, reinterpret_cast<int>(target + 0x0C));

    if (!resolvedAddress)
        return CSF_AT(int, target, 0x2C) >= 0;

    const int targetType = CSF_AT(int, target, 0x08);
    float referenceValue = g_fZero;
    uint32_t referenceHandle;
    int referenceIndex;

    if (targetType == 1)
    {
        referenceHandle = CSF_AT(uint32_t, target, 0x10);
        referenceIndex = CSF_AT(int, target, 0x14);
    }
    else
    {
        alignas(8) uint8_t alternateReferenceBytes[0x10];
        alignas(8) uint8_t specialReferenceBytes[0x10];
        alignas(8) uint8_t standardReferenceBytes[0x10];

        uint32_t* reference;
        const int sourceAddress = reinterpret_cast<int>(
            reinterpret_cast<uint8_t*>(resolvedAddress) + 0x1C);

        switch (targetType)
        {
        case 2:
        case 3:
        case 5:
        case 6:
        case 8:
        case 9:
            reference = AiCore_BuildTargetReference(
                self, nullptr,
                reinterpret_cast<uint32_t*>(standardReferenceBytes),
                sourceAddress, 1);
            break;

        case 0xA:
            reference = AiCore_BuildTargetReference(
                self, nullptr,
                reinterpret_cast<uint32_t*>(alternateReferenceBytes),
                sourceAddress, 2);
            break;

        case 0xB:
            reference = AiCore_BuildTargetReference(
                self, nullptr,
                reinterpret_cast<uint32_t*>(specialReferenceBytes),
                sourceAddress, 1);
            break;

        default:
            return CSF_AT(int, target, 0x2C) >= 0;
        }

        referenceValue = CSF_AT(float, reference, 0x0C);
        referenceHandle = CSF_AT(uint32_t, reference, 0x04);
        referenceIndex = CSF_AT(int, reference, 0x08);
    }

    if (referenceIndex < 0)
        return CSF_AT(int, target, 0x2C) >= 0;

    CSF_AT(float, target, 0x30) = referenceValue;
    CSF_AT(uint32_t, target, 0x28) = referenceHandle;
    CSF_AT(int, target, 0x2C) = referenceIndex;
    return referenceIndex >= 0;
}

HOOK(0x004C43B0, S_004bd310_UpdateCoverTargetReference, "auto");
