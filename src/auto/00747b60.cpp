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

CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SharedDispatch_65E950, 0x0065E950, (void* self, void* edx, int a1, int a2, int a3));
CSF_FUNC(bool, __fastcall, sub_747700, 0x00747700, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, sub_7478C0, 0x007478C0, (void* self, void* edx, void* arg0));
CSF_FUNC(char, __fastcall, sub_7479A0, 0x007479A0, (void* self, void* edx, void* arg0));

// Dispatches an incoming message slot (16-bit id in a3) to the matching handler.
// Returns 1 when one of the four local slots matched; otherwise defers to SharedDispatch_65E950.
// Note: the original's [ebp+8] is the CMenuMpStats argument (a2), which it uses as the handler `this`.
static char __fastcall NetProyectil_HandleMessageSlot(NetProyectil* self, void* /*edx*/,
                                                      CMenuMpStats* a2, int a3, C_008622e8* a4)
{
    const uint16_t slotId = static_cast<uint16_t>(a3);

    if (slotId == self->field_0020)
    {
        NetCogibleDdl_ReadIntoSlot(reinterpret_cast<void*>(a4), nullptr, reinterpret_cast<int>(&a4));
        C_0085ec18_NullSub(reinterpret_cast<int>(a4));
        return 1;
    }
    else if (slotId == self->field_0022)
    {
        sub_747700(reinterpret_cast<C_00858180*>(a2), nullptr, a4);
        return 1;
    }
    else if (slotId == self->field_0024)
    {
        sub_7478C0(a2, nullptr, a4);
        return 1;
    }
    else if (slotId == self->field_0026)
    {
        sub_7479A0(reinterpret_cast<C_00858180*>(a2), nullptr, a4);
        return 1;
    }

    // Fallthrough: ecx=this, edx=a4; stack pushes a4, a3, then a2 (the [ebp+8] argument).
    return static_cast<char>(SharedDispatch_65E950(
        self,
        reinterpret_cast<void*>(a4),
        reinterpret_cast<int>(a2),
        a3,
        reinterpret_cast<int>(a4)));
}

HOOK(0x00747B60, NetProyectil_HandleMessageSlot, "auto");
