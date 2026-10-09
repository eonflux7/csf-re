#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees (bound by address)
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(char, __fastcall, GlobalMenu_ApplyScenarioName, 0x004FF0F0, (void* self, void* edx, char* String1));

// Data
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void __fastcall GlobalMenu_SetSelectedScenarioName(S_004ff0f0* self, void* /*edx*/, char* String1, char a3)
{
    // Vtable of S_004ff0f0 referenced by the callee path (0x005019F5 -> 0x004FF0F0).
    (void)CSF_VTBL(0x004FF0F0);

    if (String1 == nullptr)
        return;

    // Sub-object at +0x7C: its first dword sits at +0x14 within it (i.e. this+0x90).
    void* nameSub = reinterpret_cast<uint8_t*>(self) + 0x7C;
    AssignStringRange_429B60(nameSub, nullptr, String1, static_cast<unsigned int>(std::strlen(String1)));

    if (CompareStringSubstring(nameSub, nullptr, 0,
                               CSF_AT(uint32_t, nameSub, 0x14),
                               reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        // Second sub-object at +0x98 is cleared to the empty string.
        void* pendingSub = reinterpret_cast<uint8_t*>(self) + 0x98;
        AssignStringRange_429B60(pendingSub, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

        self->field_005c = 1;

        if (a3)
            GlobalMenu_ApplyScenarioName(self, nullptr, String1);
    }
}

HOOK(0x00501990, GlobalMenu_SetSelectedScenarioName, "auto");
