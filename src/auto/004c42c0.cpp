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
    auto* target = reinterpret_cast<uint8_t*>(
        static_cast<std::uintptr_t>(static_cast<uint32_t>(targetAddress)));

    if (CSF_AT(int, target, 0x14) < 0)
        return CSF_AT(int, target, 0x2C) >= 0;

    const int resolved = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<std::uintptr_t>(target + 0x0C)));

    if (!resolved)
        return CSF_AT(int, target, 0x2C) >= 0;

    float referenceValue = g_fZero;
    uint32_t referenceId;
    int referenceIndex;

    alignas(8) uint8_t referenceBytes[0x40];
    uint32_t* output;
    int referenceKind;

    switch (CSF_AT(uint32_t, target, 0x04))
    {
    case 1:
        referenceId = CSF_AT(uint32_t, target, 0x10);
        referenceIndex = CSF_AT(int, target, 0x14);
        break;

    case 3:
    case 7:
    case 0xC:
    case 0xE:
        output = reinterpret_cast<uint32_t*>(referenceBytes);
        referenceKind = 0;
        goto buildReference;

    case 5:
    case 0xB:
        output = reinterpret_cast<uint32_t*>(referenceBytes + 0x10);
        referenceKind = 1;
        goto buildReference;

    case 6:
        output = reinterpret_cast<uint32_t*>(referenceBytes + 0x30);
        referenceKind = 3;
        goto buildReference;

    case 0xA:
        output = reinterpret_cast<uint32_t*>(referenceBytes + 0x20);
        referenceKind = 2;
        goto buildReference;

    default:
        return CSF_AT(int, target, 0x2C) >= 0;

    buildReference:
        {
            uint32_t* reference = AiCore_BuildTargetReference(
                self, nullptr, output, resolved + 0x1C, referenceKind);
            referenceValue = CSF_AT(float, reference, 0x0C);
            referenceId = CSF_AT(uint32_t, reference, 0x04);
            referenceIndex = CSF_AT(int, reference, 0x08);
        }
        break;
    }

    if (referenceIndex < 0)
        return CSF_AT(int, target, 0x2C) >= 0;

    CSF_AT(float, target, 0x30) = referenceValue;
    CSF_AT(uint32_t, target, 0x28) = referenceId;
    CSF_AT(int, target, 0x2C) = referenceIndex;
    return referenceIndex >= 0;
}

HOOK(0x004C42C0, S_004bd310_UpdateCoverTargetReference, "auto");
