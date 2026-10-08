#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees, bound by address.
CSF_FUNC(int, __cdecl, GetGlobalState, 0x50D190, ());
CSF_FUNC(bool, __fastcall, MenuMpCharSelect_ShouldShowPreview, 0x542850, (void* self, void* edx));
CSF_FUNC(void, __fastcall, MenuMpCharacterSelect_ReleaseServerInfoChild, 0x5426C0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x54ECE0, ());
CSF_FUNC(int, __fastcall, MenuMpCharSelect_RenderCameraPreview, 0x550980, (void* self, void* edx, float a2));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x550CC0, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x5515F0, ());
CSF_FUNC(int, __fastcall, MenuMpCharSelect_CreateServerInfoChild, 0x542640, (void* self, void* edx, char a2, int a3));

// Per-frame update of the MP character select menu: toggles the preview flag,
// creates or releases the server-info child depending on the owner's state,
// and forwards the update to the camera-preview renderer.
static int __fastcall MenuMpCharSelect_Update(void* self, void* edx, float a2)
{
    int result = GetGlobalState();
    if (*reinterpret_cast<uint8_t*>(result + 208))
    {
        if (CSF_AT(uint8_t, self, 8) && MenuMpCharSelect_ShouldShowPreview(self, nullptr))
        {
            CSF_AT(uint8_t, self, 8) = 0;
            CSF_AT(uint8_t, self, 9) = 1;
        }

        uint8_t* owner = CSF_AT(uint8_t*, self, 0);
        if (owner[0x184] || owner[0x17E])
        {
            if (CSF_AT(int, self, 4) == 0)
            {
                GetGlobal8B0688();
                int mpGameModeIndex = GetMpGameModeIndex();
                MenuMpCharSelect_CreateServerInfoChild(self, nullptr, 0, mpGameModeIndex);
            }
        }
        else if (CSF_AT(int, self, 4) != 0)
        {
            MenuMpCharacterSelect_ReleaseServerInfoChild(self, nullptr);
        }

        result = GetGlobalValue();
        if (result)
        {
            // The second GetGlobalValue() result becomes `this` for the renderer.
            void* renderTarget = reinterpret_cast<void*>(GetGlobalValue());
            return MenuMpCharSelect_RenderCameraPreview(renderTarget, nullptr, a2);
        }
    }
    return result;
}

HOOK(0x5428A0, MenuMpCharSelect_Update, "auto");
