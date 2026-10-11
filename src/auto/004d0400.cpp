#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, PrepareAttackState, 0x004D1700, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, GetSceneEntryIntAt0C, 0x004E9890, (uint32_t* a1));
CSF_FUNC(int, __stdcall, GetSceneObjectIntAt10, 0x004E9860, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt0C, 0x004E9830, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt08, 0x004E9800, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GuiControl_QueryFloatValue, 0x004E98C0, (uint32_t* a1));
CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630,
         (void* self, void* edx, int a2, float a3, float a4, int a5, int a6));

// Target 0x4D0400 (__thiscall, ret 0). Calls PrepareAttackState, then tail-jumps
// into 0x4D02E0 whose body is inlined here. Each read of the scene object
// (field 0x14) is repeated as in the original.
static unsigned int __fastcall C_0083b668_FireShotAtSceneTarget(C_0083b668* self, void* /*edx*/)
{
    PrepareAttackState(self, nullptr);

    GetValue8B0554();
    int sceneEntryIntAt0C = GetSceneEntryIntAt0C(reinterpret_cast<uint32_t*>(self->field_0014));

    GetValue8B0554();
    int sceneObjectIntAt10 = GetSceneObjectIntAt10(reinterpret_cast<uint32_t*>(self->field_0014));

    GetValue8B0554();
    float sceneObjectFloatAt0C =
        static_cast<float>(GetSceneObjectFloatAt0C(reinterpret_cast<uint32_t*>(self->field_0014)));

    GetValue8B0554();
    float sceneObjectFloatAt08 =
        static_cast<float>(GetSceneObjectFloatAt08(reinterpret_cast<uint32_t*>(self->field_0014)));

    GetValue8B0554();
    float guiControlValue =
        static_cast<float>(GuiControl_QueryFloatValue(reinterpret_cast<uint32_t*>(self->field_0014)));

    // The original passes the float result as raw bits in a stack int slot.
    int guiControlBits = 0;
    std::memcpy(&guiControlBits, &guiControlValue, sizeof(guiControlBits));

    return AttackTasks_QueueShot(
        reinterpret_cast<uint8_t*>(self) + 0x40, nullptr,
        guiControlBits, sceneObjectFloatAt08, sceneObjectFloatAt0C,
        sceneObjectIntAt10, sceneEntryIntAt0C);
}

HOOK(0x004D0400, C_0083b668_FireShotAtSceneTarget, "auto");
