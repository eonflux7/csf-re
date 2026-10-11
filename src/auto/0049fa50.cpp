#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(unsigned int, __cdecl, Hud_WriteNormalizedScreenSize, 0x0042DE60, (float* out));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuMessageBox_ResolveEntryByKey, 0x00692440, (int code, int flag));

CSF_GLOBAL(float, g_LabelHeightOffset, 0x00833C58);
CSF_GLOBAL(float, g_LabelBackgroundOffsetScale, 0x00834388);

static int __fastcall HudAlertIcons_DrawObjectLabel(
    void* self, void* /*edx*/, C_0085ec18* reference,
    C_008342e8* object, int selectedLevel)
{
    // Keep the overlapping output vectors in the original frame's full storage.
    alignas(8) uint8_t frame[0x54] = {};
    auto* color = reinterpret_cast<float*>(frame);
    auto* rectangle = reinterpret_cast<int32_t*>(frame + 0x10);
    auto* position = reinterpret_cast<float*>(frame + 0x20);
    auto* backgroundColor = reinterpret_cast<float*>(frame + 0x2C);
    auto* vectorScratch = reinterpret_cast<float*>(frame + 0x30);
    auto* dimensions = reinterpret_cast<int32_t*>(frame + 0x34);
    auto* projected = reinterpret_cast<float*>(frame + 0x3C);
    auto* screenScale = reinterpret_cast<float*>(frame + 0x44);
    auto* screenPosition = reinterpret_cast<int32_t*>(frame + 0x4C);

    void* labelOwner = CSF_AT(void*, self, 0x0C);
    void* label = ((void*(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, labelOwner, 0)[0x54 / 4])(
            labelOwner, nullptr, CSF_AT(void*, self, 0x08));

    const int level = static_cast<int32_t>(object->field_007c);
    backgroundColor[2] = 40.0f;
    backgroundColor[3] = 255.0f;
    if (level > selectedLevel) {
        backgroundColor[0] = 255.0f;
        backgroundColor[1] = 40.0f;
    } else {
        backgroundColor[1] = 255.0f;
        backgroundColor[0] = level == selectedLevel ? 255.0f : 40.0f;
    }
    std::memcpy(color, backgroundColor, 0x10);
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, label, 0)[0x60 / 4])(label, nullptr, color);

    int textKey;
    bool hasText = true;
    switch (object->field_007c) {
    case 1: textKey = 1194342708; break;
    case 2: textKey = 1194342709; break;
    case 3: textKey = 1194342710; break;
    case 4: textKey = 1194342711; break;
    case 5: textKey = 1194342712; break;
    default: hasText = false; break;
    }
    if (hasText) {
        void** labelVtable = CSF_AT(void**, label, 0);
        int text = MenuMessageBox_ResolveEntryByKey(textKey, 8);
        ((void(__fastcall*)(void*, void*, int))
            labelVtable[0x40 / 4])(label, nullptr, text);
    }

    screenScale[0] = 0.0f;
    screenScale[1] = 0.0f;
    GetGlobal8afc78();
    Hud_WriteNormalizedScreenSize(screenScale);
    ((void(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, label, 0)[0x98 / 4])(label, nullptr, screenScale);

    void* font = CSF_AT(void*, self, 0x08);
    rectangle[0] = 0;
    rectangle[1] = 0;
    rectangle[2] = 0;
    rectangle[3] = 0;
    int lineHeight = ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, font, 0)[0x18 / 4])(font, nullptr, 1);
    rectangle[3] = rectangle[1] + lineHeight;

    int32_t* measured = ((int32_t*(__fastcall*)(void*, void*, int32_t*, int))
        CSF_AT(void**, label, 0)[0x78 / 4])(label, nullptr, dimensions, 0);
    rectangle[2] = measured[0] + rectangle[0];
    ((void(__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, label, 0)[0x48 / 4])(label, nullptr, rectangle);

    void* worldPosition = GetObjectPositionVector(object, nullptr);
    std::memcpy(position, worldPosition, 0xC);
    void* objectVector = ((void*(__fastcall*)(void*, void*, float*))
        CSF_AT(void**, object, 0)[0xA8 / 4])(object, nullptr, vectorScratch);
    position[1] = CSF_AT(float, objectVector, 0x04) + g_LabelHeightOffset;

    void* camera = GetReferencedObject(reference, nullptr);
    ((void(__fastcall*)(void*, void*, float*, float*))
        CSF_AT(void**, camera, 0)[0x68 / 4])(camera, nullptr, projected, position);
    screenPosition[0] = static_cast<int32_t>(static_cast<int64_t>(projected[0]));
    screenPosition[1] = static_cast<int32_t>(static_cast<int64_t>(projected[1]));

    int32_t* bounds = ((int32_t*(__fastcall*)(void*, void*))
        CSF_AT(void**, label, 0)[0x4C / 4])(label, nullptr);
    screenPosition[0] += (bounds[2] - bounds[0]) / -2;

    font = CSF_AT(void*, self, 0x08);
    lineHeight = ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, font, 0)[0x18 / 4])(font, nullptr, 1);
    screenPosition[1] -= lineHeight;
    ((void(__fastcall*)(void*, void*, int32_t*))
        CSF_AT(void**, label, 0)[0x50 / 4])(label, nullptr, screenPosition);

    labelOwner = CSF_AT(void*, self, 0x0C);
    void* background = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, labelOwner, 0)[0x48 / 4])(labelOwner, nullptr);
    if (background) {
        rectangle[2] += 8;
        rectangle[3] += 8;
        dimensions[0] = rectangle[2] - rectangle[0];
        dimensions[1] = rectangle[3] - rectangle[1];
        ((void(__fastcall*)(void*, void*, int32_t*))
            CSF_AT(void**, background, 0)[0x24 / 4])(
                background, nullptr, dimensions);

        screenPosition[0] += static_cast<int32_t>(
            static_cast<int64_t>(screenScale[0] * g_LabelBackgroundOffsetScale));
        screenPosition[1] += static_cast<int32_t>(
            static_cast<int64_t>(screenScale[1] * g_LabelBackgroundOffsetScale));
        ((void(__fastcall*)(void*, void*, int32_t*))
            CSF_AT(void**, background, 0)[0x1C / 4])(
                background, nullptr, screenPosition);

        backgroundColor[0] = 20.0f;
        backgroundColor[1] = 20.0f;
        backgroundColor[2] = 20.0f;
        backgroundColor[3] = 110.0f;
        ((void(__fastcall*)(void*, void*, float*))
            CSF_AT(void**, background, 0)[0x2C / 4])(
                background, nullptr, backgroundColor);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, background, 0)[0x08 / 4])(background, nullptr);
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, background, 0)[0])(background, nullptr, 1);
    }

    ((void(__fastcall*)(void*, void*))
        CSF_AT(void**, label, 0)[0x08 / 4])(label, nullptr);
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, label, 0)[0])(label, nullptr, 1);
}

HOOK(0x0049FA50, HudAlertIcons_DrawObjectLabel, "auto");
