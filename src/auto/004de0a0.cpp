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

CSF_FUNC(uint8_t*, __fastcall, C_00867260_GetField0084, 0x0048E450, (C_00867260* self, void* /*edx*/));
CSF_FUNC(int, __fastcall, S_004ddf20_ApplyFieldValue, 0x004DE010, (void* self, void* edx, int a2));

// Forwards a field of a2 to the child handler when this->field_001c is set; always reports success.
static char __fastcall S_004ddf20_ForwardFieldWhenEnabled(S_004ddf20* self, void* /*edx*/, C_00867260* a2)
{
    if (self->field_001c)
    {
        uint8_t* fieldValue = C_00867260_GetField0084(a2, nullptr);
        S_004ddf20_ApplyFieldValue(self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(fieldValue)));
    }
    return 1;
}

HOOK(0x004DE0A0, S_004ddf20_ForwardFieldWhenEnabled, "auto");
