#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the four sub-objects held by the loader at +4, +8, +12, +16.
// Flag byte at +0x14 selects the vtable slot used: slot 4 (offset 0x10) when set,
// slot 0 otherwise. The last object (+16) is forwarded to by a tail jump.
static hook::Tail __fastcall GameBddLoader_InitializeSceneResources(void* self, const uint32_t* args)
{
    (void)args;

    if (CSF_AT(uint8_t, self, 0x14) != 0)
    {
        if (CSF_AT(uint32_t, self, 0x04) != 0)
        {
            void* first = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, first, 0)[0x10 / 4])(first, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x08) != 0)
        {
            void* second = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, second, 0)[0x10 / 4])(second, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x0C) != 0)
        {
            void* third = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, third, 0)[0x10 / 4])(third, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x10) != 0)
        {
            void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
            return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x10 / 4]);
        }
        return hook::tail_ret(0);
    }
    else
    {
        if (CSF_AT(uint32_t, self, 0x04) != 0)
        {
            void* first = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x04));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, first, 0)[0])(first, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x08) != 0)
        {
            void* second = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, second, 0)[0])(second, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x0C) != 0)
        {
            void* third = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x0C));
            reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, third, 0)[0])(third, nullptr);
        }

        if (CSF_AT(uint32_t, self, 0x10) != 0)
        {
            void* obj = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10));
            return hook::tail_jump(obj, (*reinterpret_cast<void***>(obj))[0x00 / 4]);
        }
        return hook::tail_ret(0);
    }
}

HOOK_TAIL(0x0050BBB0, GameBddLoader_InitializeSceneResources, "options_file");
