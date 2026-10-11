#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroyRecord_43E300, 0x0043E300, (std::uint32_t* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearRenderResources, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_SetRenderWorld, 0x006D6A40, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, SceneLoader_ClearWorldResources, 0x007512A0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static char* __fastcall C_00860bb8_RemoveSceneRecord(
    C_00860bb8* self, void* /*edx*/, char* indexValue)
{
    const std::uint32_t index = reinterpret_cast<std::uint32_t>(indexValue);
    char* result = reinterpret_cast<char*>(self->field_00a0);
    auto* record = CSF_AT(std::uint32_t*, result, 4 * index);

    if (!record)
        return result;

    if (self->field_00b0 == index)
        self->field_00b0 = 0xFFFFFFFF;

    void* renderResources = CSF_AT(void*, record, 0x38);
    if (renderResources)
    {
        SceneLoader_ClearRenderResources(renderResources, nullptr);
        renderResources = CSF_AT(void*, record, 0x38);
        if (renderResources)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, renderResources, 0)[1])(
                    renderResources, nullptr, 1);
            CSF_AT(void*, record, 0x38) = nullptr;
        }

        const int world = CSF_AT(int, record, 0x3C);
        const int globalObject = GetGlobalInt();
        const int renderHandle =
            RenderHandle_FromObject(reinterpret_cast<void*>(globalObject), nullptr);
        SceneLoader_SetRenderWorld(reinterpret_cast<void*>(renderHandle), nullptr, world);
    }

    void* worldResources = CSF_AT(void*, record, 0x3C);
    if (worldResources)
    {
        SceneLoader_ClearWorldResources(worldResources, nullptr);
        worldResources = CSF_AT(void*, record, 0x3C);
        if (worldResources)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, worldResources, 0)[1])(
                    worldResources, nullptr, 1);
            CSF_AT(void*, record, 0x3C) = nullptr;
        }
    }

    DestroyRecord_43E300(record, nullptr);
    Mem_Free(record);

    const std::uint32_t remainingCount = self->field_00a8 - 1;
    self->field_00a8 = remainingCount;
    result = reinterpret_cast<char*>(remainingCount);

    if (index != remainingCount)
    {
        char* destination =
            reinterpret_cast<char*>(self->field_00a0 + 4 * index);
        result = static_cast<char*>(
            _memcpy(destination, destination + 4, 4 * (remainingCount - index)));
    }

    return result;
}

HOOK(0x006CD390, C_00860bb8_RemoveSceneRecord, "auto");
