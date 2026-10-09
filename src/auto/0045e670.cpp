#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, FallbackOnIneligibleTarget, 0x0048FE90, (void* self, void* edx));

static int __fastcall ApplyHandleRecordToTarget(void* self, void* /*edx*/, int unusedArg2, int unusedArg3, uint32_t* updateArgs)
{
    // updateArgs[0] -> header whose [1] is the address of the source record
    uint32_t* headerRef = reinterpret_cast<uint32_t*>(updateArgs[0]);
    uint32_t recordAddr = headerRef[1];
    uint32_t* sourceRecord = reinterpret_cast<uint32_t*>(recordAddr);

    int result = static_cast<int>(sourceRecord[3]);
    if (result < 0) {
        return result;
    }

    // The handle key is the address of the record's +4 field; its +4 and +8 are read back later.
    uint32_t handleAddr = recordAddr + 4;
    uint32_t* handleFields = reinterpret_cast<uint32_t*>(handleAddr);

    int resolvedObject = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddr));
    if (resolvedObject == 0) {
        return 0;
    }

    C_00833908* target = reinterpret_cast<C_00833908*>(resolvedObject);
    if (target->field_0034 == 0) {
        return 0;
    }

    if (!target->vGetField016c()) {
        return FallbackOnIneligibleTarget(target, nullptr);
    }

    void* stateOwner = reinterpret_cast<void*>(target->vGetField016c());
    uint8_t stateOwnerBlocked = ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, stateOwner, 0)[42])(stateOwner, nullptr);
    if (stateOwnerBlocked != 0) {
        return FallbackOnIneligibleTarget(target, nullptr);
    }

    alignas(4) uint8_t stateBlock[0x17];
    InitStateBlockDefaults(reinterpret_cast<uint32_t*>(stateBlock), nullptr);

    // Vector taken from updateArgs[1] -> header[1] -> +4 block
    uint32_t* headerWithVector = reinterpret_cast<uint32_t*>(updateArgs[1]);
    uint32_t* vectorRecord = reinterpret_cast<uint32_t*>(headerWithVector[1] + 4);
    CSF_AT(uint32_t, stateBlock, 0x04) = vectorRecord[0];
    CSF_AT(uint32_t, stateBlock, 0x08) = vectorRecord[1];
    CSF_AT(uint32_t, stateBlock, 0x0C) = vectorRecord[2];

    // Scalar taken from updateArgs[2] -> [1] -> [1]
    uint32_t* secondaryRef = reinterpret_cast<uint32_t*>(updateArgs[2]);
    uint32_t* secondaryRecord = reinterpret_cast<uint32_t*>(secondaryRef[1]);
    CSF_AT(uint32_t, stateBlock, 0x10) = secondaryRecord[1];
    CSF_AT(uint8_t, stateBlock, 0x14) = 1;
    CSF_AT(uint8_t, stateBlock, 0x15) = 1;
    CSF_AT(uint8_t, stateBlock, 0x16) = 1;

    C_008672e4* stateSink = reinterpret_cast<C_008672e4*>(target->field_0034);
    result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stateSink, 0)[42])(stateSink, nullptr, stateBlock);

    uint32_t sceneNodeValue = target->field_0030;
    if (sceneNodeValue != 0) {
        void* sceneNode = reinterpret_cast<void*>(sceneNodeValue);
        result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, sceneNode, 0)[47])(sceneNode, nullptr, &updateArgs);

        uint8_t* flagBits = reinterpret_cast<uint8_t*>(result);
        if ((*flagBits & 2) != 0) {
            result = static_cast<int>(reinterpret_cast<uintptr_t>(self));
            CSF_AT(uint32_t, self, 0x60) = handleFields[1];
            CSF_AT(uint32_t, self, 0x64) = handleFields[2];
            CSF_AT(uint8_t, self, 0x82) = 0;
        }
    }

    return result;
}

HOOK(0x0045E670, ApplyHandleRecordToTarget, "auto");
