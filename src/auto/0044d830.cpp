#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned int, __stdcall, Scene_SerializeEntryList, 0x0044D680, (void* a1, char* a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static int __fastcall Scene_ProcessEntryList(void* self, void* /*edx*/, char* entry)
{
    bool blocked = false;
    if (CSF_AT(uint8_t, entry, 0) == 0)
    {
        void* handler = CSF_AT(void*, entry, 0x04);
        blocked = !((bool(__fastcall*)(void*, void*, int, char*))
            CSF_AT(void**, handler, 0)[0x80 / 4])(handler, nullptr, 0, entry);
    }

    char* savedContext = CSF_AT(char*, entry, 0x1C);
    CSF_AT(uint8_t, entry, 0) = blocked;
    CSF_AT(char*, entry, 0x1C) = nullptr;

    if (!blocked)
    {
        CSF_AT(uint8_t, entry, 0x15) = 0;
        CSF_AT(uint8_t, entry, 0x16) = 0;
        Scene_SerializeEntryList(static_cast<uint8_t*>(self) + 4, entry);
    }

    if (CSF_AT(uint8_t, entry, 0x14) == 0 &&
        CSF_AT(uint8_t, entry, 0) == 0)
    {
        SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr);

        uint32_t index = 0;
        if (CSF_AT(uint32_t, self, 0x0C) != 0)
        {
            do
            {
                void* controller = reinterpret_cast<void*>(
                    SceneLoader_GetSceneController(
                        reinterpret_cast<void*>(GetGlobalInt()), nullptr));

                uint32_t entryId = CSF_AT(uint32_t*, self, 0x04)[index];
                int sceneIndex = ((int(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, controller, 0)[0x74 / 4])(
                        controller, nullptr, entryId);

                if (sceneIndex != -1)
                {
                    controller = reinterpret_cast<void*>(
                        SceneLoader_GetSceneController(
                            reinterpret_cast<void*>(GetGlobalInt()), nullptr));

                    void* sceneObject = ((void*(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, controller, 0)[0x70 / 4])(
                            controller, nullptr, sceneIndex);

                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, sceneObject, 0)[0x30 / 4])(
                            sceneObject, nullptr, 0);
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, sceneObject, 0)[0x38 / 4])(
                            sceneObject, nullptr, 0);
                }

                ++index;
            } while (index < CSF_AT(uint32_t, self, 0x0C));
        }
    }

    bool needsFinalization = CSF_AT(uint8_t, entry, 0) == 0;
    CSF_AT(char*, entry, 0x1C) = savedContext;

    if (needsFinalization)
    {
        void* handler = CSF_AT(void*, entry, 0x04);
        if (((bool(__fastcall*)(void*, void*, char*))
            CSF_AT(void**, handler, 0)[0x7C / 4])(handler, nullptr, entry))
        {
            CSF_AT(uint8_t, entry, 0) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, entry, 0) = 1;
    return 1;
}

HOOK(0x0044D830, Scene_ProcessEntryList, "auto");
