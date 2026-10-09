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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_48FE90, 0x0048FE90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4DA920, 0x004DA920, (void* self, void* edx));
CSF_FUNC(int, __cdecl, sub_755880, 0x00755880, (int a1));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall with 3 stack args (ret 0xC). self = ecx.
static int __fastcall ApplyHandleMessageToControl(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint8_t* outerBase = reinterpret_cast<uint8_t*>(*a4);
    uint8_t* v5 = *reinterpret_cast<uint8_t**>(outerBase + 4);
    int result = *reinterpret_cast<int32_t*>(v5 + 12);
    uint8_t* v7 = v5 + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(v7)));
        C_00833908* control = reinterpret_cast<C_00833908*>(result);
        if (result)
        {
            result = sub_755880(control->field_0050);
            if (static_cast<uint8_t>(result))
            {
                if (control->field_0050 == 4)
                {
                    sub_4DA920(reinterpret_cast<void*>(control->field_0034), nullptr);
                    CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(v7 + 4);
                    result = *reinterpret_cast<int32_t*>(v7 + 8);
                    CSF_AT(int32_t, self, 0x64) = result;
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
                else
                {
                    return sub_48FE90(control, nullptr);
                }
            }
        }
    }
    return result;
}

HOOK(0x460BB0, ApplyHandleMessageToControl, "auto");
