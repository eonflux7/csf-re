#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, CopyPrimaryCollection, 0x00733DC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CopySecondaryCollection, 0x00733E90, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CopyTertiaryCollection, 0x00733F50, (void* self, void* edx, int a2));

static void* __fastcall C_008670f0_ReallocateOwnedData(
    C_008670f0* self, void* /*edx*/)
{
    // The leading four bytes and the following 0x18 bytes form one string.
    alignas(8) std::uint8_t stringBytes[0x1C]{};
    alignas(8) std::uint8_t primaryBytes[0x10];
    alignas(8) std::uint8_t secondaryBytes[0x10];
    alignas(8) std::uint8_t tertiaryBytes[0x10];
    alignas(8) std::uint8_t eraseResultBytes[0x8]{};
    auto* eraseResult = reinterpret_cast<int*>(eraseResultBytes);

    CSF_AT(std::uint16_t, stringBytes, 0x04) = 0;
    CSF_AT(std::uint32_t, stringBytes, 0x14) = 0;
    CSF_AT(std::uint32_t, stringBytes, 0x18) = 7;

    // The packet also labels the direct erase callee as a vtable reference.
    (void)CSF_VTBL(0x004F21D0);

    AssignStringRange_488830(stringBytes, nullptr, self->pad_0020, 0, 0xFFFFFFFF);
    std::uint32_t capacity = self->field_0038;
    std::uint32_t endBase = capacity < 8
        ? reinterpret_cast<std::uint32_t>(&self->field_0024)
        : self->field_0024;
    std::uint32_t last = endBase + 2 * self->field_0034;
    std::uint32_t first = capacity < 8
        ? reinterpret_cast<std::uint32_t>(&self->field_0024)
        : self->field_0024;
    EraseWideStringRange(reinterpret_cast<legacy::wstring*>(self->pad_0020),
                         nullptr, eraseResult, first, last);
    SetAllocationMode(2);
    AssignStringRange_488830(self->pad_0020, nullptr, stringBytes, 0, 0xFFFFFFFF);
    SetAllocationMode(0);

    AssignStringRange_488830(stringBytes, nullptr, self->pad_003c, 0, 0xFFFFFFFF);
    capacity = self->field_0054;
    endBase = capacity < 8
        ? reinterpret_cast<std::uint32_t>(&self->field_0040)
        : self->field_0040;
    last = endBase + 2 * self->field_0050;
    first = capacity < 8
        ? reinterpret_cast<std::uint32_t>(&self->field_0040)
        : self->field_0040;
    EraseWideStringRange(reinterpret_cast<legacy::wstring*>(self->pad_003c),
                         nullptr, eraseResult, first, last);
    SetAllocationMode(2);
    AssignStringRange_488830(self->pad_003c, nullptr, stringBytes, 0, 0xFFFFFFFF);
    SetAllocationMode(0);

    capacity = CSF_AT(std::uint32_t, stringBytes, 0x18);
    std::uint32_t stringStorage = CSF_AT(std::uint32_t, stringBytes, 0x04);
    first = capacity < 8
        ? reinterpret_cast<std::uint32_t>(stringBytes + 0x04)
        : stringStorage;
    last = first + 2 * CSF_AT(std::uint32_t, stringBytes, 0x14);
    EraseWideStringRange(reinterpret_cast<legacy::wstring*>(stringBytes),
                         nullptr, eraseResult, first, last);

    std::memset(primaryBytes, 0, 12);
    CSF_AT(std::uint32_t, primaryBytes, 0x0C) = 10;
    CopyPrimaryCollection(primaryBytes, nullptr, reinterpret_cast<int>(&self->field_0068));
    std::uint32_t oldStorage = self->field_0068;
    self->field_006c = 0;
    self->field_0070 = 0;
    if (oldStorage) {
        Mem_Free(reinterpret_cast<void*>(oldStorage));
        self->field_0068 = 0;
    }
    SetAllocationMode(2);
    CopyPrimaryCollection(&self->field_0068, nullptr, reinterpret_cast<int>(primaryBytes));
    SetAllocationMode(0);

    CopyPrimaryCollection(primaryBytes, nullptr, reinterpret_cast<int>(&self->field_0058));
    oldStorage = self->field_0058;
    self->field_005c = 0;
    self->field_0060 = 0;
    if (oldStorage) {
        Mem_Free(reinterpret_cast<void*>(oldStorage));
        self->field_0058 = 0;
    }
    SetAllocationMode(2);
    CopyPrimaryCollection(&self->field_0058, nullptr, reinterpret_cast<int>(primaryBytes));
    SetAllocationMode(0);
    if (CSF_AT(std::uint32_t, primaryBytes, 0)) {
        Mem_Free(CSF_AT(void*, primaryBytes, 0));
        CSF_AT(std::uint32_t, primaryBytes, 0) = 0;
    }

    std::memset(secondaryBytes, 0, 12);
    CSF_AT(std::uint32_t, secondaryBytes, 0x0C) = 10;
    CopySecondaryCollection(secondaryBytes, nullptr, reinterpret_cast<int>(&self->field_0078));
    oldStorage = self->field_0078;
    self->field_007c = 0;
    self->field_0080 = 0;
    if (oldStorage) {
        Mem_Free(reinterpret_cast<void*>(oldStorage));
        self->field_0078 = 0;
    }
    SetAllocationMode(2);
    CopySecondaryCollection(&self->field_0078, nullptr, reinterpret_cast<int>(secondaryBytes));
    SetAllocationMode(0);
    if (CSF_AT(std::uint32_t, secondaryBytes, 0)) {
        Mem_Free(CSF_AT(void*, secondaryBytes, 0));
        CSF_AT(std::uint32_t, secondaryBytes, 0) = 0;
    }

    std::memset(tertiaryBytes, 0, 12);
    CSF_AT(std::uint32_t, tertiaryBytes, 0x0C) = 10;
    CopyTertiaryCollection(tertiaryBytes, nullptr, reinterpret_cast<int>(&self->field_0088));
    oldStorage = self->field_0088;
    self->field_008c = 0;
    self->field_0090 = 0;
    if (oldStorage) {
        Mem_Free(reinterpret_cast<void*>(oldStorage));
        self->field_0088 = 0;
    }
    SetAllocationMode(2);
    CopyTertiaryCollection(&self->field_0088, nullptr, reinterpret_cast<int>(tertiaryBytes));
    SetAllocationMode(0);

    if (CSF_AT(std::uint32_t, tertiaryBytes, 0))
        Mem_Free(CSF_AT(void*, tertiaryBytes, 0));
    if (CSF_AT(std::uint32_t, secondaryBytes, 0))
        Mem_Free(CSF_AT(void*, secondaryBytes, 0));

    void* result = CSF_AT(void*, primaryBytes, 0);
    if (result)
        result = reinterpret_cast<void*>(Mem_Free(result));

    if (CSF_AT(std::uint32_t, stringBytes, 0x18) >= 8) {
        result = CSF_AT(void*, stringBytes, 0x04);
        if (result)
            return reinterpret_cast<void*>(Mem_FreeDebug(result));
    }
    return result;
}

HOOK(0x00735F90, C_008670f0_ReallocateOwnedData, "auto");
