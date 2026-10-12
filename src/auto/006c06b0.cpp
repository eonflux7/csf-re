#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxMatrix_TranslateFrameByOffset, 0x006ADB90, (uint32_t* self, void* , int offset));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static int* __cdecl GfxFrames_TranslateWrappedAttachment(
    int objectIndex, float offsetU, float offsetV)
{
    int* result = reinterpret_cast<int*>(
        GfxObjectProps_GetObjectAttachment(objectIndex));

    void* attachmentOwner = result ? CSF_AT(void*, result, 0x8) : nullptr;
    if (!attachmentOwner)
        return result;

    auto* entries =
        (reinterpret_cast<C_0085e7b8* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attachmentOwner, 0)[0x1C / 4]))(
                attachmentOwner, nullptr);

    result = reinterpret_cast<int*>(
        C_0085e7b8_FindEntryPayloadByKey(entries, nullptr, 7));
    if (!result)
        return result;

    result = CSF_AT(int*, result, 0);
    if (!result)
        return result;

    void* matrix = CSF_AT(void*, result, 0x4);
    float translatedU = offsetU + CSF_AT(float, matrix, 0x30);
    if (translatedU > g_TransitionOne)
    {
        do
            offsetU -= g_TransitionOne;
        while (offsetU > g_fZero);
    }
    else if (translatedU < g_fZero)
    {
        do
            offsetU += g_TransitionOne;
        while (offsetU < g_fZero);
    }

    float translatedV = offsetV + CSF_AT(float, matrix, 0x34);
    if (translatedV > g_TransitionOne)
    {
        do
            offsetV -= g_TransitionOne;
        while (offsetV > g_fZero);
    }
    else if (translatedV < g_fZero)
    {
        do
            offsetV += g_TransitionOne;
        while (offsetV < g_fZero);
    }

    alignas(8) uint8_t offsetBytes[0xC];
    CSF_AT(float, offsetBytes, 0x0) = offsetU;
    CSF_AT(float, offsetBytes, 0x8) = 0.0f;
    CSF_AT(float, offsetBytes, 0x4) = offsetV;

    return reinterpret_cast<int*>(GfxMatrix_TranslateFrameByOffset(
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(result) + 0x4),
        nullptr,
        reinterpret_cast<int>(offsetBytes)));
}

HOOK(0x006C06B0, GfxFrames_TranslateWrappedAttachment, "auto");
