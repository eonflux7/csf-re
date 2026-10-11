#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, GfxObjectProps_ReadTableEntryField4, 0x006B8820, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, S_006cbc80_TestSpatialMatch, 0x006CBC80,
         (void* self, void* edx, float* a2, float* a3, int a4));

using ObjectFallback = int (__cdecl*)(float*);
CSF_GLOBAL(ObjectFallback, g_ObjectFallback, 0x008BB48C);

// The packet also identifies the spatial-test address as a vtable reference.
static const auto kSpatialMatchVtable = CSF_VTBL(0x006CBC80);

static int __cdecl GfxEnvironment_FilterSpatialObject(float* object)
{
    void* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    void* flagProvider = CSF_AT(void*, globalState, 0xF0);
    void* flags =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, flagProvider, 0)[0x7C / 4])(
                flagProvider, nullptr);

    if ((CSF_AT(std::uint32_t, flags, 0) & 4) == 0)
    {
        alignas(8) std::uint8_t firstBytes[0xC];
        alignas(8) std::uint8_t secondBytes[0xC];
        float* first = reinterpret_cast<float*>(firstBytes);
        float* second = reinterpret_cast<float*>(secondBytes);

        first[0] = CSF_AT(float, object, 0x60);
        first[1] = CSF_AT(float, object, 0x64);
        first[2] = CSF_AT(float, object, 0x68);
        second[0] = CSF_AT(float, object, 0x6C);
        second[1] = CSF_AT(float, object, 0x70);
        second[2] = CSF_AT(float, object, 0x74);

        int tableEntry = GfxObjectProps_ReadTableEntryField4(
            reinterpret_cast<int>(object)) + 4;
        void* loader = reinterpret_cast<void*>(GetGlobalInt());
        void* controller = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(loader, nullptr));

        if (S_006cbc80_TestSpatialMatch(
                controller, nullptr, second, first, tableEntry))
        {
            return reinterpret_cast<int>(object);
        }
    }

    void* loader = reinterpret_cast<void*>(GetGlobalInt());
    void* controller = reinterpret_cast<void*>(
        SceneLoader_GetSceneController(loader, nullptr));
    void* statistics =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x40 / 4])(
                controller, nullptr);

    ++CSF_AT(std::uint32_t, statistics, 0xB0);
    return g_ObjectFallback(object);
}

HOOK(0x006BC730, GfxEnvironment_FilterSpatialObject, "auto");
