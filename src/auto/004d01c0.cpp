#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, Autonomo_DispatchAction, 0x004D1160, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, TCharTask_UpdateSubObject, 0x0048C680, (void* self, void* edx, float a2));
CSF_FUNC(char, __fastcall, AiAttackTasks_UpdateAlternatingTimers, 0x0047AED0, (float* self, void* edx, float elapsed));

static char __fastcall C_0083b668_UpdateBase(C_0083b668* self, void* /*edx*/, float a2)
{
    char result = self->field_0038;
    if (result)
    {
        // The original forwards the raw dword of the float argument (SLODWORD(a2)).
        int32_t a2Bits;
        std::memcpy(&a2Bits, &a2, sizeof(a2Bits));

        Autonomo_DispatchAction(self, nullptr, a2Bits);
        TCharTask_UpdateSubObject(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x40), nullptr, a2);
        return AiAttackTasks_UpdateAlternatingTimers(
            reinterpret_cast<float*>(reinterpret_cast<uint8_t*>(self) + 0x60), nullptr, a2);
    }
    return result;
}

HOOK(0x004D01C0, C_0083b668_UpdateBase, "auto");
