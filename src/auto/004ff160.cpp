#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneLoader_SelectModel, 0x004E8E00, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, SceneLoader_ProcessObjectModels, 0x004FEF20, (int a1, int a2));
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetInnerValueAtOffset4, 0x006B81D0, (void* self, void*));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(void, __fastcall, SceneLoader_SetAttachmentModelIndex, 0x006B9800, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int a1, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorCount, 0x006D4190, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSector, 0x006D4720, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Listed by the packet; the corresponding instruction is a direct call.
[[maybe_unused]] static const auto g_ListedModelVtable = CSF_VTBL(0x004E8E00);

static unsigned int __cdecl SceneLoader_ProcessSectorModels()
{
    struct ModelContext
    {
        std::uint8_t flags[4];
        int property32;
        int property28;
        void* provider;
        std::uint32_t sectorCount;
        std::uint32_t sectorIndex;
        std::uint32_t objectIndex;
    };
    struct ObjectList
    {
        int* data;
        std::uint32_t capacity;
        std::uint32_t count;
        std::uint32_t growth;
    };
    static_assert(sizeof(ModelContext) == 0x1C);
    static_assert(sizeof(ObjectList) == 0x10);

    ModelContext context{};
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
    void* provider = GetCachedObjectProvider(accessor, nullptr);
    context.provider = provider;
    context.sectorCount = SceneLoader_GetSectorCount(provider, nullptr);
    context.flags[0] = 1;

    void* object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* properties =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    context.property32 = CSF_AT(int, properties, 0x20);

    object = LookupObjectById(reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    properties =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x18 / 4])(object, nullptr);
    context.property28 = CSF_AT(int, properties, 0x1C);

    std::uint32_t sectorIndex = 0;
    context.sectorIndex = sectorIndex;
    std::uint32_t result = context.sectorCount;

    while (sectorIndex < result)
    {
        void* sector = reinterpret_cast<void*>(
            SceneLoader_GetSector(provider, nullptr, static_cast<int>(sectorIndex)));

        if (CSF_AT(std::uint32_t, sector, 0x28))
        {
            int root = CSF_AT(int, CSF_AT(void*, sector, 0x20), 0);
            ObjectList objects{nullptr, 0, 0, 10};
            SceneLoader_CollectSectorObjects(root, reinterpret_cast<int>(&objects));

            std::uint32_t objectIndex = 0;
            context.objectIndex = 0;
            if (objects.count)
            {
                do
                {
                    SceneLoader_ProcessObjectModels(
                        objects.data[objectIndex], reinterpret_cast<int>(&context));

                    void* attachment = reinterpret_cast<void*>(
                        SceneLoader_GetObjectAttachment(objects.data[objectIndex]));
                    if (attachment)
                    {
                        void* modelName = reinterpret_cast<void*>(
                            GetInnerValueAtOffset4(attachment, nullptr));
                        void* scene = reinterpret_cast<void*>(GetValue8B0554());
                        void* registry = CSF_AT(void*, scene, 0x1FD4);

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
                                    CSF_AT(void**, registry, 0)[0xC / 4])(
                                    registry, nullptr, name);
                        }

                        bool selectModel = modelIndex != -1;
                        if (!selectModel)
                        {
                            void* globalState =
                                reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                            int fallbackModel = CSF_AT(int, globalState, 0x900);
                            modelIndex =
                                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                                    CSF_AT(void**, registry, 0)[0x10 / 4])(
                                    registry, nullptr, fallbackModel);
                            selectModel = modelIndex == -1;
                        }

                        if (selectModel)
                        {
                            void* currentScene =
                                reinterpret_cast<void*>(GetValue8B0554());
                            SceneLoader_SelectModel(currentScene, nullptr, modelIndex);
                            SceneLoader_SetAttachmentModelIndex(
                                attachment, nullptr, modelIndex);
                        }
                        objectIndex = context.objectIndex;
                    }

                    ++objectIndex;
                    context.objectIndex = objectIndex;
                }
                while (objectIndex < objects.count);

                sectorIndex = context.sectorIndex;
                provider = context.provider;
            }

            if (objects.data)
            {
                Mem_Free(objects.data);
                objects.data = nullptr;
            }
        }

        result = context.sectorCount;
        ++sectorIndex;
        context.sectorIndex = sectorIndex;
    }

    return result;
}

HOOK(0x004FF160, SceneLoader_ProcessSectorModels, "auto");
