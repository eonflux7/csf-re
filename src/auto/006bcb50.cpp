#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __fastcall, SetValue18, 0x0050AAB0, (void* self, void* , int value));
CSF_FUNC(std::uint32_t*, __cdecl, RwFrameGetLTM, 0x00623720, (std::uint32_t frame));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(char, __fastcall, QueryLeafState, 0x006B6D70, (void* self, void* edx, float* a2, char* a3));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(uint32_t, __fastcall, GfxObjectProps_SetFlagLightmapBit, 0x006B8BE0, (void* self, void* , char enable));
CSF_FUNC(int, __cdecl, SceneLoader_CollectSectorObjects, 0x006C0B90, (int clumpHandle, int collectorHandle));

static int __cdecl GfxEnvironment_PrepareClumpResources(int clumpHandle)
{
    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));
    void* leafQuery = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
        CSF_AT(void**, controller, 0)[0x44 / 4])(controller, nullptr);

    alignas(8) std::uint8_t collectorBytes[0x10];
    CSF_AT(std::uint32_t, collectorBytes, 0x0) = 0;
    CSF_AT(std::uint32_t, collectorBytes, 0x4) = 0;
    CSF_AT(std::uint32_t, collectorBytes, 0x8) = 0;
    CSF_AT(int, collectorBytes, 0xC) = 10;

    SceneLoader_CollectSectorObjects(
        clumpHandle, reinterpret_cast<int>(collectorBytes));

    std::uint32_t index = 0;
    if (CSF_AT(std::uint32_t, collectorBytes, 0x8) != 0)
    {
        do
        {
            int attachment = GfxObjectProps_GetObjectAttachment(
                CSF_AT(int*, collectorBytes, 0x0)[index]);
            if (attachment)
            {
                void* object = reinterpret_cast<void*>(
                    CSF_AT(int*, collectorBytes, 0x0)[index]);
                std::uint32_t* transform = RwFrameGetLTM(
                    CSF_AT(std::uint32_t, object, 0x4));

                alignas(8) std::uint8_t positionBytes[0xF];
                CSF_AT(float, positionBytes, 0x0) =
                    CSF_AT(float, transform, 0x30);
                CSF_AT(float, positionBytes, 0x4) =
                    CSF_AT(float, transform, 0x34);
                CSF_AT(float, positionBytes, 0x8) =
                    CSF_AT(float, transform, 0x38);

                char leafState = -1;
                QueryLeafState(leafQuery, nullptr,
                    reinterpret_cast<float*>(positionBytes), &leafState);
                SetValue18(reinterpret_cast<void*>(attachment), nullptr,
                    static_cast<unsigned char>(leafState));
                GfxObjectProps_SetFlagLightmapBit(
                    reinterpret_cast<void*>(attachment), nullptr, 1);
            }
            ++index;
        }
        while (index < CSF_AT(std::uint32_t, collectorBytes, 0x8));
    }

    void* objects = CSF_AT(void*, collectorBytes, 0x0);
    if (objects)
        return Mem_Free(objects);
    return 0;
}

HOOK(0x006BCB50, GfxEnvironment_PrepareClumpResources, "auto");
