#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, GetSceneEntryIntAt0C, 0x004E9890, (uint32_t* a1));
CSF_FUNC(int, __stdcall, GetSceneObjectIntAt10, 0x004E9860, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt0C, 0x004E9830, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GetSceneObjectFloatAt08, 0x004E9800, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GuiControl_QueryFloatValue, 0x004E98C0, (uint32_t* a1));
CSF_FUNC(unsigned int, __fastcall, AttackTasks_QueueShot, 0x0048C630,
         (void* self, void* edx, int a2, float a3, float a4, int a5, int a6));

// Target 0x47BF30 (__thiscall, ret 0). Tail-jumps into 0x47BBF0 whose body is inlined here.
static int __fastcall AttackTasks_FireAtSceneTarget(void* self, void* /*edx*/)
{
    SceneLoader_NoOp();

    uint32_t sceneObject = CSF_AT(uint32_t, self, 0x14);

    GetValue8B0554();
    int sceneEntryIntAt0C = GetSceneEntryIntAt0C(reinterpret_cast<uint32_t*>(sceneObject));

    GetValue8B0554();
    int sceneObjectIntAt10 = GetSceneObjectIntAt10(reinterpret_cast<uint32_t*>(sceneObject));

    GetValue8B0554();
    float sceneObjectFloatAt0C =
        static_cast<float>(GetSceneObjectFloatAt0C(reinterpret_cast<uint32_t*>(sceneObject)));

    GetValue8B0554();
    float sceneObjectFloatAt08 =
        static_cast<float>(GetSceneObjectFloatAt08(reinterpret_cast<uint32_t*>(sceneObject)));

    GetValue8B0554();
    float guiControlValue =
        static_cast<float>(GuiControl_QueryFloatValue(reinterpret_cast<uint32_t*>(sceneObject)));

    // The original passes the float result as raw bits in a stack int slot.
    int guiControlBits = 0;
    std::memcpy(&guiControlBits, &guiControlValue, sizeof(guiControlBits));

    return static_cast<int>(AttackTasks_QueueShot(
        static_cast<uint8_t*>(self) + 0x2B8, nullptr,
        guiControlBits, sceneObjectFloatAt08, sceneObjectFloatAt0C,
        sceneObjectIntAt10, sceneEntryIntAt0C));
}

HOOK(0x0047BF30, AttackTasks_FireAtSceneTarget, "auto");
