#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, CoverAttack_CheckState_4ADAE0, 0x004ADAE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverAttack_Sub_4C8800, 0x004C8800, (void* self, void* edx, int a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, CoverAttack_Sub_4DC380, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GetStateValue, 0x0050DD30, (void* self, void* edx, int a2));

static uint8_t* GetLinkedBase(int a1)
{
    int value = GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(a1), nullptr);
    if (value)
        return reinterpret_cast<uint8_t*>(value - 4);
    return nullptr;
}

static int __fastcall CoverAttack_ChooseNextAction(void* self, void* /*edx*/, int a1, int a2, int a3)
{
    uint8_t* base = GetLinkedBase(a1);
    if (CSF_AT(int, base, 0x608) != -1)
    {
        base = GetLinkedBase(a1);
        uint8_t* sub = base + 0x5dc;
        uint8_t* arg = base + 0x628;
        if (CSF_AT(int, base, 0x630) < 0)
            arg = base + 0x610;

        if (CoverAttack_CheckState_4ADAE0(sub, nullptr, reinterpret_cast<int>(arg)))
        {
            base = GetLinkedBase(a1);
            if (CSF_AT(int, base, 0x6c8) != 0 &&
                CSF_AT(int, base, 0x6c0) != -1 &&
                CSF_AT(int, base, 0x6c4) != -1)
            {
                int f10 = GetLinkedObjectField10(reinterpret_cast<void*>(a1), nullptr);
                return CoverAttack_Sub_4DC380(reinterpret_cast<void*>(a1), nullptr, f10 + 8);
            }

            int state = GetGlobalState_50D190();
            if (GetStateValue(reinterpret_cast<void*>(state), nullptr, 100) < 30)
                return CoverAttack_Sub_4C8800(self, nullptr, a1);
        }
    }

    int f10 = GetLinkedObjectField10(reinterpret_cast<void*>(a1), nullptr);
    return CoverAttack_Sub_4DC380(reinterpret_cast<void*>(a1), nullptr, f10 + 12);
}

HOOK(0x004C8970, CoverAttack_ChooseNextAction, "auto");
