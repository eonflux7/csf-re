#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int*, __fastcall, EraseWideStringRange, 0x004F21D0, (legacy::wstring* text, void* edx, int* result, std::uint32_t first, std::uint32_t last));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_EraseStringRange, 0x004F4770, (void* self, void* edx, uint32_t* a2, int a3, unsigned int a4));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_ResetEmbeddedState, 0x00500E90, (void* self, void* edx));

CSF_GLOBAL(std::uint32_t, g_SceneResetValue, 0x00840974);

// Hex-Rays folds the global's initial value to this constant.
// The actual instruction reads the global, so preserve that runtime read.
static constexpr std::uint32_t kInitialSceneResetValue = 1527564717;
static_assert(kInitialSceneResetValue == 0x5B0CC9AD);

static int __fastcall S_004f21d0_Reset(S_004f21d0* self, void* /*edx*/)
{
    (void)CSF_VTBL(0x004F21D0);

    int eraseResult = 0;
    auto* inlineWideText = &self->field_0004;
    const std::uint32_t wideCapacity = self->field_0018;

    auto* wideEndBase = wideCapacity < 8
        ? inlineWideText
        : CSF_AT(std::uint16_t*, self, 0x04);
    const std::uint32_t wideEnd =
        reinterpret_cast<std::uint32_t>(wideEndBase) + self->field_0014 * 2;
    auto* wideBegin = wideCapacity < 8
        ? inlineWideText
        : CSF_AT(std::uint16_t*, self, 0x04);

    EraseWideStringRange(
        reinterpret_cast<legacy::wstring*>(self), nullptr, &eraseResult,
        reinterpret_cast<std::uint32_t>(wideBegin), wideEnd);

    self->field_002c = -1;
    self->field_0028 = -1;
    self->field_0024 = -1;
    self->field_0020 = -1;
    self->field_001c = -1;
    self->field_0030 = 0;
    self->field_0034 = -1;
    self->field_0048 = -1;
    self->field_0044 = -1;
    self->field_0040 = -1;
    self->field_003c = -1;
    self->field_0038 = -1;
    self->field_004c = 0;

    auto* bytes = reinterpret_cast<std::uint8_t*>(self);
    const std::uint32_t stringCapacity = CSF_AT(std::uint32_t, self, 0x68);
    auto* stringEndBase = stringCapacity < 0x10
        ? bytes + 0x54
        : CSF_AT(std::uint8_t*, self, 0x54);
    const unsigned int stringEnd =
        reinterpret_cast<std::uint32_t>(stringEndBase)
        + CSF_AT(std::uint32_t, self, 0x64);
    auto* stringBegin = stringCapacity < 0x10
        ? bytes + 0x54
        : CSF_AT(std::uint8_t*, self, 0x54);

    SceneLoader_EraseStringRange(
        bytes + 0x50, nullptr,
        reinterpret_cast<std::uint32_t*>(&eraseResult),
        reinterpret_cast<int>(stringBegin), stringEnd);
    SceneLoader_ResetEmbeddedState(bytes + 0x6c, nullptr);

    self->field_00ac = -1;
    self->field_00b0 = -1;
    self->field_00b4 = -1;
    const std::uint32_t resetValue = g_SceneResetValue;
    self->field_00bc = -1;
    self->field_00c4 = -1;
    self->field_00b8 = resetValue;
    self->field_00c0 = 0;
    return static_cast<int>(resetValue);
}

HOOK(0x00500F10, S_004f21d0_Reset, "auto");
