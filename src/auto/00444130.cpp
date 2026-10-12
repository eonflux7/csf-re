#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(unsigned int, __fastcall, RegisterSceneObjects, 0x00475800, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneLoaderList_GrowTo5, 0x00443E00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, SceneLoaderList_GrowTo50, 0x00443A70, (void* self, void* edx, int a2));

// __thiscall with no stack args (ret 0): self arrives in ecx.
static const void* __fastcall SceneLoader_PrepareObjectResources(void* self, void* /*edx*/)
{
    // The address of the embedded sub-object at +4 is passed as an int argument.
    const int sceneBase = static_cast<int>(reinterpret_cast<uintptr_t>(self));

    CSF_AT(uint32_t, self, 0x08) = 0;

    const int globalObject = GetGlobalObjectField184();
    RegisterSceneObjects(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalObject)),
        nullptr,
        sceneBase + 4);

    // Re-read after the call: callees may modify the object.
    const uint32_t countAt10 = CSF_AT(uint32_t, self, 0x10);
    if (countAt10 < 5) {
        SceneLoaderList_GrowTo5(static_cast<uint8_t*>(self) + 0x0C, nullptr,
                                static_cast<int>(5 - countAt10));
    }

    const void* result = CSF_AT(const void*, self, 0x20);
    const uintptr_t valueAt20 = reinterpret_cast<uintptr_t>(result);
    if (valueAt20 < 0x32) {
        return SceneLoaderList_GrowTo50(static_cast<uint8_t*>(self) + 0x1C, nullptr,
                                        static_cast<int>(50 - static_cast<int>(valueAt20)));
    }
    return result;
}

HOOK(0x00444130, SceneLoader_PrepareObjectResources, "auto");
