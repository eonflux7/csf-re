#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __fastcall, HudStringShow_ShowWithStyle, 0x004F39E0,
         (void* self, void* edx, void* arg0, float a5, float a6, float a2));

static void __fastcall HudStringShow_ShowDefault(
    void* self, void* /*edx*/, void* text, float arg1, float arg2)
{
    const uint32_t defaultStyleAddress =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082E65C)));
    float defaultStyle;
    std::memcpy(&defaultStyle, &defaultStyleAddress, sizeof(defaultStyle));

    HudStringShow_ShowWithStyle(self, nullptr, text, defaultStyle, arg1, arg2);
}

HOOK(0x004F3EF0, HudStringShow_ShowDefault, "auto");
