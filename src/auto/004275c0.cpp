#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static hook::Tail __fastcall HudObjectives_RenderAttachedOverlay(
    void* self, const uint32_t* /*args*/)
{
    if (!CSF_AT(uint8_t, self, 0x08))
        return hook::tail_ret(0);
    if (!CSF_AT(uint8_t, self, 0x09))
        return hook::tail_ret(0);

    const uint32_t mode = CSF_AT(uint32_t, self, 0x00);
    void* child;

    switch (mode)
    {
    case 0:
        child = CSF_AT(void*, self, 0x20);
        break;

    case 1:
    case 2:
    case 3:
        child = CSF_AT(void*, self, 0x18);
        if (child)
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x08 / 4])(child, nullptr);

        child = CSF_AT(void*, self, 0x1C);
        if (child)
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x90 / 4])(child, nullptr);

        child = CSF_AT(void*, self, 0x0C);
        if (child)
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x08 / 4])(child, nullptr);

        child = CSF_AT(void*, self, 0x10);
        if (child)
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, child, 0)[0x08 / 4])(child, nullptr);

        if (mode == 1)
        {
            child = CSF_AT(void*, self, 0x14);
            if (child)
                ((void(__fastcall*)(void*, void*))
                    CSF_AT(void**, child, 0)[0x08 / 4])(child, nullptr);

            child = CSF_AT(void*, self, 0x20);
        }
        else
        {
            child = CSF_AT(void*, self, 0x14);
        }
        break;

    default:
        return hook::tail_ret(mode);
    }

    if (!child)
        return hook::tail_ret(0);

    return hook::tail_jump(
        child, (*reinterpret_cast<void***>(child))[0x08 / 4]);
}

HOOK_TAIL(0x004275C0, HudObjectives_RenderAttachedOverlay, "auto");
