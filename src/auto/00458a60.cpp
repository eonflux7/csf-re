#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x458760 takes 'this' in ecx (a4) plus two stack args and pops 8.
// The packet's bind line says __stdcall, but the disassembly sets ecx = a4 before the call,
// so it is bound here as a __fastcall-style method (self in ecx, edx unused).
CSF_FUNC(uint32_t*, __fastcall, HudStatusIcons_ApplyElement, 0x00458760, (void* self, void* edx, uint32_t* element, uint32_t* a3));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

static void __cdecl HudStatusIcons_ForEachElement(uint32_t* elements, int count, uint32_t* a3, void* a4, int a5)
{
    (void)a5;
    uint32_t* start = elements;
    uint32_t* current = elements;
    try {
        while (count != 0) {
            HudStatusIcons_ApplyElement(a4, nullptr, current, a3);
            --count;
            current += 10; // 0x28-byte element stride
        }
    } catch (...) {
        // Cleanup path: elements already set up before the failing one get their vtable reset.
        for (uint32_t* p = start; p != current; p += 10) {
            CSF_AT(void**, p, 0x1C) = CSF_VTBL(0x0082D410);
        }
        RethrowCppException(nullptr, nullptr);
    }
}

HOOK(0x458A60, HudStatusIcons_ForEachElement, "auto");
