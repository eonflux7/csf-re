#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, SceneObject_FallbackHandler, 0x0048FE90, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the scene object named by the handle record reached from the message argument (a4).
// The object's provider can reject the request, in which case the fallback handler runs.
// Otherwise a state block is built from the message and handed to the object. When the
// secondary target reports the accepted flag, two dwords of the handle record are copied
// into self and self is returned.
static int __fastcall ApplyHandleStateToSceneObject(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, int a4)
{
    // a4 is the address of a message record: [record] -> link, link + 4 -> handle record.
    const uint32_t messageRecordAddr = static_cast<uint32_t>(a4);
    const uint32_t handleRecordAddr = *reinterpret_cast<uint32_t*>(*reinterpret_cast<uint32_t*>(messageRecordAddr) + 4);
    const uint32_t handleFieldAddr = handleRecordAddr + 4;

    int result = *reinterpret_cast<int*>(handleRecordAddr + 12);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleFieldAddr));
    C_00833908* sceneObject = reinterpret_cast<C_00833908*>(result);
    if (sceneObject == nullptr)
        return 0;

    if (sceneObject->field_0034 == 0)
        return 0;

    if (!sceneObject->vGetField016c())
        return SceneObject_FallbackHandler(sceneObject, nullptr);

    void* providerObject = reinterpret_cast<void*>(sceneObject->vGetField016c());
    uint8_t providerRejectsState =
        ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, providerObject, 0)[42])(providerObject, nullptr);
    if (providerRejectsState)
        return SceneObject_FallbackHandler(sceneObject, nullptr);

    alignas(4) uint8_t stateBlock[0x17];
    InitStateBlockDefaults(reinterpret_cast<uint32_t*>(stateBlock), nullptr);

    const uint32_t messageLinkAddr = *reinterpret_cast<uint32_t*>(messageRecordAddr + 4);
    const uint32_t valueRecordAddr = *reinterpret_cast<uint32_t*>(messageLinkAddr + 4) + 4;
    CSF_AT(uint32_t, stateBlock, 4) = *reinterpret_cast<uint32_t*>(valueRecordAddr);
    CSF_AT(uint32_t, stateBlock, 8) = *reinterpret_cast<uint32_t*>(valueRecordAddr + 4);
    CSF_AT(uint32_t, stateBlock, 0xC) = *reinterpret_cast<uint32_t*>(valueRecordAddr + 8);

    const uint32_t dataRecordAddr = *reinterpret_cast<uint32_t*>(messageRecordAddr + 8);
    const uint32_t dataLinkAddr = *reinterpret_cast<uint32_t*>(dataRecordAddr + 4);
    CSF_AT(uint32_t, stateBlock, 0x10) = *reinterpret_cast<uint32_t*>(dataLinkAddr + 4);

    CSF_AT(uint8_t, stateBlock, 0x15) = 1;
    CSF_AT(uint8_t, stateBlock, 0x16) = 1;

    // The state target is re-read here, after the calls above, as in the original.
    void* stateTarget = reinterpret_cast<void*>(sceneObject->field_0034);
    result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stateTarget, 0)[42])(stateTarget, nullptr, stateBlock);

    const uint32_t secondaryTargetAddr = sceneObject->field_0030;
    if (secondaryTargetAddr == 0)
        return result;

    void* secondaryTarget = reinterpret_cast<void*>(secondaryTargetAddr);
    result = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, secondaryTarget, 0)[47])(secondaryTarget, nullptr, &a4);

    const uint8_t acceptedFlags = *reinterpret_cast<uint8_t*>(result);
    if ((acceptedFlags & 2) == 0)
        return result;

    CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(handleFieldAddr + 4);
    CSF_AT(uint32_t, self, 0x64) = *reinterpret_cast<uint32_t*>(handleFieldAddr + 8);
    CSF_AT(uint8_t, self, 0x82) = 0;
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0045E570, ApplyHandleStateToSceneObject, "auto");
