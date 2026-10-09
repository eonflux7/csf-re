#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Float -> int conversion with the same result as the original's int64 conversion followed by
// taking the low 32 bits. Out-of-range or NaN inputs produce the int64 "indefinite" value
// 0x8000000000000000, whose low dword is 0.
static int32_t FloatToLowInt32(float value) {
    if (!(value >= -9223372036854775808.0f && value < 9223372036854775808.0f)) {
        return 0;
    }
    const int64_t wide = static_cast<int64_t>(value);
    return static_cast<int32_t>(wide);
}

// 32-bit signed multiply that keeps only the low dword, like imul on 32-bit registers.
static int32_t MulLowInt32(int32_t a, int32_t b) {
    return static_cast<int32_t>(static_cast<uint32_t>(a) * static_cast<uint32_t>(b));
}

// The holder pointer at self+0x14 points to the widget object; the widget is the first pointer inside the holder.
static void* HudBar_GetWidget(void* self) {
    void* holder = CSF_AT(void*, self, 0x14);
    return CSF_AT(void*, holder, 0);
}

// Converts a masked 0..255 color channel to float, with the unsigned-bias fixup for negative values.
static float HudBar_ChannelToFloat(int maskedChannel) {
    float channel = static_cast<float>(maskedChannel);
    if (maskedChannel < 0) {
        channel += g_UnsignedIntegerBias;
    }
    return channel;
}

// __thiscall (ecx = self). The single stack argument is never read by the original; ret 4 is kept.
static char __fastcall Hud_ResizeBarAndApplyColor(void* self, void* /*edx*/, int /*unusedArg*/) {
    if (CSF_AT(uint8_t, self, 0x119) == 0) {
        return 0;
    }

    void* sceneMenuCtx = reinterpret_cast<void*>(static_cast<uintptr_t>(GetSceneMenuContext(g_pCurrentScene, nullptr)));
    if (sceneMenuCtx == nullptr) {
        return 0;
    }

    // Divisor and scale are float fields of the scene menu context, each converted to int with __ftol2 semantics.
    const int32_t divisor = FloatToLowInt32(CSF_AT(float, sceneMenuCtx, 0x2EC));
    if (divisor == 0) {
        return 0;
    }
    const int32_t scale = FloatToLowInt32(CSF_AT(float, sceneMenuCtx, 0x2F0));
    const int32_t barWidth = MulLowInt32(scale, CSF_AT(int, self, 0x138)) / divisor;
    if (barWidth <= 0) {
        return static_cast<char>(barWidth);
    }

    // Slot 15 (byte offset 0x3C) returns a pointer to {width, height}.
    // The original reads the current width before overwriting it, so that read is kept.
    void* widget = HudBar_GetWidget(self);
    void* currentSize = ((void* (__fastcall*)(void*, void*))(CSF_AT(void**, widget, 0)[15]))(widget, nullptr);
    volatile int previousWidth = CSF_AT(int, currentSize, 0);
    (void)previousWidth;
    int newSize[2];
    newSize[0] = barWidth;
    newSize[1] = CSF_AT(int, currentSize, 4);
    widget = HudBar_GetWidget(self);
    ((void (__fastcall*)(void*, void*, int*))(CSF_AT(void**, widget, 0)[14]))(widget, nullptr, newSize);

    // Rect {left, top, right, bottom}: read it, move the right edge by the scaled offset, write it back.
    int barRect[4];
    widget = HudBar_GetWidget(self);
    ((void (__fastcall*)(void*, void*, int*))(CSF_AT(void**, widget, 0)[22]))(widget, nullptr, barRect);
    barRect[2] = barRect[0] + MulLowInt32(CSF_AT(int, self, 0x13C), barWidth) / CSF_AT(int, self, 0x138);
    widget = HudBar_GetWidget(self);
    ((void (__fastcall*)(void*, void*, int*))(CSF_AT(void**, widget, 0)[21]))(widget, nullptr, barRect);

    // Bar color: packed global value when the scene flag 0x800 is set, otherwise full white.
    float color[4];
    const uint32_t sceneFlags = CSF_AT(uint32_t, sceneMenuCtx, 0x2E0);
    if ((sceneFlags & 0x800) != 0) {
        void* globals = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
        const int packedColor = CSF_AT(int, globals, 0x1874);
        color[0] = HudBar_ChannelToFloat((packedColor >> 16) & 0xFF);
        color[1] = HudBar_ChannelToFloat((packedColor >> 8) & 0xFF);
        color[2] = HudBar_ChannelToFloat(packedColor & 0xFF);
        color[3] = 255.0f;
    } else {
        color[0] = 255.0f;
        color[1] = 255.0f;
        color[2] = 255.0f;
        color[3] = 255.0f;
    }

    widget = HudBar_GetWidget(self);
    const int colorResult = ((int (__fastcall*)(void*, void*, float*))(CSF_AT(void**, widget, 0)[26]))(widget, nullptr, color);
    return static_cast<char>(colorResult);
}

HOOK(0x0042E9E0, Hud_ResizeBarAndApplyColor, "auto");
