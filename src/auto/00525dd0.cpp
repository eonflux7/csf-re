#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));

static int __stdcall MenuMissionInfo_SetGlobalIndex(int a1) {
    uint8_t* gvBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    void* sceneLoader = gvBase + 0x22A4;

    int result = SceneLoader_GetResourceById(sceneLoader, nullptr, a1);
    if (result != -1) {
        // Stack request object (0x38 bytes), filled field by field at its original offsets.
        uint8_t requestBuffer[0x38];
        uint8_t* req = requestBuffer;

        CSF_AT(uint8_t, req, 0x35) = 1;   // v17
        CSF_AT(uint8_t, req, 0x36) = 1;   // v18
        CSF_AT(int, req, 0x18) = 0;       // v9
        CSF_AT(int, req, 0x1C) = 0;       // v10
        CSF_AT(int, req, 0x20) = 0;       // v11
        CSF_AT(void*, req, 0x24) = CSF_VTBL(0x0082D448);  // v12
        CSF_AT(int, req, 0x28) = -1;      // v13
        CSF_AT(int, req, 0x2C) = -1;      // v14
        CSF_AT(int, req, 0x14) = 0;       // v8
        CSF_AT(uint8_t, req, 0x34) = 0;   // v16
        CSF_AT(int, req, 0x0C) = -1;      // v6
        CSF_AT(int, req, 0x30) = 0;       // v15
        CSF_AT(int, req, 0x00) = 0;       // a2.field_0000
        CSF_AT(int, req, 0x08) = result;  // v5
        CSF_AT(int, req, 0x10) = -1082130432;  // v7 (-1.0f bits)
        CSF_AT(int, req, 0x04) = 3;       // v4

        int globalValueRaw = GetGlobalValue_521BB0();
        void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValueRaw));
        return ApplyGlobalValueRequest(globalValue, nullptr, requestBuffer, 1000001.0f);
    }
    return result;
}

HOOK(0x00525DD0, MenuMissionInfo_SetGlobalIndex, "auto");
