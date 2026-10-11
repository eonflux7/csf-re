#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Archive_SerializeBaseState, 0x004A3550, (void* a1, void* a2));
CSF_FUNC(unsigned int, __stdcall, CoverTask_RunStageA, 0x004A3730, (void* a1, char* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, GetSceneRegionAt, 0x004FC130, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneLoader_ResolveRegionIndex, 0x00774C30, (void* self, void* edx, int a2));

static bool Archive_BeginField(S_004a6d30* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static int __fastcall Archive_SerializeRegionObject(
    void* self, void* /*edx*/, S_004a6d30* archive)
{
    auto* object = static_cast<uint8_t*>(self);
    bool error = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        error = !((bool(__fastcall*)(void*, void*, int, S_004a6d30*))
                      CSF_AT(void**, stream, 0)[0x80 / 4])(
                          stream, nullptr, 0, archive);
    }

    const uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = error;

    if (Archive_BeginField(archive))
        Archive_SerializeBaseState(object + 8, archive);

    if (Archive_BeginField(archive))
        Archive_SerializeObjectRef(object + 56, reinterpret_cast<bool*>(archive));

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                     stream, nullptr, object + 60, archive) == -1;
    }

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x48 / 4])(
                     stream, nullptr, object + 64, archive) == -1;
    }

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x48 / 4])(
                     stream, nullptr, object + 68, archive) == -1;
    }

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x48 / 4])(
                     stream, nullptr, object + 72, archive) == -1;
    }

    if (Archive_BeginField(archive))
        CoverTask_RunStageA(object + 76, reinterpret_cast<char*>(archive));

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x54 / 4])(
                     stream, nullptr, object + 96, archive) == -1;
    }

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                     stream, nullptr, object + 116, archive) == -1;
    }

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x54 / 4])(
                     stream, nullptr, object + 92, archive) == -1;
    }

    uint8_t hasRegionEntries = 0;
    if (CSF_AT(uint8_t, archive, 0x14) != 0)
    {
        const uint32_t begin = CSF_AT(uint32_t, object, 0x68);
        int32_t count = 0;
        if (begin != 0)
        {
            const int32_t byteCount = static_cast<int32_t>(
                CSF_AT(uint32_t, object, 0x6C) - begin);
            count = byteCount / 12;
        }
        hasRegionEntries = count != 0;
    }

    if (Archive_BeginField(archive))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int(__fastcall*)(void*, void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x44 / 4])(
                     stream, nullptr, &hasRegionEntries, archive) == -1;
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        if (hasRegionEntries != 0)
        {
            const int regionKey =
                CSF_AT(int, CSF_AT(void*, object, 0x04), 0x20);
            void* sceneLoader = reinterpret_cast<void*>(GetValue8B0554());
            const int regionIndex =
                SceneLoader_ResolveRegionIndex(sceneLoader, nullptr, regionKey);
            if (regionIndex != -1)
            {
                void** ownerVtable = CSF_AT(void**, object, -4);
                void* scene = reinterpret_cast<void*>(GetValue8B0554());
                const int region = GetSceneRegionAt(scene, nullptr, regionIndex);
                ((void(__fastcall*)(void*, void*, void*))ownerVtable[0x0C / 4])(
                    object - 4, nullptr, reinterpret_cast<void*>(region + 144));
            }
        }

        const int objectAddress = reinterpret_cast<int>(self);
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr,
            objectAddress != 4 ? objectAddress : 0);
    }

    const bool readyToFinish = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (readyToFinish)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, S_004a6d30*))
                 CSF_AT(void**, stream, 0)[0x7C / 4])(
                     stream, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004A3DD0, Archive_SerializeRegionObject, "auto");
