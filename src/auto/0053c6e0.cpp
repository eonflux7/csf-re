#include <cmath>
#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectByHandle, 0x00652160,
         (int handle, void* edx));
CSF_FUNC(uint8_t*, __fastcall, LookupNamedEntry, 0x00693360,
         (void* table, void* edx, const char* name));

static uint64_t MenuSoundOptions_UnsignedValue(uint32_t bits)
{
    const int64_t signedValue = static_cast<int32_t>(bits);
    if (signedValue < 0) {
        return static_cast<uint64_t>(
            signedValue + static_cast<int64_t>(g_UnsignedIntegerBias));
    }
    return static_cast<uint64_t>(signedValue);
}

static float MenuSoundOptions_BitsToFloat(uint32_t bits)
{
    float value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

static int MenuSoundOptions_BitLength(uint64_t value)
{
    int length = 0;
    while (value != 0) {
        value >>= 1;
        ++length;
    }
    return length;
}

static uint32_t MenuSoundOptions_NextQuotientBit(uint64_t& remainder,
                                                uint64_t divisor)
{
    remainder <<= 1;
    if (remainder >= divisor) {
        remainder -= divisor;
        return 1;
    }
    return 0;
}

// Reproduce extended-precision division followed by the float store:
// round first to 64 significant bits, then to 24, both nearest-even.
static float MenuSoundOptions_RatioToFloat(uint32_t numeratorBits,
                                           uint32_t denominatorBits)
{
    const uint64_t numerator = MenuSoundOptions_UnsignedValue(numeratorBits);
    const uint64_t denominator = MenuSoundOptions_UnsignedValue(denominatorBits);
    if (denominator == 0) {
        return MenuSoundOptions_BitsToFloat(
            numerator == 0 ? 0xFFC00000u : 0x7F800000u);
    }
    if (numerator == 0) {
        return 0.0f;
    }

    uint64_t remainder = numerator % denominator;
    const uint64_t integerPart = numerator / denominator;
    uint64_t significand = 0;
    int exponent = 0;

    if (integerPart != 0) {
        const int integerBits = MenuSoundOptions_BitLength(integerPart);
        significand = integerPart << (64 - integerBits);
        exponent = integerBits - 64;
        for (int position = 63 - integerBits; position >= 0; --position) {
            if (MenuSoundOptions_NextQuotientBit(remainder, denominator) != 0) {
                significand |= 1ULL << position;
            }
        }
    } else {
        int fractionPosition = 0;
        uint32_t bit;
        do {
            bit = MenuSoundOptions_NextQuotientBit(remainder, denominator);
            ++fractionPosition;
        } while (bit == 0);

        significand = 1ULL << 63;
        exponent = -fractionPosition - 63;
        for (int position = 62; position >= 0; --position) {
            if (MenuSoundOptions_NextQuotientBit(remainder, denominator) != 0) {
                significand |= 1ULL << position;
            }
        }
    }

    const uint32_t roundBit =
        MenuSoundOptions_NextQuotientBit(remainder, denominator);
    const bool stickyBits = remainder != 0;

    if (roundBit != 0 && (stickyBits || (significand & 1ULL) != 0)) {
        ++significand;
        if (significand == 0) {
            significand = 1ULL << 63;
            ++exponent;
        }
    }

    uint64_t mantissa24 = significand >> 40;
    const uint64_t lowBits = significand & ((1ULL << 40) - 1);
    const uint64_t halfUnit = 1ULL << 39;
    if (lowBits > halfUnit ||
        (lowBits == halfUnit && (mantissa24 & 1ULL) != 0)) {
        ++mantissa24;
    }
    if (mantissa24 == (1ULL << 24)) {
        mantissa24 >>= 1;
        ++exponent;
    }

    return std::ldexp(static_cast<float>(mantissa24), exponent + 40);
}

static int __fastcall CMenuSoundOptions_InitPanelAndSliders(
    CMenuSoundOptions* self, void* /*edx*/,
    void* ownerObject, void* tableHolder)
{
    const uint32_t cachedPanel = self->field_001c;
    if (cachedPanel != 0) {
        return static_cast<int>(cachedPanel);
    }

    void* rootObject = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, ownerObject, 0)[0x54 / 4])(ownerObject, nullptr);

    // This vtable is loaded before the name lookup in the original.
    void** rootVtable = CSF_AT(void**, rootObject, 0);
    uint8_t* panelEntry = LookupNamedEntry(
        *static_cast<void**>(tableHolder), nullptr,
        CSF_STR(0x00841304, ".PANEL_PRINCIPAL"));

    void* panel = reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
        rootVtable[0x18 / 4])(rootObject, nullptr, panelEntry);
    self->field_001c = reinterpret_cast<uint32_t>(panel);
    if (panel == nullptr) {
        return 0;
    }

    void* xHandleObject = LookupObjectByHandle(GetGlobalInt(), nullptr);
    void* xResult = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, xHandleObject, 0)[0x18 / 4])(xHandleObject, nullptr);
    const uint32_t xRawBits = CSF_AT(uint32_t, xResult, 0);

    void* yHandleObject = LookupObjectByHandle(GetGlobalInt(), nullptr);
    void* yResult = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, yHandleObject, 0)[0x18 / 4])(yHandleObject, nullptr);
    const uint32_t yRawBits = CSF_AT(uint32_t, yResult, 0x04);

    const float yRatio = MenuSoundOptions_RatioToFloat(yRawBits, g_ScreenHeight);
    const float xRatio = MenuSoundOptions_RatioToFloat(xRawBits, g_ScreenWidth);

    void* panelForPlacement = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void (__fastcall*)(void*, void*, float, float)>(
        CSF_AT(void**, panelForPlacement, 0)[0x150 / 4])(
            panelForPlacement, nullptr, xRatio, yRatio);

    void* panelForMusic = reinterpret_cast<void*>(self->field_001c);
    void* musicSlider =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panelForMusic, 0)[0xFC / 4])(
                panelForMusic, nullptr, CSF_STR(0x00842A58, "SliderMusic"));
    self->field_0040 = reinterpret_cast<uint32_t>(musicSlider);
    if (musicSlider != nullptr) {
        int musicValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, musicSlider, 0)[0x244 / 4])(musicSlider, nullptr);
        void* musicSliderNow = reinterpret_cast<void*>(self->field_0040);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, musicSliderNow, 0)[0x158 / 4])(
                musicSliderNow, nullptr, musicValue);
    }

    void* panelForFx = reinterpret_cast<void*>(self->field_001c);
    void* fxSlider =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, panelForFx, 0)[0xFC / 4])(
                panelForFx, nullptr, CSF_STR(0x00842A4C, "SliderFX"));
    self->field_0044 = reinterpret_cast<uint32_t>(fxSlider);
    if (fxSlider == nullptr) {
        return 0;
    }

    int fxValue = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, fxSlider, 0)[0x244 / 4])(fxSlider, nullptr);
    void* fxSliderNow = reinterpret_cast<void*>(self->field_0044);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, fxSliderNow, 0)[0x158 / 4])(
            fxSliderNow, nullptr, fxValue);
}

HOOK(0x0053C6E0, CMenuSoundOptions_InitPanelAndSliders, "auto");
