#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Updates a state record (a2) for a water state node: a2+0x00 holds the "done/finished" flag,
// a2+0x04 the owner object, a2+0x1C a saved word. Returns 1 when the state stays active, 0 when it finishes.
static int __fastcall Water_StateNodeUpdate(void* self, void* /*edx*/, int a2) {
    using ByteQuery = uint8_t(__fastcall*)(void* obj, void* edx, void* arg0, void* arg1);
    using StartCall = int(__fastcall*)(void* obj, void* edx, void* arg0, void* arg1);
    using FinishQuery = uint8_t(__fastcall*)(void* obj, void* edx, void* arg0);

    uint8_t* state = reinterpret_cast<uint8_t*>(static_cast<std::uintptr_t>(a2));

    // v2 = !state->done && !owner->vtbl[0x80/4](owner, 0, state)
    uint8_t activeFlag = 0;
    if (CSF_AT(uint8_t, state, 0x00) == 0) {
        void* owner = CSF_AT(void*, state, 0x04);
        auto queryFn = reinterpret_cast<ByteQuery>(CSF_AT(void**, owner, 0)[0x80 / 4]);
        uint8_t queryResult = queryFn(owner, nullptr, nullptr, state);
        activeFlag = (queryResult == 0) ? 1 : 0;
    }

    int savedWord = CSF_AT(int, state, 0x1C);
    CSF_AT(uint8_t, state, 0x00) = activeFlag;
    CSF_AT(int, state, 0x1C) = 0;

    if (activeFlag == 0) {
        void* owner = CSF_AT(void*, state, 0x04);
        CSF_AT(uint8_t, state, 0x15) = 0;
        CSF_AT(uint8_t, state, 0x16) = 0;
        void* ownerThisPart = reinterpret_cast<uint8_t*>(self) + 0x04;
        auto startFn = reinterpret_cast<StartCall>(CSF_AT(void**, owner, 0)[0x30 / 4]);
        int startResult = startFn(owner, nullptr, ownerThisPart, state);
        CSF_AT(uint8_t, state, 0x00) = (startResult == -1) ? 1 : 0;
    }

    bool stillZero = CSF_AT(uint8_t, state, 0x00) == 0;
    CSF_AT(int, state, 0x1C) = savedWord;

    if (stillZero) {
        void* owner = CSF_AT(void*, state, 0x04);
        auto finishFn = reinterpret_cast<FinishQuery>(CSF_AT(void**, owner, 0)[0x7C / 4]);
        if (finishFn(owner, nullptr, state)) {
            CSF_AT(uint8_t, state, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x0070C990, Water_StateNodeUpdate, "auto");
