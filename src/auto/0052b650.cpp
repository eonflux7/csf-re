#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads the function pointer stored at byte offset `slotByteOffset` of obj's vtable.
static void* VtableEntry(void* obj, std::uint32_t slotByteOffset)
{
    return CSF_AT(void**, obj, 0)[slotByteOffset / 4];
}

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fDimOverlayAlpha, 0x008413BC);

CSF_FUNC(int, __fastcall, CMenuMessageBox_UpdateButtonLayout, 0x0052B020, (CMenuMessageBox* self, void* edx));

static int __fastcall CMenuMessageBox_RefreshLayoutAndButtons(CMenuMessageBox* self, void* /*edx*/)
{
    // child = self->field_0894->vtbl[+0x1CC]()
    void* frame = reinterpret_cast<void*>(self->field_0894);
    void* child = ((void*(__fastcall*)(void*, void*))VtableEntry(frame, 0x1CC))(frame, nullptr);
    if (child == nullptr)
        return 0;

    // child->vtbl[+0x24](&field_0990): writes extent (width at 0x990, height at 0x994)
    ((void(__fastcall*)(void*, void*, void*))VtableEntry(child, 0x24))(child, nullptr, &self->field_0990);
    // child->vtbl[+0x1C](&field_0988): writes origin (x at 0x988, y at 0x98C)
    ((void(__fastcall*)(void*, void*, void*))VtableEntry(child, 0x1C))(child, nullptr, &self->field_0988);

    // rect = { x, y, x + width, y + height }, read after the calls above
    std::uint32_t rect[4];
    rect[0] = self->field_0988;                                  // left
    rect[1] = self->field_098c;                                  // top
    rect[2] = self->field_0990 + self->field_0988;               // right
    rect[3] = CSF_AT(std::uint32_t, self, 0x994) + self->field_098c; // bottom

    // self->field_0894->vtbl[+0x208](&rect, 0)
    frame = reinterpret_cast<void*>(self->field_0894);
    ((void(__fastcall*)(void*, void*, std::uint32_t*, std::uint32_t))VtableEntry(frame, 0x208))(frame, nullptr, rect, 0u);

    // colorTarget = self->field_0894->vtbl[+0x1DC]()
    frame = reinterpret_cast<void*>(self->field_0894);
    void* colorTarget = ((void*(__fastcall*)(void*, void*))VtableEntry(frame, 0x1DC))(frame, nullptr);
    if (colorTarget != nullptr)
    {
        // Overlay color: black with alpha chosen by field_0949
        const float overlayAlpha = (self->field_0949 != 0) ? g_fDimOverlayAlpha : g_fZero;
        float overlayColor[4] = { 0.0f, 0.0f, 0.0f, overlayAlpha };
        // colorTarget->vtbl[+0x2C](overlayColor)
        ((void(__fastcall*)(void*, void*, float*))VtableEntry(colorTarget, 0x2C))(colorTarget, nullptr, overlayColor);
    }

    self->field_0028 = 2;
    self->field_09ac = 0;

    // The return value of this call is not used directly, but its upper 24 bits
    // are forwarded below because the original only overwrites the low byte (sete al).
    const int buttonLayoutResult = CMenuMessageBox_UpdateButtonLayout(self, nullptr);
    const std::uint32_t isButtonFieldZero = (self->field_08a8 == 0) ? 1u : 0u;
    const std::uint32_t flagArg =
        (static_cast<std::uint32_t>(buttonLayoutResult) & 0xFFFFFF00u) | isButtonFieldZero;

    // self->field_0894->vtbl[+0x25C](flagArg)
    frame = reinterpret_cast<void*>(self->field_0894);
    ((void(__fastcall*)(void*, void*, std::uint32_t))VtableEntry(frame, 0x25C))(frame, nullptr, flagArg);

    // self->field_0894->vtbl[+0x16C]()
    frame = reinterpret_cast<void*>(self->field_0894);
    ((void(__fastcall*)(void*, void*))VtableEntry(frame, 0x16C))(frame, nullptr);

    // return self->field_0894->vtbl[+0xC0](field_0948)
    frame = reinterpret_cast<void*>(self->field_0894);
    const std::uint32_t byteFlag = self->field_0948;
    return ((int(__fastcall*)(void*, void*, std::uint32_t))VtableEntry(frame, 0xC0))(frame, nullptr, byteFlag);
}

HOOK(0x0052B650, CMenuMessageBox_RefreshLayoutAndButtons, "auto");
