#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50,
         (void* self, void* edx));

using PreviewAnimationEntries = std::int32_t[8];
CSF_GLOBAL(PreviewAnimationEntries, g_PreviewAnimations, 0x00843288);
CSF_GLOBAL(float, g_PreviewRadiansToDegrees, 0x008432A8);

static void* const g_PreviewReferenceData =
    reinterpret_cast<void*>(CSF_ADDR_DATA(0x0089A3F8));

static int __fastcall MpCharacterSelect_SetPreviewCharacter(
    void* self, void* /*edx*/, void* previewCharacter)
{
    void* scene = CSF_AT(void*, self, 308);
    int handle = CSF_AT(int, self, 320);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, scene, 0)[25])(scene, nullptr, handle);

    scene = CSF_AT(void*, self, 308);
    handle = CSF_AT(int, self, 324);
    int result = ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, scene, 0)[25])(scene, nullptr, handle);

    CSF_AT(int, self, 320) = 0;
    CSF_AT(int, self, 324) = 0;
    CSF_AT(void*, self, 316) = previewCharacter;
    if (!previewCharacter)
        return result;

    void* renderable = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, previewCharacter, 0)[10])(previewCharacter, nullptr);

    handle = RenderHandle_FromObject(renderable, nullptr);
    scene = CSF_AT(void*, self, 308);
    CSF_AT(int, self, 320) = handle;
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, scene, 0)[24])(scene, nullptr, handle);

    void* character = CSF_AT(void*, self, 316);
    void* childSource = CSF_AT(void*, character, 68);
    void* child = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, childSource, 0)[54])(childSource, nullptr);
    void* childRenderable = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, child, 0)[42])(child, nullptr);

    handle = RenderHandle_FromObject(childRenderable, nullptr);
    scene = CSF_AT(void*, self, 308);
    CSF_AT(int, self, 324) = handle;
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, scene, 0)[24])(scene, nullptr, handle);

    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, renderable, 0)[13])(renderable, nullptr, 1);

    character = CSF_AT(void*, self, 316);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, character, 0)[27])(character, nullptr, 0);

    std::uint8_t pose[60];
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, renderable, 0)[11])(renderable, nullptr, pose);

    void* previewSource = CSF_AT(void*, self, 312);
    void* previewObject = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, previewSource, 0)[4])(previewSource, nullptr);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, previewObject, 0)[5])(
            previewObject, nullptr, g_PreviewReferenceData);

    previewSource = CSF_AT(void*, self, 312);
    previewObject = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, previewSource, 0)[4])(previewSource, nullptr);
    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, previewObject, 0)[21])(previewObject, nullptr, pose);

    std::uint32_t animationIndex = CSF_AT(std::uint32_t, self, 336);
    character = CSF_AT(void*, self, 316);
    std::int32_t animation = g_PreviewAnimations[animationIndex];
    ((void(__fastcall*)(void*, void*, std::int32_t, int, int, int))
        CSF_AT(void**, character, 0)[35])(
            character, nullptr, animation, 0, -1, 0);

    character = CSF_AT(void*, self, 316);
    void* angleSource = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, character, 0)[10])(character, nullptr);
    void* angleTarget = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, angleSource, 0)[6])(angleSource, nullptr);

    float degrees = CSF_AT(float, self, 332) * g_PreviewRadiansToDegrees;
    return ((int(__fastcall*)(void*, void*, float))
        CSF_AT(void**, angleTarget, 0)[13])(angleTarget, nullptr, degrees);
}

HOOK(0x005422D0, MpCharacterSelect_SetPreviewCharacter, "auto");
