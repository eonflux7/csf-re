#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __fastcall, GfxMatrix_TranslateFrameByOffset, 0x006ADB90, (uint32_t* self, void* , int offset));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

static int* __fastcall C_00861ce0_TranslateTextureFrame(
    C_00861ce0* self, void* /*edx*/, float horizontal, float vertical)
{
    void* owner = reinterpret_cast<void*>(self->field_0040);
    C_0085e7b8* entries =
        reinterpret_cast<C_0085e7b8* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x1C / 4])(owner, nullptr);

    int* payload = reinterpret_cast<int*>(
        C_0085e7b8_FindEntryPayloadByKey(entries, nullptr, 7));
    if (!payload)
        return payload;

    int* frameObject = reinterpret_cast<int*>(CSF_AT(uint32_t, payload, 0));
    if (!frameObject)
        return frameObject;

    void* matrix = reinterpret_cast<void*>(CSF_AT(uint32_t, frameObject, 0x04));

    float translatedHorizontal = horizontal;
    float horizontalSum = horizontal + CSF_AT(float, matrix, 0x30);
    if (horizontalSum > g_TransitionOne)
    {
        do
        {
            translatedHorizontal -= g_TransitionOne;
        } while (translatedHorizontal > g_fZero);
    }
    else if (horizontalSum < g_fZero)
    {
        do
        {
            translatedHorizontal += g_TransitionOne;
        } while (translatedHorizontal < g_fZero);
    }

    float translatedVertical = vertical;
    float verticalSum = vertical + CSF_AT(float, matrix, 0x34);
    if (verticalSum > g_TransitionOne)
    {
        do
        {
            translatedVertical -= g_TransitionOne;
        } while (translatedVertical > g_fZero);
    }
    else if (verticalSum < g_fZero)
    {
        do
        {
            translatedVertical += g_TransitionOne;
        } while (translatedVertical < g_fZero);
    }

    alignas(8) uint8_t offsetBytes[0xC];
    CSF_AT(float, offsetBytes, 0x00) = translatedHorizontal;
    CSF_AT(float, offsetBytes, 0x08) = 0.0f;
    CSF_AT(float, offsetBytes, 0x04) = translatedVertical;

    return reinterpret_cast<int*>(GfxMatrix_TranslateFrameByOffset(
        reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(frameObject) + 0x04),
        nullptr,
        reinterpret_cast<int>(offsetBytes)));
}

HOOK(0x006E5B20, C_00861ce0_TranslateTextureFrame, "auto");
