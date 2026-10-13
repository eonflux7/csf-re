#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(char, __fastcall, C_0083eb70_TestPairLink, 0x004E60B0, (C_0083eb70* self, void* , int a2));
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int kind));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall S_004f0cd0_ClassifyObjectAction(
    S_004f0cd0* self, void* /*edx*/, std::uint32_t* action)
{
    const std::uint32_t actionKind = action[1];
    action[0] = -1;

    auto* object = reinterpret_cast<C_008342e8*>(self->field_0008);
    const std::uint32_t objectKind = CSF_AT(std::uint32_t, object, 0x1D8);

    switch (actionKind)
    {
    case 0:
    case 1:
        if ((CSF_AT(std::uint8_t, object, 0x1F0) & 0x40) == 0 &&
            objectKind != 3 && objectKind != 2 && objectKind != 1)
        {
            action[0] = 0;
            const int resolved = CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(action + 2));

            if (resolved &&
                CSF_AT(std::uint32_t, reinterpret_cast<void*>(resolved), 0x50) == 21)
            {
                auto* currentObject =
                    reinterpret_cast<C_008342e8*>(self->field_0008);
                auto* link =
                    reinterpret_cast<C_0083eb70*>(currentObject->field_003c);
                if (link && C_0083eb70_TestPairLink(
                                link, nullptr,
                                reinterpret_cast<int>(
                                    reinterpret_cast<std::uint8_t*>(resolved) + 0x188)))
                {
                    action[0] = -1;
                }
            }
        }
        break;

    case 2:
    case 3:
        if (objectKind != 2)
            action[0] = 1;
        break;

    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
        action[0] = 2;
        break;

    default:
        break;
    }

    auto* currentObject = reinterpret_cast<C_008342e8*>(self->field_0008);
    if (static_cast<std::uint8_t>(
            IsAlternateControlKind(currentObject->field_0050)) &&
        action[0] == 0)
    {
        action[0] = -1;
    }

    return action[0] != static_cast<std::uint32_t>(-1);
}

HOOK(0x004F0CD0, S_004f0cd0_ClassifyObjectAction, "auto");
