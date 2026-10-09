#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __stdcall, GuiControl_QueryIntValue, 0x004E9890, (uint32_t* a1));
CSF_FUNC(double, __stdcall, GuiControl_QueryFloatValue, 0x004E98C0, (uint32_t* a1));

// Parameter block passed by address to the child object's virtual method (vtable slot 42, +0xA8).
struct SceneParamBlock {
    int32_t kind;          // +0x00  always 14
    float queryFloat;      // +0x04  GuiControl_QueryFloatValue(control)
    int32_t firstValue;    // +0x08  a3[1] -> record+4 -> node+4
    int32_t secondValue;   // +0x0C  a3[2] -> record+4 -> node+4
    int32_t thresholdInt;  // +0x10  (int)(float) of a3[3] -> record+4 -> node+4
    int32_t queryInt;      // +0x14  GuiControl_QueryIntValue(control)
};
static_assert(sizeof(SceneParamBlock) == 0x18);

static int __stdcall SceneHandle_ForwardParamsToChild(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    // a3[0] -> record; record+4 -> node; node+12 -> signed rank.
    uint8_t* firstRecord = reinterpret_cast<uint8_t*>(a3[0]);
    int32_t nodeValue = CSF_AT(int32_t, firstRecord, 4);
    int32_t nodeRank = CSF_AT(int32_t, reinterpret_cast<uint8_t*>(nodeValue), 12);

    int result = nodeValue + 4;
    if (nodeRank < 0) {
        return result;
    }

    int control = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
    result = control;
    if (control == 0) {
        return result;
    }

    uint8_t* controlObject = reinterpret_cast<uint8_t*>(control);
    result = CSF_AT(int32_t, controlObject, 0x34);
    if (result == 0) {
        return result;
    }

    SceneParamBlock block{};
    block.kind = 14;

    // a3[1] -> record; record+4 -> node; node+4 -> value
    uint8_t* secondArgRecord = reinterpret_cast<uint8_t*>(a3[1]);
    uint8_t* secondArgNode = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, secondArgRecord, 4));
    block.firstValue = CSF_AT(int32_t, secondArgNode, 4);

    // a3[2] -> record; record+4 -> node; node+4 -> value
    uint8_t* thirdArgRecord = reinterpret_cast<uint8_t*>(a3[2]);
    uint8_t* thirdArgNode = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, thirdArgRecord, 4));
    block.secondValue = CSF_AT(int32_t, thirdArgNode, 4);

    // a3[3] -> record; record+4 -> node; node+4 -> float
    uint8_t* fourthArgRecord = reinterpret_cast<uint8_t*>(a3[3]);
    uint8_t* fourthArgNode = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, fourthArgRecord, 4));
    float scaleArg = CSF_AT(float, fourthArgNode, 4);
    block.thresholdInt = static_cast<int32_t>(static_cast<int64_t>(scaleArg));

    GetValue8B0554();
    block.queryInt = GuiControl_QueryIntValue(reinterpret_cast<uint32_t*>(control));

    GetValue8B0554();
    block.queryFloat = static_cast<float>(GuiControl_QueryFloatValue(reinterpret_cast<uint32_t*>(control)));

    // The child pointer is reloaded after the calls above, as the original does.
    uint8_t* childObject = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, controlObject, 0x34));
    return ((int(__fastcall*)(void*, void*, SceneParamBlock*))CSF_AT(void**, childObject, 0)[42])(childObject, nullptr, &block);
}

HOOK(0x0045E8D0, SceneHandle_ForwardParamsToChild, "auto");
