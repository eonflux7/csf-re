#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall AudioSoundManager_ReleaseHandles(void* self, void* /*edx*/)
{
    int result = 0;

    void* handle = CSF_AT(void*, self, 0x8);
    if (handle) {
        // Virtual slot 0 of the handle object, called as a __thiscall with flag 1.
        using HandleReleaseFn = int(__fastcall*)(void*, void*, int);
        auto release = reinterpret_cast<HandleReleaseFn>(CSF_AT(void**, handle, 0)[0]);
        result = release(handle, nullptr, 1);
        CSF_AT(void*, self, 0x8) = nullptr;
    }

    void* elements = CSF_AT(void*, self, 0x0);
    if (elements) {
        // Array allocation carries a count cookie in the dword just before the elements.
        uint8_t* block = static_cast<uint8_t*>(elements) - 4;
        uint32_t elementCount = *reinterpret_cast<uint32_t*>(block);
        if (elementCount != 0) {
            using ElementDeleteFn = int(__fastcall*)(void*, void*, int);
            auto deleteElements = reinterpret_cast<ElementDeleteFn>(CSF_AT(void**, elements, 0)[0]);
            result = deleteElements(elements, nullptr, 3);
        } else {
            result = Mem_Free(block);
        }
        CSF_AT(void*, self, 0x0) = nullptr;
    }

    return result;
}

HOOK(0x526CA0, AudioSoundManager_ReleaseHandles, "auto");
