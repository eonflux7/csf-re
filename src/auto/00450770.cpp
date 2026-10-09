#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int32_t, __cdecl, GetGlobalValue_51F740, 0x0051F740, ());
CSF_FUNC(void, __fastcall, C_008443c4_Constructor, 0x0054D850, (C_008443c4* self, void*));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceBuffer, 0x0054D860, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_008443c8_Construct, 0x0054E500, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceArchive, 0x00691C80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_FinalizeReferenceArchive, 0x00691E00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, C_0085e748_Construct, 0x0069A080, (void* self, void* edx, int a2, char a3));
CSF_FUNC(uint32_t*, __fastcall, C_0085e760_Construct, 0x0069A370, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessDeferredReferences, 0x0069A910, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_DestroyReferenceRegistry, 0x0069ACF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceRegistry, 0x0069AD30, (void* self, void* edx));

CSF_GLOBAL(int, g_CurrentSceneIndex, 0x0089972C);

// The packet labels this constructor address as a vtable; no vtable
// store occurs in this function.
[[maybe_unused]] static const auto g_ReferenceRegistryAddressTag =
    CSF_VTBL(0x0069AD30);

struct SceneReferenceContext
{
    uint8_t failed;
    uint8_t padding0001[3];
    uint32_t* reader;
    uint32_t* archive;
    int reserved000C;
    uint32_t* registry;
    uint8_t flag0014;
    uint8_t flag0015;
    uint8_t padding0016[2];
    int limit;
    int reserved001C;
    int reserved0020;
    C_008443c4* buffer;
    uint32_t* adapter;
    uint32_t* source;
};

static_assert(sizeof(SceneReferenceContext) == 0x30);

static int __fastcall SceneLoader_ResetCurrentScene(void* self, void* /*edx*/)
{
    SceneReferenceContext context{};
    context.limit = 100;

    void* sceneStream = CSF_AT(void*, self, 0x188);
    reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
        CSF_AT(void**, sceneStream, 0)[1])(sceneStream, nullptr, 0, 0);

    void* allocation = Mem_Alloc(8);
    context.buffer = allocation
        ? reinterpret_cast<C_008443c4*>(
              SceneLoader_CreateReferenceBuffer(allocation, nullptr))
        : nullptr;

    allocation = Mem_Alloc(20);
    context.source = allocation
        ? C_0085e748_Construct(
              allocation, nullptr, CSF_AT(int, self, 0x188), 1)
        : nullptr;

    allocation = Mem_Alloc(12);
    context.adapter = allocation
        ? C_0085e760_Construct(
              allocation, nullptr, reinterpret_cast<int>(context.source))
        : nullptr;

    context.flag0014 = 0;
    context.failed = 0;
    allocation = Mem_Alloc(24);
    context.archive = allocation
        ? SceneLoader_CreateReferenceArchive(
              allocation, nullptr, reinterpret_cast<int>(context.buffer))
        : nullptr;

    allocation = Mem_Alloc(8);
    context.reader = allocation
        ? C_008443c8_Construct(
              allocation, nullptr, reinterpret_cast<int>(context.adapter))
        : nullptr;

    allocation = Mem_Alloc(12);
    context.registry = allocation
        ? SceneLoader_CreateReferenceRegistry(allocation, nullptr)
        : nullptr;

    const int savedSceneIndex = g_CurrentSceneIndex;
    void* sceneObject = static_cast<uint8_t*>(self) + 4;
    reinterpret_cast<void(__fastcall*)(void*, void*, SceneReferenceContext*)>(
        CSF_AT(void**, sceneObject, 0)[2])(sceneObject, nullptr, &context);
    g_CurrentSceneIndex = savedSceneIndex;

    void* globalObject = reinterpret_cast<uint8_t*>(GetGlobalValue_51F740()) + 4;
    reinterpret_cast<void(__fastcall*)(void*, void*, SceneReferenceContext*)>(
        CSF_AT(void**, globalObject, 0)[2])(globalObject, nullptr, &context);

    void* managerObject = reinterpret_cast<uint8_t*>(GetValue8B0554()) + 0x1FBC;
    reinterpret_cast<void(__fastcall*)(void*, void*, SceneReferenceContext*)>(
        CSF_AT(void**, managerObject, 0)[2])(managerObject, nullptr, &context);

    if (!context.failed)
    {
        SceneLoader_FinalizeReferenceArchive(context.archive, nullptr);
        SceneLoader_ProcessDeferredReferences(
            context.registry, nullptr, reinterpret_cast<int>(&context));
    }

    if (context.source)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, context.source, 0)[0])(context.source, nullptr, 1);
    }
    if (context.adapter)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, context.adapter, 0)[0])(context.adapter, nullptr, 1);
    }
    if (context.archive)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, context.archive, 0)[0])(context.archive, nullptr, 1);
        context.archive = nullptr;
    }
    if (context.reader)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, context.reader, 0)[0])(context.reader, nullptr, 1);
        context.reader = nullptr;
    }

    C_008443c4* buffer = context.buffer;
    if (buffer)
    {
        C_008443c4_Constructor(buffer, nullptr);
        Mem_Free(buffer);
    }

    if (context.registry)
    {
        uint32_t* registry = context.registry;
        SceneLoader_DestroyReferenceRegistry(registry, nullptr);
        Mem_Free(registry);
        context.registry = nullptr;
    }

    sceneStream = CSF_AT(void*, self, 0x188);
    int result = reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
        CSF_AT(void**, sceneStream, 0)[5])(
            sceneStream, nullptr,
            static_cast<uint8_t*>(self) + 0x18C,
            static_cast<uint8_t*>(self) + 0x190);

    sceneStream = CSF_AT(void*, self, 0x188);
    if (sceneStream)
    {
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, sceneStream, 0)[0])(sceneStream, nullptr, 1);
        CSF_AT(void*, self, 0x188) = nullptr;
    }
    return result;
}

HOOK(0x00450770, SceneLoader_ResetCurrentScene, "auto");
