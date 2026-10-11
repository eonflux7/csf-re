#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_InitializeSceneObjectOptions, 0x004777D0, (uint32_t* self, void*));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static unsigned int __fastcall C_00834598_CreateSceneObject(C_00834598* self, void* /*edx*/, void* a2)
{
    // Frame region from ebp-0xD0 up to ebp-0x18: options (4 bytes) + storage (starts at +4)
    alignas(8) uint8_t frameBytes[0xB8];
    alignas(8) uint8_t queryBytes[0x18];

    uint8_t* args = reinterpret_cast<uint8_t*>(a2);
    uint8_t* nameStr = reinterpret_cast<uint8_t*>(self) + 0x90;

    AssignStringSubstring(nameStr, nullptr, args + 0x38, 0, (std::uint32_t)-1);

    uint8_t* chars;
    if (CSF_AT(uint32_t, nameStr, 0x18) < 0x10u)
        chars = nameStr + 4;
    else
        chars = CSF_AT(uint8_t*, nameStr, 4);

    CSF_AT(uint8_t*, queryBytes, 0) = chars;
    CSF_AT(uint8_t, queryBytes, 4) = 1;
    CSF_AT(int, queryBytes, 8) = 0;
    CSF_AT(int, queryBytes, 0xC) = 0;
    CSF_AT(int, queryBytes, 0x10) = 0;
    CSF_AT(int, queryBytes, 0x14) = 0;

    int global = GetGlobalInt();
    int controller = SceneLoader_GetSceneController(reinterpret_cast<void*>(global), nullptr);
    unsigned int result = ((unsigned int(__fastcall*)(void*, void*, void*))
                           CSF_AT(void**, reinterpret_cast<void*>(controller), 0)[5])(
        reinterpret_cast<void*>(controller), nullptr, queryBytes);
    self->field_008c = result;

    if (result != 0) {
        WeaponFirearms_InitializeSceneObjectOptions(reinterpret_cast<uint32_t*>(frameBytes), nullptr);

        void* sceneObj = reinterpret_cast<void*>(self->field_008c);
        CSF_AT(uint32_t, frameBytes, 0) = (CSF_AT(uint32_t, frameBytes, 0) & 0xFFFFFE3Eu) | 0x140u;
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, sceneObj, 0)[4])(
            sceneObj, nullptr, frameBytes);

        void* owner = reinterpret_cast<void*>(self->field_0008);
        void* target = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr);

        int mode = CSF_AT(int, args, 0x54);
        int priority = (mode != 1) ? 40 : 50;
        ((void(__fastcall*)(void*, void*, uint32_t, int, int))CSF_AT(void**, target, 0)[40])(
            target, nullptr, self->field_008c, priority, 0);

        if (CSF_AT(uint32_t, frameBytes, 0x90) >= 0x10u) {
            void* block = CSF_AT(void*, frameBytes, 0x7C);
            if (block != nullptr)
                Mem_FreeDebug(block);
        }
        CSF_AT(uint32_t, frameBytes, 0x90) = 0xF;
        CSF_AT(int, frameBytes, 0x8C) = 0;
        CSF_AT(uint8_t, frameBytes, 0x7C) = 0;
        return DestroySceneObjectOptionsStorage(frameBytes + 4, nullptr);
    }
    return result;
}

HOOK(0x004A5960, C_00834598_CreateSceneObject, "auto");
