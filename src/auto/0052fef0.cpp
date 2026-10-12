#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(char, __cdecl, NormalizeResourceName, 0x006949D0, (char* source, char* destination, unsigned int capacity));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

static void* __fastcall CMenuSceneSelect_AddScene(
    CMenuSceneSelect* self, void* /*edx*/, void* sceneName)
{
    char destination[256];
    char source[256];
    alignas(8) uint8_t displayNameBytes[0x1C];

    CSF_AT(uint32_t, displayNameBytes, 0x18) = 7;
    CSF_AT(uint32_t, displayNameBytes, 0x14) = 0;
    CSF_AT(uint16_t, displayNameBytes, 0x04) = 0;

    auto* pageNames = reinterpret_cast<S_00470ab0*>(
        reinterpret_cast<uint8_t*>(self) + 0x38);
    MenuManager_AppendPageName(pageNames, nullptr, sceneName);

    int32_t count = static_cast<int32_t>(self->field_003c);
    if (count != 0) {
        const uint32_t byteCount =
            CSF_AT(uint32_t, self, 0x40) - self->field_003c;
        count = static_cast<int32_t>(byteCount) / 28;
    }

    const int32_t sceneIndex = count - 1;
    auto* scene = reinterpret_cast<uint8_t*>(self->field_003c)
        + 28 * sceneIndex;

    const char* resourceName;
    if (CSF_AT(uint32_t, scene, 0x18) < 0x10)
        resourceName = reinterpret_cast<const char*>(scene + 4);
    else
        resourceName = CSF_AT(const char*, scene, 0x04);

    char* output = source;
    char character;
    do {
        character = *resourceName++;
        *output++ = character;
    } while (character != '\0');

    NormalizeResourceName(source, destination, 256);
    TextInputSubobject_Init(
        displayNameBytes, nullptr, reinterpret_cast<uint8_t*>(destination));

    void* displayText = CSF_AT(void*, displayNameBytes, 0x04);
    if (CSF_AT(uint32_t, displayNameBytes, 0x18) < 8)
        displayText = displayNameBytes + 4;

    void* listBox = reinterpret_cast<void*>(self->field_0050);
    void* result =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int, int, void*, int)>(
            CSF_AT(void**, listBox, 0)[0x294 / 4])(
                listBox, nullptr, sceneIndex, 0, displayText, sceneIndex);

    if (CSF_AT(uint32_t, displayNameBytes, 0x18) >= 8) {
        result = CSF_AT(void*, displayNameBytes, 0x04);
        if (result) {
            result = reinterpret_cast<void*>(
                static_cast<uintptr_t>(Mem_FreeDebug(result)));
        }
    }
    return result;
}

HOOK(0x0052FEF0, CMenuSceneSelect_AddScene, "auto");
