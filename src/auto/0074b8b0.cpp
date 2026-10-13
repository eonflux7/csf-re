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

CSF_FUNC(void*, __fastcall, NetPlayerDDL_ResolveObject, 0x00655DF0, (void* self, void* edx, bool* a2));
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SharedDispatch_65E950, 0x0065E950, (void* self, void* edx, int a1, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_73BD20, 0x0073BD20, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_73BD60, 0x0073BD60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_73BE00, 0x0073BE00, (void* self, void* edx, char a2));

// Dispatches a network slot message on NetGhost by 16-bit message id.
// Ids stored at +0x20, +0x22, +0x24 select a handler; anything else goes to SharedDispatch.
static int __fastcall NetGhost_HandleSlotMessage(NetGhost* self, void* /*edx*/, std::uint32_t* a2, int a3, C_008622e8* a4)
{
    if (static_cast<uint16_t>(a3) == CSF_AT(uint16_t, self, 0x20))
    {
        NetCogibleDdl_ReadIntoSlot(a4, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&a4)));
        sub_73BD60(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a4)));
        return 1;
    }
    if (static_cast<uint16_t>(a3) == CSF_AT(uint16_t, self, 0x22))
    {
        NetCogibleDdl_ReadIntoSlot(a4, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(&a4)));
        sub_73BD20(a2, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(a4)));
        return 1;
    }
    if (static_cast<uint16_t>(a3) == CSF_AT(uint16_t, self, 0x24))
    {
        NetPlayerDDL_ResolveObject(a4, nullptr, reinterpret_cast<bool*>(&a4));
        sub_73BE00(a2, nullptr, static_cast<char>(reinterpret_cast<uintptr_t>(a4)));
        return 1;
    }
    return SharedDispatch_65E950(self, nullptr, reinterpret_cast<int>(a2), a3,
                                 static_cast<int>(reinterpret_cast<uintptr_t>(a4)));
}

HOOK(0x0074B8B0, NetGhost_HandleSlotMessage, "auto");
