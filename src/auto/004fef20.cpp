#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_SelectModel, 0x004E8E00, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, SceneLoader_ProcessChild, 0x004FEE70, (int a1, int a2));
CSF_FUNC(int, __fastcall, GetInnerValueAtOffset4, 0x006B81D0, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_EnableAttachment, 0x006B8C10, (void* self, void* edx, char a2));
CSF_FUNC(void, __fastcall, SceneLoader_SetAttachmentModelIndex, 0x006B9800, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Retain the packet's listed vtable binding; the instruction itself is a direct call.
[[maybe_unused]] static const auto g_ListedModelVtable = CSF_VTBL(0x004E8E00);

static int __cdecl SceneLoader_ProcessObjectModels(int objectAddress, int contextAddress)
{
    void* attachment =
        reinterpret_cast<void*>(SceneLoader_GetObjectAttachment(objectAddress));
    void* children = CSF_AT(void*, reinterpret_cast<void*>(objectAddress), 0x18);
    void* context = reinterpret_cast<void*>(contextAddress);

    CSF_AT(std::uint8_t, context, 0x1) = 0;
    const std::uint32_t childCount = CSF_AT(std::uint32_t, children, 0x24);

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        int* childArray = CSF_AT(int*, children, 0x20);
        SceneLoader_ProcessChild(childArray[index], contextAddress);
    }

    if (!CSF_AT(std::uint8_t, context, 0x1))
    {
        if (!attachment)
            return objectAddress;

        SceneLoader_EnableAttachment(attachment, nullptr, 1);
    }

    if (attachment)
    {
        void* modelName =
            reinterpret_cast<void*>(GetInnerValueAtOffset4(attachment, nullptr));
        void* scene = reinterpret_cast<void*>(GetValue8B0554());
        void* modelRegistry = CSF_AT(void*, scene, 0x1FD4);

        int modelIndex = -1;
        if (modelName && CSF_AT(std::uint32_t, modelName, 0x14))
        {
            const char* name;
            if (CSF_AT(std::uint32_t, modelName, 0x18) < 0x10u)
                name = reinterpret_cast<const char*>(modelName) + 4;
            else
                name = CSF_AT(const char*, modelName, 0x4);

            modelIndex =
                reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
                    CSF_AT(void**, modelRegistry, 0)[3])(
                    modelRegistry, nullptr, name);
        }

        bool selectModel = modelIndex != -1;
        if (!selectModel)
        {
            void* globalState = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            int fallbackModel = CSF_AT(int, globalState, 0x900);

            modelIndex =
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, modelRegistry, 0)[4])(
                    modelRegistry, nullptr, fallbackModel);

            selectModel = modelIndex == -1;
        }

        if (selectModel)
        {
            void* currentScene = reinterpret_cast<void*>(GetValue8B0554());
            SceneLoader_SelectModel(currentScene, nullptr, modelIndex);
            SceneLoader_SetAttachmentModelIndex(attachment, nullptr, modelIndex);
        }
    }

    return objectAddress;
}

HOOK(0x004FEF20, SceneLoader_ProcessObjectModels, "auto");
