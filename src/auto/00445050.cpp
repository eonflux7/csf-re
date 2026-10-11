#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int kind));
CSF_FUNC(int, __cdecl, GameConfig_IsResolvedEntryActive, 0x00755880, (int entry));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall ApplyColorAnimationToControl(
    uint32_t* self, void* /*edx*/, C_00833908* animation, uint32_t* entry)
{
    const uint32_t count = CSF_AT(uint32_t, self, 0x5C);
    uint32_t index = 0;
    bool found = false;

    if (count != 0)
    {
        auto* cursor = CSF_AT(uint8_t*, self, 0x54) + 4;
        const uint32_t target = animation->field_0020;
        while (index < count)
        {
            if (CSF_AT(uint32_t, cursor, 0) == target)
            {
                found = index != UINT32_MAX;
                break;
            }
            ++index;
            cursor += 12;
        }
    }

    if (!found)
    {
        C_00833908_GetTargetName(animation, nullptr);
    }
    else
    {
        const uint32_t kind = animation->field_0050;
        if (kind == 2 || kind == 3)
            --CSF_AT(uint32_t, self, 0x08);
        else if (kind == 1)
            --CSF_AT(uint32_t, self, 0x0C);

        const uint32_t remaining = CSF_AT(uint32_t, self, 0x5C) - 1;
        CSF_AT(uint32_t, self, 0x5C) = remaining;
        if (index != remaining)
        {
            auto* destination = CSF_AT(uint8_t*, self, 0x54) + 12 * index;
            _memcpy(destination, destination + 12, 12 * (remaining - index));
        }
    }

    int result = CSF_AT(int, entry, 0x08);
    if (result < 0)
        return result;

    result = CHandleTable_Resolve(
        g_pCurrentScene, nullptr,
        static_cast<int>(reinterpret_cast<uintptr_t>(entry)));
    if (result == 0)
        return result;

    void* control = reinterpret_cast<void*>(static_cast<uintptr_t>(
        static_cast<uint32_t>(result)));
    result = CSF_AT(int, control, 0x50);

    bool isMenuContext = false;
    if (result == 0)
    {
        result = static_cast<int>(GetSceneMenuContext(g_pCurrentScene, nullptr));
        isMenuContext = static_cast<uint32_t>(result) ==
                        reinterpret_cast<uintptr_t>(control);
    }
    if (!isMenuContext && CSF_AT(uint32_t, control, 0x50) != 19)
        return result;

    result = IsAlternateControlKind(static_cast<int>(animation->field_0050));
    if (static_cast<uint8_t>(result) != 0)
        return result;

    result = GameConfig_IsResolvedEntryActive(static_cast<int>(animation->field_0050));
    if (static_cast<uint8_t>(result) != 0)
        return result;

    if (animation->field_0078 != 1)
        return 1;

    uint32_t priority = CSF_AT(uint32_t, entry, 0x24);
    if (priority == 0 &&
        CSF_AT(uint32_t, entry, 0x14) == 1 &&
        CSF_AT(uint32_t, entry, 0x0C) != 9)
    {
        priority = 2;
    }

    if (CSF_AT(uint32_t, entry, 0x0C) == 10 && priority != 1)
        ++CSF_AT(uint32_t, self, 0x48);

    ++CSF_AT(uint32_t, self, 0x10);
    switch (animation->field_007c)
    {
        case 1: ++CSF_AT(uint32_t, self, 0x18); break;
        case 2: ++CSF_AT(uint32_t, self, 0x20); break;
        case 3: ++CSF_AT(uint32_t, self, 0x28); break;
        case 4: ++CSF_AT(uint32_t, self, 0x30); break;
        case 5: ++CSF_AT(uint32_t, self, 0x38); break;
        default: break;
    }

    if (priority == 1)
    {
        ++CSF_AT(uint32_t, self, 0x3C);
        return 0;
    }
    if (priority == 2)
    {
        const uint32_t updated = CSF_AT(uint32_t, self, 0x40) + 1;
        CSF_AT(uint32_t, self, 0x40) = updated;
        return static_cast<int>(updated);
    }
    return static_cast<int>(priority - 2);
}

HOOK(0x00445050, ApplyColorAnimationToControl, "auto");
