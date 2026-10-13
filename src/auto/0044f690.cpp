#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(int, __fastcall, C_00840fdc_ClearSceneResources, 0x00520420, (C_00840fdc* self, void* , char clearOptionalResources));
CSF_FUNC(uint32_t*, __fastcall, C_008443c8_Construct, 0x0054E500, (C_008443c8* self, void* , int value));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(C_0085e5c4*, __fastcall, C_0085e5c4_Construct, 0x00691A20, (C_0085e5c4* self, void*));
CSF_FUNC(char, __fastcall, C_0085e748_FinalizeCompression, 0x00699FD0, (C_0085e748* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0085e748_ConstructCompressionStream, 0x0069A080, (C_0085e748* self, void* , int contextValue, char decompress));
CSF_FUNC(uint32_t*, __fastcall, C_0085e760_Construct, 0x0069A370, (C_0085e760* self, void* , int a2));
CSF_FUNC(C_0085e7b8*, __fastcall, MemoryStream_Construct, 0x0069A3F0, (C_0085e7b8* self, void* , uint32_t dataPointer, uint32_t sizeArg));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall GameSceneAccess_PrepareSceneRebuild(void* self, void* /*edx*/)
{
    alignas(8) uint8_t context[0x28] = {};
    CSF_AT(int, context, 0x18) = 100;
    CSF_AT(int, context, 0x18) =
        CSF_AT(int, reinterpret_cast<void*>(GetGlobalValue_8BC1E0()), 0xB44);

    if (!CSF_AT(uint32_t, self, 0x188))
    {
        auto* memory = reinterpret_cast<C_0085e7b8*>(CoreMemory_AllocUntagged(20));
        C_0085e7b8* stream = nullptr;
        if (memory)
        {
            stream = MemoryStream_Construct(
                memory, nullptr,
                CSF_AT(uint32_t, self, 0x18C),
                CSF_AT(uint32_t, self, 0x190));
        }
        CSF_AT(uint32_t, self, 0x188) = reinterpret_cast<uint32_t>(stream);
        CSF_AT(uint32_t, self, 0x18C) = 0;
    }

    CSF_AT(uint8_t, self, 0x194) = 1;
    auto* compressionMemory =
        reinterpret_cast<C_0085e748*>(CoreMemory_AllocUntagged(20));
    uint32_t* compressionStream = nullptr;
    if (compressionMemory)
    {
        compressionStream = C_0085e748_ConstructCompressionStream(
            compressionMemory, nullptr, CSF_AT(int, self, 0x188), 0);
    }

    auto* adapterMemory =
        reinterpret_cast<C_0085e760*>(CoreMemory_AllocUntagged(12));
    CSF_AT(uint32_t, context, 0x24) = adapterMemory
        ? reinterpret_cast<uint32_t>(C_0085e760_Construct(
              adapterMemory, nullptr, reinterpret_cast<int>(compressionStream)))
        : 0;

    CSF_AT(uint8_t, context, 0x14) = 1;
    CSF_AT(uint8_t, context, 0) = 0;
    auto* auxiliaryMemory =
        reinterpret_cast<C_0085e5c4*>(CoreMemory_AllocUntagged(20));
    CSF_AT(uint32_t, context, 0x0C) = auxiliaryMemory
        ? reinterpret_cast<uint32_t>(C_0085e5c4_Construct(auxiliaryMemory, nullptr))
        : 0;

    auto* wrapperMemory =
        reinterpret_cast<C_008443c8*>(CoreMemory_AllocUntagged(8));
    CSF_AT(uint32_t, context, 0x04) = wrapperMemory
        ? reinterpret_cast<uint32_t>(C_008443c8_Construct(
              wrapperMemory, nullptr, CSF_AT(int, context, 0x24)))
        : 0;

    void* sceneSerializer = static_cast<uint8_t*>(self) + 4;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, sceneSerializer, 0)[2])(sceneSerializer, nullptr, context);

    void* resourceSerializer =
        reinterpret_cast<uint8_t*>(GetGlobalValue_51F740()) + 4;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, resourceSerializer, 0)[2])(resourceSerializer, nullptr, context);

    C_00840fdc_ClearSceneResources(
        reinterpret_cast<C_00840fdc*>(GetGlobalValue_51F740()), nullptr, 1);

    void* worldSerializer =
        reinterpret_cast<uint8_t*>(GetValue8B0554()) + 0x1FBC;
    reinterpret_cast<void(__fastcall*)(void*, void*, uint8_t*)>(
        CSF_AT(void**, worldSerializer, 0)[2])(worldSerializer, nullptr, context);

    C_0085e748_FinalizeCompression(
        reinterpret_cast<C_0085e748*>(compressionStream), nullptr);

    void* memoryStream = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x188));
    int result = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, memoryStream, 0)[4])(memoryStream, nullptr);

    CSF_AT(uint8_t, self, 0x194) = 0;
    if (compressionStream)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, compressionStream, 0)[0])(compressionStream, nullptr, 1);
    }

    void* adapter = reinterpret_cast<void*>(CSF_AT(uint32_t, context, 0x24));
    if (adapter)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, adapter, 0)[0])(adapter, nullptr, 1);
    }

    void* auxiliary = reinterpret_cast<void*>(CSF_AT(uint32_t, context, 0x0C));
    if (auxiliary)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, auxiliary, 0)[0])(auxiliary, nullptr, 1);
        CSF_AT(uint32_t, context, 0x0C) = 0;
    }

    void* wrapper = reinterpret_cast<void*>(CSF_AT(uint32_t, context, 0x04));
    if (wrapper)
    {
        return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, wrapper, 0)[0])(wrapper, nullptr, 1);
    }
    return result;
}

HOOK(0x0044F690, GameSceneAccess_PrepareSceneRebuild, "auto");
