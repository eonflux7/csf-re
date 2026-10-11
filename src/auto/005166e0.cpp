#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(unsigned int, __fastcall, ProfilesSavegames_SetDataBufferSize, 0x00516320, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, ProfilesSavegames_InitializeDataBuffer, 0x00516410, (void* self, void* edx, unsigned int a4a, int arg8, int _18, int a5, int a6));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

[[maybe_unused]] static const auto kReportedStringVtable = CSF_VTBL(0x004F21D0);

static unsigned int __fastcall S_004f21d0_ResetSaveData(
    S_004f21d0* self, void* /*edx*/)
{
    const std::uint32_t capacity = self->field_0018;
    auto* inlineText = &self->field_0004;

    std::uint16_t* endBase = inlineText;
    if (capacity >= 8)
        endBase = CSF_AT(std::uint16_t*, self, 0x04);

    const std::uint32_t last =
        reinterpret_cast<std::uint32_t>(endBase) + self->field_0014 * 2;

    std::uint16_t* first = inlineText;
    if (capacity >= 8)
        first = CSF_AT(std::uint16_t*, self, 0x04);

    alignas(8) std::uint8_t eraseResultBytes[0x4];
    EraseWideStringRange(
        reinterpret_cast<legacy::wstring*>(self), nullptr,
        reinterpret_cast<int*>(eraseResultBytes),
        reinterpret_cast<std::uint32_t>(first), last);

    self->field_0048 = 0xFFFFFFFF;
    reinterpret_cast<std::uint8_t*>(&self->field_004c)[0] = 0;
    reinterpret_cast<std::uint8_t*>(&self->field_004c)[1] = 0;

    if (CSF_AT(std::uint32_t, self, 0x38))
        Mem_FreeDebug(reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x38)));

    CSF_AT(std::uint32_t, self, 0x38) = 0;
    CSF_AT(std::uint32_t, self, 0x3C) = 0;
    CSF_AT(std::uint32_t, self, 0x40) = 0;

    auto* dataBuffer = reinterpret_cast<std::uint8_t*>(self) + 0x34;
    ProfilesSavegames_InitializeDataBuffer(
        dataBuffer, nullptr, 0, -1, 0, 0, 0);
    const unsigned int result =
        ProfilesSavegames_SetDataBufferSize(dataBuffer, nullptr, 0x14);

    reinterpret_cast<std::uint8_t*>(&self->field_0044)[0] = 0;
    return result;
}

HOOK(0x005166E0, S_004f21d0_ResetSaveData, "auto");
