#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Argument block passed by pointer to the object's virtual slot 42 (+168).
struct SceneCommandPayload {
    int code;
    uint8_t flagA;
    uint8_t flagB;
};

static int __fastcall Scene_ApplyHandleEntry(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    // v4 = *(*a4 + 4); result = *(v4 + 12); v6 = v4 + 4
    uintptr_t v4 = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a4[0]) + 4);
    int result = *reinterpret_cast<int*>(v4 + 12);
    uintptr_t v6 = v4 + 4;

    if (result >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(v6));
        if (result != 0) {
            uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(result)));
            uint32_t v8 = CSF_AT(uint32_t, entry, 0x34);
            if (v8 != 0) {
                if (CSF_AT(int, entry, 0x50) == 0x1C) {
                    // second entry of a4: v9 = a4[1]; v11 = *(*(v9 + 4) + 1)
                    uintptr_t v9 = a4[1];
                    uintptr_t inner = *reinterpret_cast<uint32_t*>(v9 + 4);

                    SceneCommandPayload payload;
                    payload.code = 40;
                    payload.flagA = *reinterpret_cast<uint8_t*>(inner + 1);
                    payload.flagB = 1;

                    void* component = reinterpret_cast<void*>(static_cast<uintptr_t>(v8));
                    auto slot42 = reinterpret_cast<void(__fastcall*)(void*, void*, SceneCommandPayload*)>(
                        CSF_AT(void**, component, 0)[42]);
                    slot42(component, nullptr, &payload);

                    result = *reinterpret_cast<int*>(v6 + 4);
                    CSF_AT(int, self, 0x60) = result;
                    CSF_AT(int, self, 0x64) = *reinterpret_cast<int*>(v6 + 8);
                    CSF_AT(uint8_t, self, 0x82) = 0;
                }
            }
        }
    }

    return result;
}

HOOK(0x0045FE70, Scene_ApplyHandleEntry, "auto");
