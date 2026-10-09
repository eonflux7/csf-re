#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));

static int __cdecl SoundManager_ApplyResource89Request()
{
    // Base object returned by GetGlobalValue_8BC1E0; the resource lookup uses its sub-object at +0x22A4.
    uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_8BC1E0()));
    int resourceById = SceneLoader_GetResourceById(globalBase + 0x22A4, nullptr, 89);

    // 0x38-byte request record built on the stack.
    uint8_t request[0x38] = {};
    uint8_t* req = request;

    CSF_AT(int, req, 0x00) = 0;
    CSF_AT(int, req, 0x04) = 4;
    CSF_AT(int, req, 0x08) = resourceById;
    CSF_AT(int, req, 0x0C) = -1;
    CSF_AT(int, req, 0x10) = 0;
    CSF_AT(int, req, 0x14) = 0;
    CSF_AT(int, req, 0x18) = 0;
    CSF_AT(int, req, 0x1C) = 0;
    CSF_AT(int, req, 0x20) = 0;
    CSF_AT(void*, req, 0x24) = CSF_VTBL(0x0082D448);
    CSF_AT(int, req, 0x28) = -1;
    CSF_AT(int, req, 0x2C) = -1;
    CSF_AT(int, req, 0x30) = 0;
    CSF_AT(uint8_t, req, 0x34) = 0;
    CSF_AT(uint8_t, req, 0x35) = 1;
    CSF_AT(uint8_t, req, 0x36) = 1;

    int globalValue = GetGlobalValue_521BB0();
    return ApplyGlobalValueRequest(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)),
                                   nullptr, request, 1000002.0f);
}

HOOK(0x00525E70, SoundManager_ApplyResource89Request, "auto");
