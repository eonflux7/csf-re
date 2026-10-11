#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, C_00834598_CompareRegion, 0x004A5930, (unsigned int* a1, uint32_t* a2));
CSF_FUNC(unsigned int, __fastcall, C_00834598_ApplyState, 0x004A5960, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, C_00834598_HandleNoState, 0x004A5C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());

static uint32_t __fastcall C_00834598_Update(C_00834598* self, void* /*edx*/)
{
    int slotResult = self->vSlot64();
    if (slotResult == -1)
        return (uint32_t)slotResult;

    uint8_t* state = reinterpret_cast<uint8_t*>(self->vSlot68());
    if (CSF_AT(uint32_t, state, 0x4C) == 0)
        return (uint32_t)(uintptr_t)C_00834598_HandleNoState(self, nullptr);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    void** obj = reinterpret_cast<void**>(CSF_AT(uint32_t, owner, 0x44));
    if (obj)
    {
        void* sub = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0xD8 / 4])(obj, nullptr);
        if (sub)
        {
            void* sub2 = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0xD8 / 4])(obj, nullptr);
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, sub2, 0)[0x30 / 4])(sub2, nullptr, 0);
        }
    }

    if (self->field_008c)
    {
        if ((uint8_t)C_00834598_CompareRegion(
                reinterpret_cast<unsigned int*>(reinterpret_cast<uint8_t*>(self) + 0x90),
                reinterpret_cast<uint32_t*>(state + 0x38)))
        {
            int global = GetGlobalInt();
            int controller = SceneLoader_GetSceneController(reinterpret_cast<void*>(global), nullptr);
            ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, controller, 0)[0x18 / 4])(
                reinterpret_cast<void*>(controller), nullptr, self->field_008c);
            self->field_008c = 0;
        }
    }

    if (self->field_008c == 0)
        return C_00834598_ApplyState(self, nullptr, (int)(intptr_t)state);
    return self->field_008c;
}

HOOK(0x004A5D20, C_00834598_Update, "auto");
