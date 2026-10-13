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

CSF_FUNC(int, __fastcall, NetCallbacks_ForwardApplyDefaultDescriptor, 0x007435B0, (void* self, void*));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, NetCallbacks_ForwardToChildAtD4, 0x00743590, (void* self, void*, int a2));
CSF_FUNC(int, __fastcall, NetCallbacks_ProcessDescriptorArg, 0x0074FBE0, (void* self, void* edx, void* arg0));
CSF_FUNC(int, __fastcall, SharedDispatch_65E950, 0x0065E950, (void* self, void* edx, int a1, int a2, int a3));

static char __fastcall NetPostman_DispatchByCode(NetPostman* self, void* /*edx*/, S_00743570* a2, int a3, C_008622e8* a4)
{
    const uint16_t code = static_cast<uint16_t>(a3);

    if (code == self->field_0020)
    {
        NetCallbacks_ForwardApplyDefaultDescriptor(a2, nullptr);
        return 1;
    }
    else if (code == self->field_0022)
    {
        NetCogibleDdl_ReadIntoSlot(a4, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&a4)));
        NetCallbacks_ForwardToChildAtD4(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a4)));
        return 1;
    }
    else if (code == self->field_0024)
    {
        NetCallbacks_ProcessDescriptorArg(a2, nullptr, a4);
        return 1;
    }
    else
    {
        const int result = SharedDispatch_65E950(self, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(a2)),
            a3,
            static_cast<int>(reinterpret_cast<uintptr_t>(a4)));
        return static_cast<char>(result);
    }
}

HOOK(0x74FDD0, NetPostman_DispatchByCode, "auto");
