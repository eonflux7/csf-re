#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x42D6D0: cdecl, no stack args, returns the global 8afc78 object.
// The original pushes a value before calling it and never cleans the stack;
// that pushed value is consumed later as the stack argument of 0x431F10 / 0x42E150 / 0x42E1A0.
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, sub_431F10, 0x00431F10, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_42E150, 0x0042E150, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, sub_42E1A0, 0x0042E1A0, (void* self, void* edx, int a2));
// 0x42F4F0: original is __usercall with a1 in edi (the widget object) and stack args (a2, a3).
// Bound here with the stack arguments; see notes about the edi input.
CSF_FUNC(void, __cdecl, sub_42F4F0, 0x0042F4F0, (int* a2, int a3));

using WidgetRectSetFn = void(__fastcall*)(void* self, void* edx, int32_t* rect);

static void* WidgetPtrFromInt(int32_t value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(value)));
}

static int __fastcall Hud_RefreshWidgetPlacement(int* self, void* /*edx*/) {
    int32_t widgetSlot = CSF_AT(int32_t, self, 0x30);

    int32_t globalObject = GetGlobal8afc78();
    int32_t lookupResult = sub_431F10(WidgetPtrFromInt(globalObject), nullptr, widgetSlot);
    int32_t* widget = reinterpret_cast<int32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(lookupResult)));
    if (widget == nullptr) {
        return lookupResult;
    }

    int32_t* widgetPtrArg = widget;
    globalObject = GetGlobal8afc78();
    sub_42E150(WidgetPtrFromInt(globalObject), nullptr, widgetPtrArg);

    // Position: first vtable slot (offset 40) with origin (+0x20, +0x24).
    int32_t originRect[2];
    originRect[0] = CSF_AT(int32_t, self, 0x20);
    originRect[1] = CSF_AT(int32_t, self, 0x24);
    reinterpret_cast<WidgetRectSetFn>(CSF_AT(void**, widget, 0)[10])(
        widget, nullptr, originRect);

    // Size: vtable slot (offset 56) with width/height from the far corner minus origin.
    int32_t sizeRect[2];
    sizeRect[0] = CSF_AT(int32_t, self, 0x28) - CSF_AT(int32_t, self, 0x20);
    sizeRect[1] = CSF_AT(int32_t, self, 0x2C) - CSF_AT(int32_t, self, 0x24);
    reinterpret_cast<WidgetRectSetFn>(CSF_AT(void**, widget, 0)[14])(
        widget, nullptr, sizeRect);

    sub_42F4F0(widget, 0);

    globalObject = GetGlobal8afc78();
    return sub_42E1A0(WidgetPtrFromInt(globalObject), nullptr, reinterpret_cast<int>(widget));
}

HOOK(0x0042D0B0, Hud_RefreshWidgetPlacement, "auto");
