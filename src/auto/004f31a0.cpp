#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static std::uint32_t* __fastcall C_0083f0ec_ConstructSubtitlePanel(
    C_0083f0ec* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0083F0EC);
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    auto* textBytes = reinterpret_cast<std::uint8_t*>(self) + 0x38;
    CSF_AT(std::uint32_t, self, 0x50) = 7;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;
    CSF_AT(std::uint16_t, self, 0x3C) = 0;
    self->field_005c = 0;
    self->field_0060 = 0;

    alignas(8) std::uint8_t resultBytes[0x8];
    void* object = LookupObjectById(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    CSF_AT(void*, resultBytes, 0x4) = object;
    CSF_AT(int, resultBytes, 0x0) = GetGlobalValue_8BC1E0();

    void* dimensions =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    void* context = reinterpret_cast<void*>(
        CSF_AT(int, resultBytes, 0x0));

    self->field_0030 = static_cast<std::uint32_t>(
        static_cast<std::int64_t>(
            static_cast<double>(CSF_AT(std::int32_t, dimensions, 0x4)) *
            CSF_AT(float, context, 0x1830)));
    self->field_0020 = 0;

    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x50);
    const std::uint32_t endBase = capacity < 8
        ? reinterpret_cast<std::uint32_t>(textBytes + 4)
        : CSF_AT(std::uint32_t, self, 0x3C);
    const std::uint32_t last =
        endBase + 2 * CSF_AT(std::uint32_t, self, 0x4C);
    const std::uint32_t first = capacity < 8
        ? reinterpret_cast<std::uint32_t>(textBytes + 4)
        : CSF_AT(std::uint32_t, self, 0x3C);

    // The packet also identifies this call target as a vtable reference.
    (void)CSF_VTBL(0x004F21D0);
    EraseWideStringRange(
        reinterpret_cast<legacy::wstring*>(textBytes), nullptr,
        reinterpret_cast<int*>(resultBytes), first, last);

    self->field_0054 = 0;
    self->field_0058 = 0;
    self->field_0059 = 0;
    self->field_0024 = 0;
    return reinterpret_cast<std::uint32_t*>(self);
}

HOOK(0x004F31A0, C_0083f0ec_ConstructSubtitlePanel, "auto");
