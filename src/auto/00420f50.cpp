#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTask_EnterBase, 0x004DB440, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(bool, __fastcall, NavManager_TryReserveSpot, 0x0075C5A0, (void* self, void* edx, int a2, float* a3));

// Owner task pointer stored at TNavegando+0x08. Re-read at each use, as the original does.
static uint8_t* TNavegando_OwnerOf(TNavegando* self)
{
    return reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
}

static int __fastcall TNavegando_OnEnterState(TNavegando* self, void* /*edx*/, int a2, int /*a3*/)
{
    // Step 1: call the virtual method at slot 4 (+0x10) on the animation sub-object (owner+0x14 + 4).
    uint8_t* ownerAnim = CSF_AT(uint8_t*, TNavegando_OwnerOf(self), 0x14);
    void* animSub = ownerAnim + 4;
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, animSub, 0)[4])(animSub, nullptr);

    // Step 2: branch on owner's state mode (+0x1c).
    int stateMode = CSF_AT(int, TNavegando_OwnerOf(self), 0x1c);
    if (stateMode == 0)
    {
        CSF_AT(uint8_t, TNavegando_OwnerOf(self), 0x274) = 0;
    }
    else if (stateMode == 1)
    {
        uint8_t* singleton = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
        uint8_t* navManager = CSF_AT(uint8_t*, singleton, 0x1fd8);
        float* targetPos = reinterpret_cast<float*>(TNavegando_OwnerOf(self) + 0x224);
        bool reserved = NavManager_TryReserveSpot(navManager, nullptr, 0, targetPos);
        if (reserved)
        {
            CSF_AT(uint8_t, TNavegando_OwnerOf(self), 0x274) = 0;
        }
    }

    // Step 3: set bit 4 in owner's flags (+0x18).
    CSF_AT(uint32_t, TNavegando_OwnerOf(self), 0x18) |= 4u;

    // Step 4: forward to the base enter routine with (this + 0x0c field + 0x20).
    int baseArg = static_cast<int>(CSF_AT(uint32_t, self, 0x0c) + 0x20);
    return CharTask_EnterBase(self, nullptr, baseArg);
}

HOOK(0x420F50, TNavegando_OnEnterState, "auto");
