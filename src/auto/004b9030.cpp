#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_4B8CD0, 0x004B8CD0, (void* self, void* edx, int a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Looks up the scene handle; if it resolves to an object other than self->field_0000 and that
// object's sub-object reports state 1, compares the tracked target and, when it changed, calls
// S_004ad990_UpdateTrackedTarget and clears the two status bytes.
static uint32_t* __fastcall S_004ad990_RefreshTrackedHandleTarget(S_004ad990* self, void* /*edx*/, int handle) {
    uint32_t* result = reinterpret_cast<uint32_t*>(
        static_cast<uintptr_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle)));
    uint32_t* handleObj = result;

    if (handleObj != nullptr &&
        reinterpret_cast<uintptr_t>(handleObj) != static_cast<uintptr_t>(self->field_0000)) {
        uint32_t child = handleObj[13];
        if (child != 0) {
            void* childPtr = reinterpret_cast<void*>(static_cast<uintptr_t>(child));
            int status = reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, childPtr, 0)[152 / 4])(childPtr, nullptr);
            result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(status));

            if (status == 1) {
                uint32_t v6 = CSF_AT(uint32_t, childPtr, 0x30C);
                uint8_t* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(v6));
                int v7 = CSF_AT(int, base, 0x608);
                uint8_t* targetEntry = base + 0x5DC;
                result = reinterpret_cast<uint32_t*>(targetEntry);

                if (v7 != -1 || CSF_AT(int, targetEntry, 0x30) != -1) {
                    uint8_t* v8 = targetEntry + 0x4C;
                    if (CSF_AT(int, targetEntry, 0x54) < 0) {
                        v8 = targetEntry + 0x34;
                    }
                    uint32_t v8Value = CSF_AT(uint32_t, v8, 4);
                    uint32_t trackedTargetId = CSF_AT(
                        uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0000)), 0x20);

                    if (v8Value != trackedTargetId) {
                        result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(
                            sub_4B8CD0(self, nullptr,
                                       static_cast<int>(reinterpret_cast<uintptr_t>(targetEntry)))));
                        self->field_00a9 = 1;
                        self->field_00a8 = 0;
                    }
                }
            }
        }
    }
    return result;
}

HOOK(0x4B9030, S_004ad990_RefreshTrackedHandleTarget, "auto");
