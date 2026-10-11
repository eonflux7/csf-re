#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void**, __fastcall, sub_555390, 0x00555390, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_740F10, 0x00740F10, (void* self, void* edx, char* Source));

// S_00555390's vtable is referenced by the original's `call 0x555390` site.
// The original performs no vtable store here; this is a file-local binding only.
[[maybe_unused]] static const auto kSpawnStructVtbl = CSF_VTBL(0x00555390);

static char __fastcall MpSpawn_RefreshSelection(S_00555390* self, void* /*edx*/) {
    uint32_t result = self->field_0c9d;

    if (static_cast<uint8_t>(result) == 0) {
        result = static_cast<uint32_t>(GetGlobalValue_8B0568());
        if (CSF_AT(int, reinterpret_cast<void*>(result), 0x5C) == 3) {
            result = self->field_0c8c;
            if (result != 0) {
                result = CSF_AT(uint32_t, reinterpret_cast<void*>(result), 0x18);
                if (result != 0 && result != 1712) {
                    uint32_t spawnEntry = result - 0x6B0;
                    if (spawnEntry != 0) {
                        int globalValue = GetGlobalValue_8B0568();
                        uint32_t nameLength = CSF_AT(uint32_t, reinterpret_cast<void*>(globalValue), 0x94);

                        // Original: v5 = gv + 124; name at v5 + 4 (inline) or *(v5 + 4) (pointer)
                        void* nameBlock = reinterpret_cast<void*>(
                            reinterpret_cast<uint8_t*>(globalValue) + 124);
                        char* displayName;
                        if (nameLength < 0x10) {
                            displayName = reinterpret_cast<char*>(
                                reinterpret_cast<uint8_t*>(nameBlock) + 4);
                        } else {
                            displayName = CSF_AT(char*, nameBlock, 4);
                        }
                        sub_740F10(reinterpret_cast<void*>(spawnEntry), nullptr, displayName);

                        if (self->field_14a3) {
                            self->field_14a3 = 0;
                        } else {
                            sub_555390(self, nullptr);
                        }
                        self->field_0c9d = 1;

                        int globalState = GetGlobalState_50D190();
                        void* stateObj = CSF_AT(void*, reinterpret_cast<void*>(globalState), 0x18);
                        using SlotFn = char(__fastcall*)(void*, void*, int);
                        char notifyResult = reinterpret_cast<SlotFn>(
                            CSF_AT(void**, stateObj, 0)[0x58 / 4])(stateObj, nullptr, 1);
                        result = static_cast<uint8_t>(notifyResult);
                    }
                }
            }
        }
    }

    return static_cast<char>(result);
}

HOOK(0x00555660, MpSpawn_RefreshSelection, "auto");
