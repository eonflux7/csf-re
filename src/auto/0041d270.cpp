#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_0082daf4_FindOrCreateByKey, 0x004087D0, (void* self, void* edx, int a3));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(float*, __fastcall, C_0082daf4_ApplyVec4, 0x0040B550, (void* self, void* edx, int a2, float* a3));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(double, __fastcall, S_00526d40_NormalizeCopyAndReportY, 0x006AB9E0, (S_00526d40* self, void*));

static float* __fastcall C_0082e428_UpdateWheelAxisVector(C_0082e428* self, void* /*edx*/)
{
    using EntryGetterFn = int(__fastcall*)(void* self, void* edx, void* outPair);

    // Target container for the axis vector.
    void* vectorTarget = C_0082daf4_FindOrCreateByKey(
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0030)), nullptr, 26);

    // Table entry object for index 26.
    void* tableEntry = reinterpret_cast<void*>(static_cast<std::uintptr_t>(
        GetTableEntryDwordByIndex(reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0030)), nullptr, 26)));

    // Out pair: matrix pointer (dword) followed by a flag byte.
    alignas(8) unsigned char matrixBuf[8] = {};

    // Virtual slot 0x40 / 4 = 16, thiscall with one stack argument.
    int holderValue = reinterpret_cast<EntryGetterFn>(CSF_AT(void**, tableEntry, 0)[16])(
        tableEntry, nullptr, matrixBuf);

    auto* holderPtr = reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(holderValue));
    std::uint32_t holderFirstDword = holderPtr[0];
    auto* axisFields = reinterpret_cast<std::uint32_t*>(
        static_cast<std::uintptr_t>(holderFirstDword + 0x20));

    // sized by the original's frame (0x24 bytes): struct sizes in auto_classes.h are lower bounds
    alignas(8) uint8_t normalizeInputBytes[0x24];
    S_00526d40& normalizeInput = *reinterpret_cast<S_00526d40*>(normalizeInputBytes);
    normalizeInput.field_0000 = axisFields[0];
    CSF_AT(std::uint32_t, &normalizeInput, 0x4) = axisFields[1];
    CSF_AT(std::uint32_t, &normalizeInput, 0x8) = axisFields[2];

    if (CSF_AT(unsigned char, matrixBuf, 4) != 0)
    {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(
            static_cast<std::uintptr_t>(CSF_AT(int, matrixBuf, 0))));
    }

    float normalizedLength = static_cast<float>(
        S_00526d40_NormalizeCopyAndReportY(&normalizeInput, nullptr));

    float axisVector[4];
    axisVector[0] = normalizedLength;
    axisVector[1] = 0.2f;
    axisVector[2] = 0.0f;
    axisVector[3] = -1.0f;

    float* result = C_0082daf4_ApplyVec4(vectorTarget, nullptr, 0, axisVector);
    self->field_008c = 5;
    return result;
}

HOOK(0x0041D270, C_0082e428_UpdateWheelAxisVector, "auto");
