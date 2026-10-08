#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __fastcall, CMenuPlayerInfo_RefreshPlayerControls, 0x0055D880,
         (CMenuPlayerInfo* self, void* edx));

static int __fastcall CMenuPlayerInfo_HandleControlEvent(
    CMenuPlayerInfo* self, void* /*edx*/, int event)
{
    void* eventObject = reinterpret_cast<void*>(event);
    const uint32_t requestControl = self->field_0020;

    if (!requestControl || requestControl != CSF_AT(uint32_t, eventObject, 0x04))
    {
        const uint32_t voiceControl = self->field_0024;
        if (voiceControl && voiceControl == CSF_AT(uint32_t, eventObject, 0x04))
        {
            void* context = reinterpret_cast<void*>(GetGlobal8B0688());
            void* player = reinterpret_cast<void*>(self->field_0048);
            void* voiceService = CSF_AT(void*, context, 0x1484);
            uint32_t playerId = CSF_AT(uint32_t, player, 0x204);

            const uint8_t isMuted =
                reinterpret_cast<uint8_t(__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, voiceService, 0)[6])(
                        voiceService, nullptr, playerId);

            context = reinterpret_cast<void*>(GetGlobal8B0688());
            player = reinterpret_cast<void*>(self->field_0048);
            voiceService = CSF_AT(void*, context, 0x1484);
            playerId = CSF_AT(uint32_t, player, 0x204);

            if (isMuted)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, voiceService, 0)[12])(
                        voiceService, nullptr, playerId);
            }
            else
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, voiceService, 0)[11])(
                        voiceService, nullptr, playerId);
            }

            CMenuPlayerInfo_RefreshPlayerControls(self, nullptr);
        }
    }

    return reinterpret_cast<int(__fastcall*)(CMenuPlayerInfo*, void*, int)>(
        CSF_AT(void**, self, 0)[18])(self, nullptr, event);
}

HOOK(0x0055DC00, CMenuPlayerInfo_HandleControlEvent, "auto");
