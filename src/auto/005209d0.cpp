#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

typedef void* (__fastcall* VCall0Fn)(void* self, void* edx);
typedef void* (__fastcall* VCall1Fn)(void* self, void* edx, int arg);
typedef void  (__fastcall* VCallVoid1Fn)(void* self, void* edx, void* arg);

// Re-binds each stored 36-byte handle entry's target to the resolved object's transform/bone data.
static int __fastcall ParticleManager_UpdateAttachedEmitters(void* self, void* /*edx*/, int /*unused*/)
{
    alignas(8) uint8_t handleBytes[0x20];
    uint8_t* handle = handleBytes;

    uint32_t count = CSF_AT(uint32_t, self, 0x60);
    uint32_t offset = 0;

    CSF_AT(void*, handle, 0x00) = CSF_VTBL(0x0082D448);
    CSF_AT(int, handle, 0x04) = -1;
    CSF_AT(int, handle, 0x08) = -1;
    CSF_AT(int, handle, 0x0C) = -1;
    CSF_AT(int, handle, 0x10) = 0;
    CSF_AT(int, handle, 0x14) = 0;
    CSF_AT(int, handle, 0x18) = -1;
    CSF_AT(int, handle, 0x1C) = -1;

    if (count == 0)
        return 0;

    uint32_t remaining = count;
    do {
        uint8_t* entry = CSF_AT(uint8_t*, self, 0x58) + offset;
        CSF_AT(int, handle, 0x04) = CSF_AT(int, entry, 0x08);
        CSF_AT(int, handle, 0x08) = CSF_AT(int, entry, 0x0C);
        CSF_AT(int, handle, 0x0C) = CSF_AT(int, entry, 0x10);
        CSF_AT(int, handle, 0x10) = CSF_AT(int, entry, 0x14);
        CSF_AT(int, handle, 0x14) = CSF_AT(int, entry, 0x18);
        CSF_AT(int, handle, 0x18) = CSF_AT(int, entry, 0x1C);
        CSF_AT(int, handle, 0x1C) = CSF_AT(int, entry, 0x20);

        void* obj = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)(intptr_t)handle));

        if (obj) {
            if (CSF_AT(int, handle, 0x0C) == -1) {
                void* a = ((VCall0Fn)CSF_AT(void**, obj, 0)[0x28 / 4])(obj, nullptr);
                void* target = CSF_AT(void*, handle, 0x10);
                void* targetVtbl = CSF_AT(void*, target, 0);
                void* b = ((VCall0Fn)CSF_AT(void**, a, 0)[0x18 / 4])(a, nullptr);
                ((VCallVoid1Fn)(reinterpret_cast<void**>(targetVtbl))[0x44 / 4])(target, nullptr, b);
            } else {
                void* a = ((VCall0Fn)CSF_AT(void**, obj, 0)[0x28 / 4])(obj, nullptr);
                int idx = CSF_AT(int, handle, 0x0C);
                void* b = ((VCall1Fn)CSF_AT(void**, a, 0)[0x98 / 4])(a, nullptr, idx);
                void* c = ((VCall0Fn)CSF_AT(void**, obj, 0)[0x28 / 4])(obj, nullptr);
                void* d = ((VCall1Fn)CSF_AT(void**, c, 0)[0x8C / 4])(c, nullptr, (int)(intptr_t)b);
                void* target = CSF_AT(void*, handle, 0x10);
                void* targetVtbl = CSF_AT(void*, target, 0);
                void* e = ((VCall0Fn)CSF_AT(void**, d, 0)[0x18 / 4])(d, nullptr);
                ((VCallVoid1Fn)(reinterpret_cast<void**>(targetVtbl))[0x44 / 4])(target, nullptr, e);
            }
        }

        offset += 0x24;
        --remaining;
    } while (remaining != 0);

    return 0;
}

HOOK(0x005209D0, ParticleManager_UpdateAttachedEmitters, "auto");
