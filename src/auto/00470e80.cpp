#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Expand the 12-byte by-value object into its three ABI stack words.
CSF_FUNC(char*, __fastcall, S_0046f5e0_Resize, 0x004704E0,
         (S_0046f5e0* self, void* edx, unsigned int requestedSize,
          void** fillVtable, uint32_t fillData, uint32_t fillTag));
CSF_FUNC(uint8_t*, __fastcall, ReserveScratchEntries, 0x004725C0,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_CopyEntryToValue, 0x00472D50,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_AssignValueReference, 0x00473EB0,
         (void* self, void* edx, int a2));

static char* __fastcall MissionScript_PopulateValues(
    void* self, void* /*edx*/, int destinationAddress)
{
    auto* destination = reinterpret_cast<uint8_t*>(
        static_cast<uint32_t>(destinationAddress));

    const uint32_t begin = CSF_AT(uint32_t, self, 0x8);
    const unsigned int count = begin
        ? static_cast<unsigned int>(
              static_cast<int32_t>(
                  CSF_AT(uint32_t, self, 0xC) - begin) / 68)
        : 0;

    C_008313f8 fillValue;
    fillValue.vtbl = CSF_VTBL(0x0082D410);
    fillValue.vtbl = CSF_VTBL(0x0082D428);
    fillValue.vtbl = CSF_VTBL(0x008313F8);
    fillValue.field_0004 = 0;
    fillValue.field_0008 = 0xFFFFFFFF;

    char* result = S_0046f5e0_Resize(
        reinterpret_cast<S_0046f5e0*>(destination + 4),
        nullptr, count, fillValue.vtbl,
        fillValue.field_0004, fillValue.field_0008);

    uint32_t sourceOffset = 0;
    uint32_t valueOffset = 0;
    unsigned int remaining = count;
    while (remaining)
    {
        auto* entry = reinterpret_cast<uint8_t*>(
            CSF_AT(uint32_t, self, 0x8) + sourceOffset);
        void* value = reinterpret_cast<void*>(
            CSF_AT(uint32_t, destination, 0x8) + valueOffset);

        if (CSF_AT(uint8_t, entry, 0x8))
        {
            MissionScript_AssignValueReference(
                value, nullptr, CSF_AT(int, entry, 0x4));
        }
        else
        {
            ReserveScratchEntries(
                value, nullptr, CSF_AT(int, entry, 0x4));
            MissionScript_CopyEntryToValue(
                value, nullptr,
                static_cast<int32_t>(reinterpret_cast<uintptr_t>(entry)));
        }

        sourceOffset += 68;
        valueOffset += 12;
        --remaining;
        result = reinterpret_cast<char*>(remaining);
    }

    return result;
}

HOOK(0x00470E80, MissionScript_PopulateValues, "auto");
