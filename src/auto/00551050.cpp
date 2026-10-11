#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(void, __cdecl, DestroyObjectStateTemporary, 0x0040ECF0, ());
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void*, __fastcall, Network_CreateMessage, 0x0073C050, (void* self, void* edx, int arg0, int a2));
CSF_FUNC(int, __fastcall, Network_LinkMessage, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Network_InitializeObjectState, 0x00747EE0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Scene_IsObjectRemovable, 0x007558C0, (int a1));

static int __stdcall MpSpawnPoints_QueueObjectState(int entryAddress)
{
    auto* entry = reinterpret_cast<uint8_t*>(entryAddress);
    auto* object = reinterpret_cast<uint8_t*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr,
                             reinterpret_cast<int>(entry + 8)));
    if (!object)
        return 0;

    alignas(4) uint8_t state[0x28]{};
    Network_InitializeObjectState(state, nullptr);

    void* identity = CSF_AT(void*, object, 0x2C);
    int objectType = CSF_AT(int, object, 0x50);
    CSF_AT(uint32_t, state, 0x1C) = CSF_AT(uint32_t, identity, 0);
    CSF_AT(uint16_t, state, 0x02) = static_cast<uint16_t>(objectType);

    if (static_cast<uint8_t>(Scene_IsObjectRemovable(objectType)))
        CSF_AT(uint16_t, state, 0) = CSF_AT(uint16_t, object, 0x194);
    else
        CSF_AT(uint16_t, state, 0) = 0xFFFF;

    CSF_AT(uint32_t, state, 0x04) = CSF_AT(
        uint32_t, reinterpret_cast<void*>(GetObjectPositionVector(object, nullptr)), 0);
    CSF_AT(uint32_t, state, 0x08) = CSF_AT(
        uint32_t, reinterpret_cast<void*>(GetObjectPositionVector(object, nullptr)), 4);
    CSF_AT(uint32_t, state, 0x0C) = CSF_AT(
        uint32_t, reinterpret_cast<void*>(GetObjectPositionVector(object, nullptr)), 8);

    int relatedHandle = -1;
    if (CSF_AT(int, entry, 0x18) != -1)
    {
        auto* relatedObject = reinterpret_cast<uint8_t*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                 reinterpret_cast<int>(entry + 0x14)));
        if (relatedObject)
        {
            auto* secondaryBase = CSF_AT(uint8_t*, relatedObject, 0x18);
            auto* owner = secondaryBase ? secondaryBase - 1712 : nullptr;
            relatedHandle = CSF_AT(int, owner, 0x2C);
            if ((relatedHandle & 0x3FFFFF) == 0)
            {
                Game_RaiseStatusNotice(0, 0, static_cast<uint32_t>(-536870898), 0);
                relatedHandle = 0;
            }
        }
    }
    CSF_AT(int, state, 0x20) = relatedHandle;

    if (CSF_AT(int, object, 0x50) == 11)
    {
        void* orientation = CSF_AT(void*, object, 0x4C);
        CSF_AT(float, state, 0x10) =
            orientation ? CSF_AT(float, orientation, 0x84) : g_fZero;
        CSF_AT(float, state, 0x14) =
            orientation ? CSF_AT(float, orientation, 0x88) : g_fZero;
        CSF_AT(float, state, 0x18) =
            orientation ? CSF_AT(float, orientation, 0x8C) : g_fZero;
    }

    void* manager = reinterpret_cast<void*>(GetGlobalDword_73C020());
    void* message = Network_CreateMessage(
        manager, nullptr, 12, reinterpret_cast<int>(state));

    int result = reinterpret_cast<int>(message);
    if (message)
        result = Network_LinkMessage(object + 0x14, nullptr,
                                     reinterpret_cast<int>(message));

    DestroyObjectStateTemporary();
    return result;
}

HOOK(0x00551050, MpSpawnPoints_QueueObjectState, "auto");
