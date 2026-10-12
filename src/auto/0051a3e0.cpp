#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, IntArray_PushBack, 0x004FD290, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));

static int __fastcall C_00840704_ResolveEntryReferences(
    C_00840704* self, void* /*edx*/, int entryIndex, int context)
{
    void* table = reinterpret_cast<void*>(self->field_0004);
    uint32_t recordsAddress = CSF_AT(uint32_t, table, 0x4);
    auto* record = reinterpret_cast<uint8_t*>(
        recordsAddress + 96 * static_cast<uint32_t>(entryIndex));

    void* global = reinterpret_cast<void*>(GetGlobalInt());
    void* accessor = reinterpret_cast<void*>(
        GlobalInt_ResolveAccessor(global, nullptr));
    void* resolver =
        ((void* (__fastcall*)(void*, void*))
            CSF_AT(void**, accessor, 0)[0x14 / 4])(accessor, nullptr);

    uint32_t referencesBegin = CSF_AT(uint32_t, record, 0x24);
    uint32_t remaining = referencesBegin;
    if (referencesBegin)
    {
        int32_t byteCount = static_cast<int32_t>(
            CSF_AT(uint32_t, record, 0x28) - referencesBegin);
        remaining = static_cast<uint32_t>(byteCount / 44);
    }

    if (remaining)
    {
        uint32_t arrayOffset = 16 * static_cast<uint32_t>(entryIndex);
        uint32_t referenceOffset = 0;
        alignas(8) uint8_t resolvedValueBytes[0x14];
        auto* resolvedValue = reinterpret_cast<uint32_t*>(resolvedValueBytes);

        do
        {
            auto* reference = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, record, 0x24) + referenceOffset);
            const char* name;
            if (CSF_AT(uint32_t, reference, 0x18) < 0x10)
                name = reinterpret_cast<const char*>(reference + 4);
            else
                name = reinterpret_cast<const char*>(
                    CSF_AT(uint32_t, reference, 0x4));

            int resolved =
                ((int (__fastcall*)(void*, void*, const char*, int))
                    CSF_AT(void**, resolver, 0)[0x10 / 4])(
                        resolver, nullptr, name, context);

            auto* resolvedArray = reinterpret_cast<S_0046c410*>(
                self->field_000c + arrayOffset);
            auto* end = reinterpret_cast<uint32_t*>(
                CSF_AT(uint32_t, resolvedArray, 0x8));
            auto* current = reinterpret_cast<uint32_t*>(
                resolvedArray->field_0004);
            *resolvedValue = static_cast<uint32_t>(resolved);

            while (current != end)
            {
                if (*current == static_cast<uint32_t>(resolved))
                    break;
                ++current;
            }
            if (current == end)
                IntArray_PushBack(resolvedArray, nullptr, resolvedValue);

            reference = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, record, 0x24) + referenceOffset);
            uint32_t dependenciesBegin = CSF_AT(uint32_t, reference, 0x20);
            uint32_t dependencyCount = 0;
            if (dependenciesBegin)
            {
                int32_t byteCount = static_cast<int32_t>(
                    CSF_AT(uint32_t, reference, 0x24) - dependenciesBegin);
                dependencyCount = static_cast<uint32_t>(byteCount >> 3);
            }

            for (uint32_t i = 0; i < dependencyCount; ++i)
            {
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                void* dependencyManager = reinterpret_cast<void*>(
                    CSF_AT(uint32_t, state, 0xE4));
                auto* currentReference = reinterpret_cast<uint8_t*>(
                    CSF_AT(uint32_t, record, 0x24) + referenceOffset);
                auto* dependency = reinterpret_cast<void*>(
                    CSF_AT(uint32_t, currentReference, 0x20) + 8 * i);
                uint32_t dependencyId = CSF_AT(uint32_t, dependency, 0);

                ((void (__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, dependencyManager, 0)[0x1C / 4])(
                        dependencyManager, nullptr, dependencyId);
            }

            referenceOffset += 44;
            --remaining;
        } while (remaining);
    }

    int result = CSF_AT(int, record, 0x4C);
    if (result)
    {
        global = reinterpret_cast<void*>(GetGlobalInt());
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(global, nullptr));

        const char* name;
        if (CSF_AT(uint32_t, record, 0x50) < 0x10)
            name = reinterpret_cast<const char*>(record + 0x3C);
        else
            name = reinterpret_cast<const char*>(
                CSF_AT(uint32_t, record, 0x3C));

        return ((int (__fastcall*)(void*, void*, const char*, int))
            CSF_AT(void**, sceneController, 0)[0x10 / 4])(
                sceneController, nullptr, name, 0);
    }
    return result;
}

HOOK(0x0051A3E0, C_00840704_ResolveEntryReferences, "auto");
