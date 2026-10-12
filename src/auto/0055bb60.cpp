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

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(uint32_t*, __fastcall, StateBlock_Reset_55AF70, 0x0055AF70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0086c2b4_SetupFieldBuffer, 0x0069DB80, (void* self, void* edx, int a2, int a3, unsigned int a4));

// Constructs a C_008449a0 entry from a C string: zeroes the header, resets the
// embedded state block, copies the string into the inline buffer at +0x140, then
// stores a single flag byte (via SetupFieldBuffer) through the caller's char* output.
static C_008449a0* __fastcall C_008449a0_InitFromKey(C_008449a0* self, void* /*edx*/, char* a2, char* a3)
{
    char* valueText = nullptr;

    self->field_0004 = 0;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008449A0);
    StateBlock_Reset_55AF70(reinterpret_cast<void*>(self->pad_0008), nullptr);

    self->field_0158 = 15;
    self->field_0154 = 0;
    *reinterpret_cast<uint8_t*>(&self->field_0144) = 0;

    AssignStringRange_429B60(reinterpret_cast<void*>(&self->pad_0018[296]), nullptr, a2, static_cast<unsigned int>(strlen(a2)));

    if (self->field_0158 < 0x10)
        valueText = reinterpret_cast<char*>(&self->field_0144);
    else
        valueText = reinterpret_cast<char*>(self->field_0144);

    *a3 = C_0086c2b4_SetupFieldBuffer(self, nullptr, reinterpret_cast<int>(valueText), reinterpret_cast<int>(self->pad_0008), 1u);
    return self;
}

HOOK(0x0055BB60, C_008449a0_InitFromKey, "auto");
