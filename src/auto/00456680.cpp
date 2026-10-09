#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeEntryList, 0x0044D680, (void* a1, char* a2));
CSF_FUNC(int, __fastcall, Scene_ReleaseSerializedStream, 0x004549B0, (void* self, void* edx));
CSF_FUNC(int, __stdcall, Scene_SerializePropertyList, 0x00454CF0, (int a1, int a2));
CSF_FUNC(int, __stdcall, Scene_SerializeResourceList, 0x00455850, (void* a1, int arg4));
CSF_FUNC(int, __stdcall, Scene_SerializeLinkList, 0x00455A40, (void* a1, int a2));
CSF_FUNC(int, __stdcall, Scene_SerializeStateList, 0x004563F0, (void* a1, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060, (void* a1, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(bool, __cdecl, Archive_SerializeBytes, 0x00691580, (bool* a1, int a2, int a3, int a4));
CSF_FUNC(uint32_t*, __fastcall, MemoryStream_Construct, 0x0069A3F0, (void* self, void* edx, int a2, int a3));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

static bool Scene_PrepareArchiveField(void* archive, uint8_t reference = 0)
{
    const bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = reference;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static void Scene_SerializeByteField(void* archive, void* field)
{
    if (Scene_PrepareArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, field, archive) == -1;
    }
}

static void Scene_SerializeIntegerField(void* archive, void* field)
{
    if (Scene_PrepareArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[72 / 4])(
                    backend, nullptr, field, archive) == -1;
    }
}

static void Scene_SerializeFloatField(void* archive, void* field)
{
    if (Scene_PrepareArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[84 / 4])(
                    backend, nullptr, field, archive) == -1;
    }
}

static void Scene_SerializeIndexField(void* archive, void* field)
{
    if (Scene_PrepareArchiveField(archive))
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[88 / 4])(
                    backend, nullptr, field, archive) == -1;
    }
}

static int __fastcall CScene_Serialize(void* self, void* /*edx*/, void* archive)
{
    auto* scene = static_cast<uint8_t*>(self);
    struct
    {
        uint8_t bytes[256];
        int savedContext;
        uint32_t chunkSize;
    } transfer{};

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* backend = CSF_AT(void*, archive, 4);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, backend, 0)[128 / 4])(
                backend, nullptr, 0, archive);
    }

    transfer.savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(int, archive, 0x1C) = 0;
    if (!failed)
    {
        void* backend = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, backend, 0)[68 / 4])(
                    backend, nullptr, scene + 292, archive) == -1;
    }

    uint8_t useStream = CSF_AT(uint8_t, self, 400) == 0
        && CSF_AT(void*, self, 388) != nullptr;
    Scene_SerializeByteField(archive, &useStream);

    if (useStream)
    {
        uint32_t remaining = 0;
        if (CSF_AT(uint8_t, archive, 20) != 0
            && CSF_AT(uint8_t, archive, 0) == 0)
        {
            void* stream = CSF_AT(void*, self, 388);
            remaining = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, stream, 0)[16 / 4])(stream, nullptr);
        }
        Scene_SerializeIntegerField(archive, &remaining);

        if (CSF_AT(uint8_t, archive, 20) == 0
            && CSF_AT(uint8_t, archive, 0) == 0)
        {
            Scene_ReleaseSerializedStream(scene - 4, nullptr);
            if (CSF_AT(void*, self, 392) == nullptr)
            {
                CSF_AT(void*, self, 392) = Mem_Alloc(204800);
                CSF_AT(int, self, 396) = 204800;
            }

            void* storage = Mem_Alloc(20);
            uint32_t* stream = nullptr;
            if (storage)
            {
                stream = MemoryStream_Construct(
                    storage, nullptr,
                    CSF_AT(int, self, 392), CSF_AT(int, self, 396));
            }
            CSF_AT(void*, self, 388) = stream;
            CSF_AT(void*, self, 392) = nullptr;
        }

        void* stream = CSF_AT(void*, self, 388);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, stream, 0)[4 / 4])(stream, nullptr, 0, 0);

        while (remaining != 0)
        {
            transfer.chunkSize = 256;
            if (remaining < 0x100)
                transfer.chunkSize = remaining;

            if (CSF_AT(uint8_t, archive, 20) != 0)
            {
                stream = CSF_AT(void*, self, 388);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                    CSF_AT(void**, stream, 0)[8 / 4])(
                        stream, nullptr, &transfer, transfer.chunkSize);
            }

            Archive_SerializeBytes(
                static_cast<bool*>(archive),
                reinterpret_cast<int>(&transfer), transfer.chunkSize, 0);

            if (CSF_AT(uint8_t, archive, 20) == 0
                && CSF_AT(uint8_t, archive, 0) == 0)
            {
                stream = CSF_AT(void*, self, 388);
                reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                    CSF_AT(void**, stream, 0)[12 / 4])(
                        stream, nullptr, &transfer, transfer.chunkSize);
            }
            remaining -= transfer.chunkSize;
        }
    }
    else
    {
        const int archiveAddress = reinterpret_cast<int>(archive);

        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 12, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 28, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 44, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 60, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 92, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 148, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 132, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeNameList(scene + 76, archiveAddress);

        Scene_SerializeByteField(archive, scene + 8);

        if (Scene_PrepareArchiveField(archive, 1))
            Archive_SerializeObjectRef(scene + 172, static_cast<bool*>(archive));
        if (Scene_PrepareArchiveField(archive, 1))
            SceneLoader_SerializeReference(scene + 176, static_cast<bool*>(archive));
        if (Scene_PrepareArchiveField(archive, 1))
            Archive_SerializeObjectRef(scene + 180, static_cast<bool*>(archive));
        if (Scene_PrepareArchiveField(archive, 1))
            SceneLoader_SerializeReference(scene + 184, static_cast<bool*>(archive));
        if (Scene_PrepareArchiveField(archive, 1))
            Archive_SerializeObjectRef(scene + 188, static_cast<bool*>(archive));
        if (Scene_PrepareArchiveField(archive, 1))
            Archive_SerializeObjectRef(scene + 192, static_cast<bool*>(archive));
        if (Scene_PrepareArchiveField(archive, 1))
            Archive_SerializeObjectRef(scene + 300, static_cast<bool*>(archive));

        Scene_SerializeIntegerField(archive, scene + 108);

        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeStateList(scene + 196, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeEntryList(scene + 212, static_cast<char*>(archive));
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializePropertyList(reinterpret_cast<int>(scene + 228), archiveAddress);

        Scene_SerializeByteField(archive, scene + 244);

        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeResourceList(scene + 248, archiveAddress);
        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeLinkList(scene + 264, archiveAddress);

        Scene_SerializeFloatField(archive, scene + 280);

        if (Scene_PrepareArchiveField(archive))
            Scene_SerializeEntryList(scene + 364, static_cast<char*>(archive));

        Scene_SerializeFloatField(archive, scene + 288);
        Scene_SerializeFloatField(archive, scene + 284);
        Scene_SerializeByteField(archive, scene + 380);

        if (Scene_PrepareArchiveField(archive))
            Archive_SerializeString(reinterpret_cast<int>(scene + 404), archive);

        Scene_SerializeIndexField(archive, scene + 416);
        Scene_SerializeByteField(archive, scene + 420);
        Scene_SerializeByteField(archive, scene + 421);
        Scene_SerializeIndexField(archive, &g_CurrentSceneIndex);
    }

    const bool ready = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(int, archive, 0x1C) = transfer.savedContext;
    if (ready)
    {
        void* backend = CSF_AT(void*, archive, 4);
        const int result = reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, backend, 0)[124 / 4])(backend, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x00456680, CScene_Serialize, "auto");
