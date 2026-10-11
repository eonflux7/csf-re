#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static C_0082d410* __fastcall C_00834598_DetachFromSceneAndClearName(C_00834598* self, void* /*edx*/)
{
    C_0082d410* result = reinterpret_cast<C_0082d410*>(self->field_008c);
    if (result)
    {
        uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
        uint32_t ownerLinked = CSF_AT(uint32_t, owner, 0x44);
        if (ownerLinked)
        {
            void* linkedObj = reinterpret_cast<void*>(ownerLinked);
            auto checkLinked = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, linkedObj, 0)[54]);
            if (checkLinked(linkedObj, nullptr))
            {
                auto getLinked = reinterpret_cast<int(__fastcall*)(void*, void*)>(CSF_AT(void**, linkedObj, 0)[54]);
                int linkedChild = getLinked(linkedObj, nullptr);
                void* childObj = reinterpret_cast<void*>(linkedChild);
                auto setChildFlag = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, childObj, 0)[12]);
                setChildFlag(childObj, nullptr, 1);
            }
        }

        void* sceneObj = reinterpret_cast<void*>(self->field_008c);
        auto releaseSceneObj = reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, sceneObj, 0)[41]);
        releaseSceneObj(sceneObj, nullptr);

        C_00860bb8* globalInt = reinterpret_cast<C_00860bb8*>(GetGlobalInt());
        int sceneController = SceneLoader_GetSceneController(globalInt, nullptr);

        void* controllerObj = reinterpret_cast<void*>(sceneController);
        auto removeFromScene = reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(CSF_AT(void**, controllerObj, 0)[6]);
        removeFromScene(controllerObj, nullptr, self->field_008c);

        self->field_008c = 0;
        return reinterpret_cast<C_0082d410*>(
            AssignStringRange_429B60(
                reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x90),
                nullptr,
                reinterpret_cast<char*>(&g_byte_82E65C),
                0));
    }
    return result;
}

HOOK(0x004A5C40, C_00834598_DetachFromSceneAndClearName, "auto");
