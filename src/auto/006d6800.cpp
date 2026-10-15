#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __cdecl, RpToonInkSetThickness, 0x007FAFD0, (float* a1, float a2));
CSF_FUNC(int, __cdecl, Gfx2d_RefreshTeamSelection, 0x007FB060, (int a1));
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_PrepareTeamUpdate, 0x007FB800, (uint32_t* a1));
CSF_FUNC(int, __cdecl, _rpTeamOptimizePatch, 0x007FB970, (int a1));
CSF_FUNC(int, __cdecl, RpTeamOptimize, 0x007FB990, (int a1));
CSF_FUNC(int, __cdecl, RpTeamUpdate, 0x007FD880, (int a1, float a2));
CSF_FUNC(int, __cdecl, _rpTeamPlayerSelect, 0x007FEE60, (uint32_t* a1, int a2));

// The supplied callee disassembly confirms three stack arguments and ret 0.
// Its first ECX read follows "mov cx, [edi + 0x20]": this is not a this pointer.
// A __fastcall binding with three stack arguments would corrupt ESP.
CSF_FUNC(uint32_t*, __cdecl, Gfx2d_SetTeamPlayerState, 0x007FEFA0,
         (uint32_t* a1, int a2, int16_t a3));

CSF_FUNC(int, __cdecl, Rt2dSceneGetChildCount, 0x008013C0, (int a1));

static int __fastcall S_006d6800_UpdateTeam(
    S_006d6800* self, void* /*edx*/, float thickness)
{
    const int team = Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));

    for (uint32_t index = 0; index < self->field_005c; ++index)
    {
        void* child = reinterpret_cast<void**>(self->field_0054)[index];

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr)
            && !((bool(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[0x30 / 4])(child, nullptr))
        {
            void* status =
                ((void*(__fastcall*)(void*, void*))
                     CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr);

            if (CSF_AT(uint8_t, status, 5))
            {
                void* player = CSF_AT(void*, child, 4);
                Gfx2d_SetTeamPlayerState(
                    CSF_AT(uint32_t*, player, 0x24),
                    reinterpret_cast<int>(player), 0);
            }
        }
    }

    RpToonInkSetThickness(reinterpret_cast<float*>(team), thickness);
    Gfx2d_RefreshTeamSelection(team);

    bool selectionChanged = false;
    void* limits =
        ((void*(__fastcall*)(void*, void*))
             reinterpret_cast<void**>(self->field_0000)[0x5c / 4])(
            self, nullptr);
    int remaining = static_cast<int>(
        CSF_AT(uint32_t, self, 0x08) - CSF_AT(uint32_t, limits, 4));

    for (uint32_t index = 0; index < self->field_005c; ++index)
    {
        void* child = reinterpret_cast<void**>(self->field_0054)[index];

        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, child, 0)[0x28 / 4])(child, nullptr))
            continue;

        if (((bool(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x30 / 4])(child, nullptr))
            continue;

        void* status =
            ((void*(__fastcall*)(void*, void*))
                 CSF_AT(void**, child, 0)[0x18 / 4])(child, nullptr);
        if (!CSF_AT(uint8_t, status, 5))
            continue;

        void* player = CSF_AT(void*, child, 4);
        if (!(CSF_AT(uint8_t, player, 8) & 0x40))
            continue;

        void* entry = CSF_AT(void*, player, 0x2c);
        while (entry)
        {
            if (CSF_AT(uint16_t, entry, 8) != 0xFFFF)
            {
                void* candidate = CSF_AT(void*, entry, 4);
                uint32_t* candidateTeam =
                    CSF_AT(uint32_t*, candidate, 0x24);

                bool selected = false;
                if (candidateTeam)
                {
                    const uint32_t playerIndex =
                        CSF_AT(uint16_t, candidate, 0x20);
                    uint32_t* selectionBits =
                        CSF_AT(uint32_t*, candidateTeam, 0x60);
                    selected =
                        (selectionBits[playerIndex >> 5]
                         & (uint32_t{1} << (playerIndex & 0x1F))) != 0;
                }

                if (!selected)
                {
                    if (remaining > 0)
                    {
                        _rpTeamPlayerSelect(
                            candidateTeam, reinterpret_cast<int>(candidate));
                        --remaining;
                    }
                    else
                    {
                        void* currentPlayer = CSF_AT(void*, child, 4);
                        Gfx2d_SetTeamPlayerState(
                            CSF_AT(uint32_t*, currentPlayer, 0x24),
                            reinterpret_cast<int>(currentPlayer), 2);
                        selectionChanged = true;
                    }
                }
            }

            entry = CSF_AT(void*, entry, 0x10);
        }
    }

    if (selectionChanged)
        Gfx2d_RefreshTeamSelection(team);

    RpTeamOptimize(team);
    Gfx2d_PrepareTeamUpdate(reinterpret_cast<uint32_t*>(team));
    _rpTeamOptimizePatch(team);

    const int updateTeam =
        Rt2dSceneGetChildCount(static_cast<int>(self->field_003c));
    return RpTeamUpdate(updateTeam, thickness);
}

HOOK(0x006D6800, S_006d6800_UpdateTeam, "auto");
