#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, RejectCommandOnSceneObject, 0x0048FE90, (void* self, void* edx));

static int __fastcall ApplyPositionRecordToSceneObject(void* self, void* /*edx*/, int /*unusedA2*/, int /*unusedA3*/, int a4)
{
    void* const parameterBlock = reinterpret_cast<void*>(a4);
    void* const message = CSF_AT(void*, parameterBlock, 0);
    int recordValue = CSF_AT(int, message, 4);
    void* const record = reinterpret_cast<void*>(recordValue);

    int result = CSF_AT(int, record, 12);
    if (result < 0) {
        return result;
    }

    int handleValue = recordValue + 4;
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, handleValue);
    C_00833908* const sceneObject = reinterpret_cast<C_00833908*>(result);
    if (sceneObject != nullptr) {
        result = static_cast<int>(sceneObject->field_0034);
        if (result != 0) {
            if (!sceneObject->vGetField016c()) {
                return RejectCommandOnSceneObject(sceneObject, nullptr);
            }

            int precheckValue = sceneObject->vGetField016c();
            void* const precheckObject = reinterpret_cast<void*>(precheckValue);
            if (reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(CSF_AT(void**, precheckObject, 0)[42])(precheckObject, nullptr)) {
                return RejectCommandOnSceneObject(sceneObject, nullptr);
            }

            uint32_t stateBlock[6];
            InitStateBlockDefaults(stateBlock, nullptr);

            void* const payloadHolder = CSF_AT(void*, parameterBlock, 4);
            void* const payload = CSF_AT(void*, payloadHolder, 4);
            CSF_AT(int, stateBlock, 4) = CSF_AT(int, payload, 4);
            int const secondPayloadValue = CSF_AT(int, payload, 8);
            void* const senderHolder = CSF_AT(void*, parameterBlock, 8);
            CSF_AT(int, stateBlock, 8) = secondPayloadValue;
            CSF_AT(int, stateBlock, 12) = CSF_AT(int, payload, 12);

            void* const senderPayload = CSF_AT(void*, senderHolder, 4);
            uint8_t const senderValue = CSF_AT(uint8_t, senderPayload, 1);
            C_008672e4* const stateHandler = reinterpret_cast<C_008672e4*>(sceneObject->field_0034);
            CSF_AT(uint8_t, stateBlock, 0x15) = senderValue;
            result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(stateHandler->vtbl[42])(stateHandler, nullptr, stateBlock);

            uint32_t commandTarget = sceneObject->field_0030;
            if (commandTarget != 0) {
                void* const flagsOwner = reinterpret_cast<void*>(commandTarget);
                result = reinterpret_cast<int(__fastcall*)(void*, void*, int*)>(CSF_AT(void**, flagsOwner, 0)[47])(flagsOwner, nullptr, &a4);
                uint8_t* const flagsByte = reinterpret_cast<uint8_t*>(result);
                if ((*flagsByte & 2) != 0) {
                    int const firstRecordValue = CSF_AT(int, record, 8);
                    result = reinterpret_cast<int>(self);
                    CSF_AT(int, self, 0x60) = firstRecordValue;
                    CSF_AT(int, self, 0x64) = CSF_AT(int, record, 12);
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
            }
        }
    }
    return result;
}

HOOK(0x0045E190, ApplyPositionRecordToSceneObject, "auto");
