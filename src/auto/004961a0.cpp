#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(float, __fastcall, ApplyTargetPointRequest, 0x0040FC50, (void* self, void* edx, void* request));

CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

struct TargetPointRequest
{
    char  enabled;      // +0x00
    int   mode;         // +0x04
    float x;            // +0x08
    float y;            // +0x0C
    float z;            // +0x10
};

static float __fastcall CPlayerCharacter_UpdateFarAimPoint(CPlayerCharacter* self, void* /*edx*/)
{
    if ((self->field_02e0 & 0x1000) == 0)
        return 0.0f; // original leaves st0 untouched on this path

    void* firstElement = reinterpret_cast<void*>(
        GetFirstElement(reinterpret_cast<void*>(GetGlobalValue_4FBDE0()), nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);

    void* target = reinterpret_cast<void*>(self->field_004c);

    TargetPointRequest request;
    request.x = 0.0f;
    request.y = 0.0f;
    request.z = 0.0f;

    void* referenced = GetReferencedObject(provider, nullptr);
    void* viewObject = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, referenced, 0)[0x10 / 4])(referenced, nullptr);

    // The original gives this out-buffer 0x24 bytes of stack room.
    float directionBuf[9];
    float* direction = ((float*(__fastcall*)(void*, void*, float*))CSF_AT(void**, viewObject, 0)[0x5C / 4])(
        viewObject, nullptr, directionBuf);

    float offsetX = direction[0] * g_flt_0082E25C;
    float offsetY = direction[1] * g_flt_0082E25C;
    float offsetZ = direction[2] * g_flt_0082E25C;

    float positionBuf[3];
    float* position = ((float*(__fastcall*)(void*, void*, float*))CSF_AT(void**, self, 0)[0xA4 / 4])(
        self, nullptr, positionBuf);

    float pointX = offsetX + position[0];
    float pointY = offsetY + position[1];
    float pointZ = offsetZ + position[2];

    request.x = pointX;
    request.z = pointZ;
    request.y = pointY;
    request.enabled = 1;
    request.mode = 1;
    ApplyTargetPointRequest(target, nullptr, &request);

    request.mode = 0;
    return ApplyTargetPointRequest(target, nullptr, &request);
}

HOOK(0x004961A0, CPlayerCharacter_UpdateFarAimPoint, "auto");
