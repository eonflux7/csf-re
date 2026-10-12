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

CSF_FUNC(void*, __fastcall, sub_760E20, 0x00760E20, (void* a1, int a2));
CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(char, __fastcall, C_0086c2b4_SetupFieldBuffer, 0x0069DB80, (void* self, void* edx, int a2, int a3, unsigned int a4));

// Initializes a C_0086c2b4 from a source text (a3) and writes the setup
// result byte into *a4. Returns self. The register edx (a2) is forwarded
// unchanged to sub_760E20. Callee pops 8 bytes (a3, a4).
static C_0086c2b4* __fastcall C_0086c2b4_InitAndStoreFieldText(C_0086c2b4* self, int a2, const char* a3, char* a4)
{
    uint8_t* pad0008 = reinterpret_cast<uint8_t*>(self) + 0x08;

    self->field_0004 = 0;
    self->vtbl = CSF_VTBL(0x0086C2B4);

    sub_760E20(pad0008, a2);

    CSF_AT(uint32_t, self, 0x12C) = 15;
    CSF_AT(uint32_t, self, 0x128) = 0;
    CSF_AT(uint8_t, self, 0x118) = 0;

    // Inline strlen loop, as in the original (no CRT call).
    size_t textLength = 0;
    while (a3[textLength] != '\0')
        ++textLength;

    AssignStringRange_429B60(reinterpret_cast<uint8_t*>(self) + 0x114, nullptr,
                             const_cast<char*>(a3),
                             static_cast<unsigned int>(textLength));

    char* textBuffer;
    if (CSF_AT(uint32_t, self, 0x12C) < 0x10)
        textBuffer = reinterpret_cast<char*>(reinterpret_cast<uint8_t*>(self) + 0x118);
    else
        textBuffer = reinterpret_cast<char*>(CSF_AT(uint32_t, self, 0x118));

    *a4 = C_0086c2b4_SetupFieldBuffer(self, nullptr,
                                      static_cast<int>(reinterpret_cast<uintptr_t>(textBuffer)),
                                      static_cast<int>(reinterpret_cast<uintptr_t>(pad0008)),
                                      0u);
    return self;
}

HOOK(0x00760EC0, C_0086c2b4_InitAndStoreFieldText, "auto");
