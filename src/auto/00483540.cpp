#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_482DC0, 0x00482DC0, (void* self, void* edx, int a2, float a3, char a4));
CSF_GLOBAL(float, g_DegToRad, 0x00833438);

// Picks the aim angle (degrees -> radians) from the global block, unless the
// value block's flag word is -1.0f, then dispatches to the weapon FSM handler.
static int __fastcall CCharActionFsm_DispatchWithAimAngle(CCharActionFsm* self, void* /*edx*/, float a2)
{
    uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_753A30());
    float angleRad = CSF_AT(float, globals, 0x13C) * g_DegToRad;

    uint8_t* value = reinterpret_cast<uint8_t*>(GetValue8B0554());
    int flagWord = CSF_AT(int, value, 0x90C);
    uint8_t* sub = value + 0x54;
    if (flagWord != static_cast<int>(0xBF800000u))
        angleRad = CSF_AT(float, sub, 0x8B8) * g_DegToRad;

    // The callee receives the angle as its raw 32-bit pattern in the first stack slot.
    int angleBits;
    std::memcpy(&angleBits, &angleRad, sizeof(angleBits));
    return sub_482DC0(self, nullptr, angleBits, a2, 0);
}

HOOK(0x483540, CCharActionFsm_DispatchWithAimAngle, "auto");
