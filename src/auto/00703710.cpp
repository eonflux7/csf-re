#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RpAtomicDestroy, 0x0058F060, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RpAtomicSetFrame, 0x0058F300, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwFrameDestroy, 0x00623B10, (int frame));
CSF_FUNC(char, __fastcall, AtomicEmitterArray_FreeElementsAndHeader, 0x00703600, (void* self, void* edx));

static int __fastcall S_00703360_DestroyAtomicsAndClearArray(
    S_00703360* self, void* /*edx*/)
{
    uint32_t groupIndex = 0;
    uint32_t groupOffset = 0;

    if (self->field_0008 != 0)
    {
        do
        {
            uint32_t atomicIndex = 0;
            uint32_t atomicOffset = 0;
            auto* groups = reinterpret_cast<uint8_t*>(self->field_0000);

            if (CSF_AT(uint32_t, groups, groupOffset + 8) != 0)
            {
                do
                {
                    auto* entries = reinterpret_cast<uint8_t*>(
                        CSF_AT(uint32_t, groups, groupOffset));
                    auto* entry = entries + atomicOffset;
                    const uint32_t atomicAddress = CSF_AT(uint32_t, entry, 0);
                    const int frame = CSF_AT(
                        int, reinterpret_cast<void*>(atomicAddress), 4);

                    if (frame != 0)
                    {
                        RpAtomicSetFrame(static_cast<int>(atomicAddress), 0);
                        RwFrameDestroy(frame);
                    }

                    RpAtomicDestroy(reinterpret_cast<uint32_t*>(
                        CSF_AT(uint32_t, entry, 0)));
                    CSF_AT(uint32_t, entry, 0) = 0;

                    groups = reinterpret_cast<uint8_t*>(self->field_0000);
                    ++atomicIndex;
                    atomicOffset += 12;
                } while (atomicIndex <
                         CSF_AT(uint32_t, groups, groupOffset + 8));
            }

            groupOffset += 16;
            ++groupIndex;
        } while (groupIndex < self->field_0008);
    }

    // The supplied binding says char, but the verified callee returns full EAX.
    using ClearArrayFunction = int(__fastcall*)(void*, void*);
    return reinterpret_cast<ClearArrayFunction>(
        AtomicEmitterArray_FreeElementsAndHeader)(self, nullptr);
}

HOOK(0x00703710, S_00703360_DestroyAtomicsAndClearArray, "auto");
