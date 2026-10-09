#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Request block built on the stack by the original: 0x24 bytes, vtable pointer first.
struct SceneHandleRequest {
    void* vtbl;        // +0x00  C_0082d448 vtable
    int32_t field04;   // +0x04  -1
    int32_t field08;   // +0x08  -1
    uint8_t field0C;   // +0x0C  0
    uint8_t pad0D[3];
    int32_t field10;   // +0x10  0
    int32_t field14;   // +0x14  0
    int32_t field18;   // +0x18  0
    float field1C;     // +0x1C  -1.0f
    int32_t field20;   // +0x20  6 - (flag != 0)
};

static int __stdcall DispatchHandleRequestToScene(int /*a1*/, int /*a2*/, uint32_t* a3)
{
    uint32_t v3 = *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a3[0]) + 4);
    int32_t v4 = *reinterpret_cast<int32_t*>(static_cast<uintptr_t>(v3) + 12);
    int result = static_cast<int>(v3 + 4);

    if (v4 >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result) {
            uintptr_t resolved = static_cast<uintptr_t>(result);
            if (*reinterpret_cast<int32_t*>(resolved + 80) == 26) {
                uint32_t v6 = *reinterpret_cast<uint32_t*>(resolved + 52);
                uintptr_t v7 = a3[1];
                uintptr_t flagOwner = static_cast<uintptr_t>(*reinterpret_cast<uint32_t*>(v7 + 4));
                uint8_t flagByte = *reinterpret_cast<uint8_t*>(flagOwner + 1);

                SceneHandleRequest request{};
                request.vtbl = CSF_VTBL(0x0082D448);
                request.field04 = -1;
                request.field08 = -1;
                request.field0C = 0;
                request.field10 = 0;
                request.field14 = 0;
                request.field18 = 0;
                request.field1C = -1.0f;
                request.field20 = 6 - (flagByte != 0 ? 1 : 0);

                void* target = reinterpret_cast<void*>(static_cast<uintptr_t>(v6));
                return reinterpret_cast<int(__fastcall*)(void*, void*, SceneHandleRequest*)>(
                    CSF_AT(void**, target, 0)[17])(target, nullptr, &request);
            }
        }
    }
    return result;
}

HOOK(0x463F10, DispatchHandleRequestToScene, "auto");
