#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, RejectObjectOrder, 0x0048FE90, (void* self, void* edx));

struct MoveCommandBlock {
    uint32_t header;
    uint32_t positionX;
    uint32_t positionY;
    uint32_t positionZ;
    uint32_t parameter;
    uint8_t enableFlag;
    uint8_t modeFlag;
    uint8_t padding[6];
};
static_assert(sizeof(MoveCommandBlock) == 0x1C);

static int __fastcall IssueMoveOrderFromRequest(
    void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    uint32_t requestRecord = reinterpret_cast<uint32_t*>(a4[0])[1];
    int requestCode = *reinterpret_cast<int*>(requestRecord + 12);
    uint32_t handleKey = requestRecord + 4;
    if (requestCode < 0)
        return requestCode;

    C_00833908* sceneObject = reinterpret_cast<C_00833908*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleKey)));
    if (sceneObject == nullptr)
        return 0;
    if (!sceneObject->field_0034)
        return 0;

    if (!sceneObject->vGetField016c())
        return RejectObjectOrder(sceneObject, nullptr);

    void* stateObject = reinterpret_cast<void*>(sceneObject->vGetField016c());
    if (((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, stateObject, 0)[42])(
            stateObject, nullptr))
        return RejectObjectOrder(sceneObject, nullptr);

    MoveCommandBlock commandBlock;
    InitStateBlockDefaults(reinterpret_cast<uint32_t*>(&commandBlock), nullptr);

    int* positionRecord = reinterpret_cast<int*>(
        reinterpret_cast<uint32_t*>(a4[1])[1] + 4);
    commandBlock.positionX = static_cast<uint32_t>(positionRecord[0]);
    commandBlock.positionY = static_cast<uint32_t>(positionRecord[1]);
    commandBlock.positionZ = static_cast<uint32_t>(positionRecord[2]);

    commandBlock.modeFlag =
        *(reinterpret_cast<uint8_t*>(reinterpret_cast<uint32_t*>(a4[2])[1]) + 1);
    commandBlock.parameter =
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(a4[3])[1])[1];
    commandBlock.enableFlag = 1;

    void* behaviorObject = reinterpret_cast<void*>(sceneObject->field_0034);
    int behaviorResult =
        ((int(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, behaviorObject, 0)[42])(
            behaviorObject, nullptr, reinterpret_cast<uint32_t*>(&commandBlock));

    uint32_t subObject = sceneObject->field_0030;
    if (subObject == 0)
        return behaviorResult;

    uint8_t* statusFlags =
        ((uint8_t*(__fastcall*)(void*, void*, uint32_t**))
            CSF_AT(void**, reinterpret_cast<void*>(subObject), 0)[47])(
                reinterpret_cast<void*>(subObject), nullptr, &a4);
    if (*statusFlags & 2)
    {
        CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(handleKey + 4);
        CSF_AT(uint32_t, self, 0x64) = *reinterpret_cast<uint32_t*>(handleKey + 8);
        CSF_AT(uint8_t, self, 0x82) = 0;
        return reinterpret_cast<int>(self);
    }
    return reinterpret_cast<int>(statusFlags);
}

HOOK(0x0045E380, IssueMoveOrderFromRequest, "auto");
