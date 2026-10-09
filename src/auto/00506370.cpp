#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void, __fastcall, C_008443c4_Constructor, 0x0054D850, (C_008443c4* self, void*));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceBuffer, 0x0054D860, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateArchiveReader, 0x0054E500, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceArchive, 0x00691C80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_FinalizeReferenceArchive, 0x00691E00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateStreamAdapter, 0x0069A080, (void* self, void* edx, int a2, char a3));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateBufferedStream, 0x0069A370, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_ProcessDeferredReferences, 0x0069A910, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, SceneLoader_DestroyReferenceRegistry, 0x0069ACF0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneLoader_CreateReferenceRegistry, 0x0069AD30, (void* self, void* edx));

// The packet also classifies this constructor address as a vtable reference.
static const auto g_ReferenceRegistryVtable = CSF_VTBL(0x0069AD30);

struct SceneLoaderReferenceContext
{
    bool failed;
    std::uint8_t padding[3];
    uint32_t* reader;
    uint32_t* referenceArchive;
    int field_000c;
    uint32_t* registry;
    char flag_0014;
    char flag_0015;
    char flag_0016;
    char padding_0017;
    int limit;
    int progress;
    int field_0020;
    C_0083f5d8* loader;
    C_008443c4* referenceBuffer;
};

static_assert(sizeof(SceneLoaderReferenceContext) == 0x2C);

static int __fastcall CSceneLoader_LoadObjectReferences(
    C_0083f5d8* self, void* /*edx*/,
    uint32_t* firstReference, uint32_t* secondReference, uint32_t* thirdReference)
{
    SceneLoaderReferenceContext context{};
    context.limit = 100;
    context.loader = self;

    void* stream = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10C));
    ((void(__fastcall*)(void*, void*, int, int))
        CSF_AT(void**, stream, 0)[1])(stream, nullptr, 0, 0);

    void* allocation = Mem_Alloc(8);
    context.referenceBuffer = allocation
        ? reinterpret_cast<C_008443c4*>(
            SceneLoader_CreateReferenceBuffer(allocation, nullptr))
        : nullptr;

    allocation = Mem_Alloc(20);
    uint32_t* streamAdapter = allocation
        ? SceneLoader_CreateStreamAdapter(
            allocation, nullptr, CSF_AT(int, self, 0x10C), 1)
        : nullptr;

    allocation = Mem_Alloc(12);
    uint32_t* bufferedStream = allocation
        ? SceneLoader_CreateBufferedStream(
            allocation, nullptr, reinterpret_cast<int>(streamAdapter))
        : nullptr;

    context.flag_0014 = 0;
    context.failed = false;
    allocation = Mem_Alloc(24);
    context.referenceArchive = allocation
        ? SceneLoader_CreateReferenceArchive(
            allocation, nullptr, reinterpret_cast<int>(context.referenceBuffer))
        : nullptr;

    allocation = Mem_Alloc(8);
    context.reader = allocation
        ? SceneLoader_CreateArchiveReader(
            allocation, nullptr, reinterpret_cast<int>(bufferedStream))
        : nullptr;

    allocation = Mem_Alloc(12);
    context.registry = allocation
        ? SceneLoader_CreateReferenceRegistry(allocation, nullptr)
        : nullptr;

    unsigned int referenceCount = 0;
    context.progress = 0;
    bool failed = context.failed;
    if (!failed)
    {
        context.flag_0015 = 0;
        context.flag_0016 = 0;
        failed = ((int(__fastcall*)(void*, void*, unsigned int*, bool*))
            CSF_AT(void**, context.reader, 0)[72 / 4])(
                context.reader, nullptr, &referenceCount, &context.failed) == -1;
        context.failed = failed;
    }

    context.progress = 0;
    if (!failed)
    {
        context.flag_0015 = 0;
        context.flag_0016 = 0;
        Archive_SerializeObjectRef(firstReference, &context.failed);
        failed = context.failed;
    }

    if (referenceCount > 1)
    {
        context.progress = 0;
        if (!failed)
        {
            context.flag_0015 = 0;
            context.flag_0016 = 0;
            Archive_SerializeObjectRef(secondReference, &context.failed);
            failed = context.failed;
        }
    }

    if (referenceCount > 2)
    {
        context.progress = 0;
        if (!failed)
        {
            context.flag_0015 = 0;
            context.flag_0016 = 0;
            Archive_SerializeObjectRef(thirdReference, &context.failed);
        }
    }

    void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    int result = ((int(__fastcall*)(void*, void*, bool*))
        CSF_AT(void**, manager, 0)[2])(manager, nullptr, &context.failed);

    if (!context.failed)
    {
        SceneLoader_FinalizeReferenceArchive(context.referenceArchive, nullptr);
        result = SceneLoader_ProcessDeferredReferences(
            context.registry, nullptr, reinterpret_cast<int>(&context.failed));
    }

    if (streamAdapter)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, streamAdapter, 0)[0])(streamAdapter, nullptr, 1);
    }
    if (bufferedStream)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, bufferedStream, 0)[0])(bufferedStream, nullptr, 1);
    }
    if (context.referenceArchive)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, context.referenceArchive, 0)[0])(
                context.referenceArchive, nullptr, 1);
        context.referenceArchive = nullptr;
    }
    if (context.reader)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, context.reader, 0)[0])(context.reader, nullptr, 1);
        context.reader = nullptr;
    }

    C_008443c4* referenceBuffer = context.referenceBuffer;
    if (referenceBuffer)
    {
        C_008443c4_Constructor(referenceBuffer, nullptr);
        result = Mem_Free(referenceBuffer);
    }

    if (context.registry)
    {
        uint32_t* registry = context.registry;
        SceneLoader_DestroyReferenceRegistry(registry, nullptr);
        result = Mem_Free(registry);
        context.registry = nullptr;
    }

    C_0083f5d8* loader = context.loader;
    stream = reinterpret_cast<void*>(CSF_AT(uint32_t, loader, 0x10C));
    if (stream)
    {
        result = ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, stream, 0)[0])(stream, nullptr, 1);
        CSF_AT(uint32_t, loader, 0x10C) = 0;
    }
    return result;
}

HOOK(0x00506370, CSceneLoader_LoadObjectReferences, "auto");
