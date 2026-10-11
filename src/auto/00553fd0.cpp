#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

CSF_FUNC(unsigned int, __cdecl, Hud_UnregisterObject, 0x004754D0, (int a1));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(bool, __fastcall, Character_748380, 0x00748380, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, ConsumeFlagByteAndClear, 0x007483D0, (void* self, void*));

static uint32_t** __fastcall C_00844694_ClearSpawnResources(
    C_00844694* self, void* /*edx*/)
{
    void* selectionState = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
    if (selectionState)
    {
        const uint32_t characterInterface = CSF_AT(uint32_t, selectionState, 0x4);
        if (characterInterface)
        {
            const uint32_t characterAddress = characterInterface - 776;
            if (characterAddress)
            {
                void* character = reinterpret_cast<void*>(characterAddress);
                void* interfaceObject = reinterpret_cast<void*>(characterInterface);
                if (((bool(__fastcall*)(void*, void*))
                        CSF_AT(void**, interfaceObject, 0)[2])(
                            interfaceObject, nullptr))
                {
                    Character_748380(character, nullptr, 0);
                }
                ConsumeFlagByteAndClear(character, nullptr);
                selectionState =
                    reinterpret_cast<void*>(g_CharacterSelectState8B0694);
            }
        }
    }

    void* object = reinterpret_cast<void*>(self->field_1474);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_1474 = 0;
        selectionState = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
    }

    object = reinterpret_cast<void*>(self->field_0c88);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_0c88 = 0;
        selectionState = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
    }

    if (selectionState)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, selectionState, 0)[0])(selectionState, nullptr, 1);
        g_CharacterSelectState8B0694 = 0;
    }

    object = reinterpret_cast<void*>(self->field_1484);
    if (object)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
        self->field_1484 = 0;
    }

    Hud_UnregisterObject(static_cast<int>(reinterpret_cast<std::uintptr_t>(self)));

    void* buffer = reinterpret_cast<void*>(self->field_1490);
    self->field_1494 = 0;
    self->field_1498 = 0;
    if (buffer)
    {
        Mem_Free(buffer);
        self->field_1490 = 0;
    }

    uint32_t** result = reinterpret_cast<uint32_t**>(self->field_1468);
    void* node = CSF_AT(void*, result, 0);
    if (node != result)
    {
        do
        {
            void* entry = CSF_AT(void*, node, 0x8);
            bool removeNode = false;
            if (entry)
            {
                result = CSF_AT(uint32_t**, entry, 0x4);
                if (result)
                {
                    result = reinterpret_cast<uint32_t**>(
                        reinterpret_cast<std::uintptr_t>(result) -
                        85 * sizeof(uint32_t));
                    removeNode = result != nullptr;
                }
            }

            if (removeNode)
            {
                result = reinterpret_cast<uint32_t**>(node);
                const bool isSentinel =
                    node == reinterpret_cast<void*>(self->field_1468);
                node = CSF_AT(void*, node, 0);
                if (!isSentinel)
                {
                    CSF_AT(void*, CSF_AT(void*, result, 0x4), 0) =
                        CSF_AT(void*, result, 0);
                    CSF_AT(void*, CSF_AT(void*, result, 0), 0x4) =
                        CSF_AT(void*, result, 0x4);
                    Mem_FreeDebug(result);
                    const uint32_t count = self->field_146c - 1;
                    self->field_146c = count;
                    result = reinterpret_cast<uint32_t**>(count);
                }
            }
            else
            {
                node = CSF_AT(void*, node, 0);
            }

            if (entry)
            {
                result = ((uint32_t**(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, entry, 0)[0])(entry, nullptr, 1);
            }
        }
        while (node != reinterpret_cast<void*>(self->field_1468));
    }
    return result;
}

HOOK(0x00553FD0, C_00844694_ClearSpawnResources, "auto");
