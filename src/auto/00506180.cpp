#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(uint32_t*, __fastcall, C_008443c8_Construct, 0x0054E500, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, C_0085e5c4_Construct, 0x00691A20, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0085e748_Finalize, 0x00699FD0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0085e748_Construct, 0x0069A080, (void* self, void* edx, int a2, char a3));
CSF_FUNC(uint32_t*, __fastcall, C_0085e760_Construct, 0x0069A370, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_0085e7b8_Construct, 0x0069A590, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall CSceneLoader_SerializeObjectReferences(
    void* self, void* /*edx*/, int firstObject, int secondObject, int thirdObject)
{
    alignas(4) uint8_t archive[0x24]{};
    bool* archiveState = reinterpret_cast<bool*>(archive);

    CSF_AT(bool, archive, 0x00) = false;
    CSF_AT(void*, archive, 0x04) = nullptr;
    CSF_AT(int, archive, 0x08) = 0;
    CSF_AT(void*, archive, 0x0C) = nullptr;
    CSF_AT(char, archive, 0x14) = 0;
    CSF_AT(char, archive, 0x15) = 0;
    CSF_AT(int, archive, 0x18) = 100;
    CSF_AT(int, archive, 0x1C) = 0;
    CSF_AT(int, archive, 0x20) = 0;

    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    CSF_AT(int, archive, 0x18) = CSF_AT(int, settings, 0xB44);

    void* storageMemory = Mem_Alloc(20);
    uint32_t* storage = storageMemory
        ? C_0085e7b8_Construct(storageMemory, nullptr)
        : nullptr;
    CSF_AT(uint32_t, self, 0x10C) = reinterpret_cast<uint32_t>(storage);

    void* streamMemory = Mem_Alloc(20);
    uint32_t* stream = streamMemory
        ? C_0085e748_Construct(
              streamMemory, nullptr, static_cast<int>(CSF_AT(uint32_t, self, 0x10C)), 0)
        : nullptr;

    void* adapterMemory = Mem_Alloc(12);
    uint32_t* adapter = adapterMemory
        ? C_0085e760_Construct(adapterMemory, nullptr, reinterpret_cast<int>(stream))
        : nullptr;

    CSF_AT(char, archive, 0x14) = 1;
    CSF_AT(bool, archive, 0x00) = false;

    void* referencesMemory = Mem_Alloc(20);
    CSF_AT(void*, archive, 0x0C) = referencesMemory
        ? C_0085e5c4_Construct(referencesMemory, nullptr)
        : nullptr;

    void* serializerMemory = Mem_Alloc(8);
    void* serializer = serializerMemory
        ? C_008443c8_Construct(serializerMemory, nullptr, reinterpret_cast<int>(adapter))
        : nullptr;
    CSF_AT(void*, archive, 0x04) = serializer;

    int objectCount = 1;
    if (secondObject)
    {
        objectCount = 2;
        if (thirdObject)
            objectCount = 3;
    }

    bool failed = CSF_AT(bool, archive, 0x00);
    CSF_AT(int, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(char, archive, 0x15) = 0;
        CSF_AT(char, archive, 0x16) = 0;
        failed = ((int(__fastcall*)(void*, void*, int*, bool*))
                      CSF_AT(void**, serializer, 0)[0x48 / 4])(
                      serializer, nullptr, &objectCount, archiveState) == -1;
        CSF_AT(bool, archive, 0x00) = failed;
    }

    CSF_AT(int, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(char, archive, 0x15) = 0;
        CSF_AT(char, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&firstObject, archiveState);
        failed = CSF_AT(bool, archive, 0x00);
    }

    if (secondObject)
    {
        CSF_AT(int, archive, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(char, archive, 0x15) = 0;
            CSF_AT(char, archive, 0x16) = 0;
            Archive_SerializeObjectRef(&secondObject, archiveState);
            failed = CSF_AT(bool, archive, 0x00);
        }
    }

    if (thirdObject)
    {
        CSF_AT(int, archive, 0x1C) = 0;
        if (!failed)
        {
            CSF_AT(char, archive, 0x15) = 0;
            CSF_AT(char, archive, 0x16) = 0;
            Archive_SerializeObjectRef(&thirdObject, archiveState);
        }
    }

    void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    ((void(__fastcall*)(void*, void*, bool*))
         CSF_AT(void**, manager, 0)[8 / 4])(manager, nullptr, archiveState);

    C_0085e748_Finalize(stream, nullptr);

    void* references = CSF_AT(void*, archive, 0x0C);
    int result = ((int(__fastcall*)(void*, void*))
                      CSF_AT(void**, references, 0)[0x10 / 4])(references, nullptr);

    if (stream)
        result = ((int(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, stream, 0)[0])(stream, nullptr, 1);

    if (adapter)
        result = ((int(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, adapter, 0)[0])(adapter, nullptr, 1);

    references = CSF_AT(void*, archive, 0x0C);
    if (references)
    {
        result = ((int(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, references, 0)[0])(references, nullptr, 1);
        CSF_AT(void*, archive, 0x0C) = nullptr;
    }

    serializer = CSF_AT(void*, archive, 0x04);
    if (serializer)
        result = ((int(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, serializer, 0)[0])(serializer, nullptr, 1);

    return result;
}

HOOK(0x00506180, CSceneLoader_SerializeObjectReferences, "auto");
