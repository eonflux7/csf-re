#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t*, __fastcall, InitStateBlockDefaults, 0x0045BC30, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, RejectStateEventOnControl, 0x0048FE90, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

struct StateBlockScratch {
    uint32_t defaultsHead;
    uint32_t valueA;
    uint32_t valueB;
    uint32_t valueC;
    uint8_t pad10[5];
    uint8_t flag15;
    uint8_t flag16;
};
static_assert(sizeof(StateBlockScratch) == 0x18);

static int __fastcall ApplyStateEventToSceneControl(void* self, void* /*edx*/,
                                                    [[maybe_unused]] int a2,
                                                    [[maybe_unused]] int a3,
                                                    int a4) {
    uint32_t eventBlock = *reinterpret_cast<uint32_t*>(a4);
    uint32_t handleRecordAddr = *reinterpret_cast<uint32_t*>(eventBlock + 4);
    uint8_t* handleRecord = reinterpret_cast<uint8_t*>(handleRecordAddr);

    int result = CSF_AT(int, handleRecord, 0x0C);
    if (result < 0) {
        return result;
    }

    uint32_t handleValue = handleRecordAddr + 4;
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleValue));
    if (!result) {
        return result;
    }

    C_00833908* control = reinterpret_cast<C_00833908*>(result);
    result = static_cast<int>(control->field_0034);
    if (!result) {
        return result;
    }

    if (!control->vGetField016c()) {
        return RejectStateEventOnControl(control, nullptr);
    }

    void* controlSubObject = (void*)control->vGetField016c();
    uint8_t rejectFlag =
        ((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, controlSubObject, 0)[42])(
            controlSubObject, nullptr);
    if (rejectFlag != 0) {
        return RejectStateEventOnControl(control, nullptr);
    }

    StateBlockScratch stateBlock;
    InitStateBlockDefaults(reinterpret_cast<uint32_t*>(&stateBlock), nullptr);

    uint32_t secondRef = *reinterpret_cast<uint32_t*>(a4 + 4);
    uint32_t valueRecordAddr = *reinterpret_cast<uint32_t*>(secondRef + 4) + 4;
    uint32_t* valueWords = reinterpret_cast<uint32_t*>(valueRecordAddr);
    stateBlock.valueA = valueWords[0];
    uint32_t valueB = valueWords[1];
    C_008672e4* stateSink = reinterpret_cast<C_008672e4*>(control->field_0034);
    stateBlock.valueB = valueB;
    stateBlock.valueC = valueWords[2];
    stateBlock.flag15 = 1;
    stateBlock.flag16 = 1;

    result = ((int(__fastcall*)(void*, void*, void*))stateSink->vtbl[42])(
        stateSink, nullptr, &stateBlock);

    uint32_t auxObjectAddr = control->field_0030;
    if (!auxObjectAddr) {
        return result;
    }

    void* auxObject = reinterpret_cast<void*>(auxObjectAddr);
    result = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, auxObject, 0)[47])(
        auxObject, nullptr, &a4);

    if (!(*reinterpret_cast<uint8_t*>(result) & 2)) {
        return result;
    }

    CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleRecord, 0x08);
    CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleRecord, 0x0C);
    CSF_AT(uint8_t, self, 0x82) = 0;
    return reinterpret_cast<int>(self);
}

HOOK(0x0045E480, ApplyStateEventToSceneControl, "auto");
