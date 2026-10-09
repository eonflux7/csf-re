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

// Forwards four stack arguments to a widget's rectangle/position interface.
// Original: __thiscall, ret 0x10. Written as __fastcall (ecx = self, edx unused).
static int __fastcall Hud_ApplyWidgetRectangle(void* self, void* /*edx*/, int a2, int a3, int a4, int a5)
{
    uint32_t sourceA[2];  // [ebp-0x10]: fed to slot 14
    uint32_t sourceB[2];  // [ebp-0x08]: fed to slot 10
    uint32_t rect[4];     // [ebp-0x20]: fed to slots 22 and 21

    // slot 15 (+0x3C): getter returning pointer to two dwords -> sourceA
    {
        void* widget = CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
        const uint32_t* values = static_cast<const uint32_t*>(
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[15])(widget, nullptr));
        sourceA[0] = values[0];
        sourceA[1] = values[1];
    }

    // slot 11 (+0x2C): getter returning pointer to two dwords -> sourceB
    {
        void* widget = CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
        const uint32_t* values = static_cast<const uint32_t*>(
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, widget, 0)[11])(widget, nullptr));
        sourceB[0] = values[0];
        sourceB[1] = values[1];
    }

    // slot 22 (+0x58): fills rect from the widget
    {
        void* widget = CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(CSF_AT(void**, widget, 0)[22])(
            widget, nullptr, rect);
    }

    // slot 10 (+0x28): sets the first value of sourceB to a2
    sourceB[0] = static_cast<uint32_t>(a2);
    {
        void* widget = CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(CSF_AT(void**, widget, 0)[10])(
            widget, nullptr, sourceB);
    }

    // slot 14 (+0x38): sets the first value of sourceA to a3
    sourceA[0] = static_cast<uint32_t>(a3);
    {
        void* widget = CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
        reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t*)>(CSF_AT(void**, widget, 0)[14])(
            widget, nullptr, sourceA);
    }

    // slot 21 (+0x54): applies rect (a4 at [0], a5 at [2]); its result is returned
    rect[0] = static_cast<uint32_t>(a4);
    rect[2] = static_cast<uint32_t>(a5);
    {
        void* widget = CSF_AT(void*, CSF_AT(void*, self, 0x4), 0x8);
        return reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t*)>(CSF_AT(void**, widget, 0)[21])(
            widget, nullptr, rect);
    }
}

HOOK(0x42E950, Hud_ApplyWidgetRectangle, "auto");
