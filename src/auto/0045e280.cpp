#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, HandleUnavailableObjectFallback, 0x0048FE90, (void* self, void* edx));

// Resolves a scene object from the handle record in the argument block, builds a 0x18-byte
// state block from the argument records and pushes it into the object's state sink. When the
// flags query on the secondary provider has bit 2 set, copies two dwords from the handle record
// into this+0x60/+0x64 and clears this+0x82.
static int __fastcall ApplyHandleStateUpdate(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4)
{
    // a4[0] -> record P; P[1] -> handle record C.
    uint32_t* argRecord = reinterpret_cast<uint32_t*>(a4[0]);
    uint8_t* handleRecord = reinterpret_cast<uint8_t*>(argRecord[1]);
    int result = *reinterpret_cast<int*>(handleRecord + 12);
    uintptr_t handleAddr = reinterpret_cast<uintptr_t>(handleRecord) + 4;

    if (result < 0)
        return result;

    C_00833908* sceneObject = reinterpret_cast<C_00833908*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleAddr)));
    if (!sceneObject)
        return 0;
    if (!sceneObject->field_0034)
        return 0;

    if (!sceneObject->vGetField016c())
        return HandleUnavailableObjectFallback(sceneObject, nullptr);

    auto stateProvider = sceneObject->vGetField016c();
    void* stateProviderObj = reinterpret_cast<void*>(stateProvider);
    if (((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, stateProviderObj, 0)[42])(stateProviderObj, nullptr))
        return HandleUnavailableObjectFallback(sceneObject, nullptr);

    // Build the state block.
    int stateBlock[6];  // 0x18 bytes
    InitStateBlockDefaults(reinterpret_cast<uint32_t*>(stateBlock), nullptr);

    uint32_t* secondArgRecord = reinterpret_cast<uint32_t*>(a4[1]);
    int* sourceFields = reinterpret_cast<int*>(secondArgRecord[1] + 4);
    stateBlock[1] = sourceFields[0];
    stateBlock[2] = sourceFields[1];
    stateBlock[3] = sourceFields[2];

    uint8_t* flagSourceRecord = reinterpret_cast<uint8_t*>(reinterpret_cast<uint32_t*>(a4[2])[1]);
    reinterpret_cast<uint8_t*>(stateBlock)[21] = flagSourceRecord[1];

    uint32_t* fourthArgRecord = reinterpret_cast<uint32_t*>(reinterpret_cast<uint32_t*>(a4[3])[1]);
    stateBlock[4] = static_cast<int>(fourthArgRecord[1]);

    C_008672e4* stateSink = reinterpret_cast<C_008672e4*>(sceneObject->field_0034);
    result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stateSink, 0)[42])(stateSink, nullptr, stateBlock);

    uint32_t flagsProvider = sceneObject->field_0030;
    if (flagsProvider == 0)
        return result;

    void* flagsProviderObj = reinterpret_cast<void*>(flagsProvider);
    result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, flagsProviderObj, 0)[47])(flagsProviderObj, nullptr, &a4);
    uint8_t* flagsByte = reinterpret_cast<uint8_t*>(result);
    if ((*flagsByte & 2) == 0)
        return result;

    CSF_AT(uint32_t, self, 0x60) = *reinterpret_cast<uint32_t*>(handleAddr + 4);
    CSF_AT(uint32_t, self, 0x64) = *reinterpret_cast<uint32_t*>(handleAddr + 8);
    CSF_AT(uint8_t, self, 0x82) = 0;
    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x0045E280, ApplyHandleStateUpdate, "auto");
