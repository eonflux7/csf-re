#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_Value_8BB450, 0x008BB450);
CSF_GLOBAL(uint32_t, g_BindingCount, 0x008600D8);
CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

CSF_FUNC(void*, __fastcall, Cheats_ReserveBindingChanges, 0x00428D60, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, Sub_695680, 0x00695680, (char* a1, char* a2));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));
CSF_FUNC(int, __fastcall, Cheats_ApplyBindingChanges, 0x006AF8B0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));

static C_00860130* __fastcall C_008300c0_ApplyControlBindings(
    C_008300c0* self, void* /*edx*/)
{
    if (!g_Value_8BB450)
        return nullptr;

    alignas(8) uint8_t snapshotBytes[0x2A8];
    alignas(8) uint8_t textBytes[0x20];
    alignas(8) uint8_t lookupBytes[0x1C];
    alignas(8) uint8_t changesBytes[0x10];

    CSF_AT(void*, lookupBytes, 0x00) = nullptr;
    CSF_AT(uint32_t, lookupBytes, 0x04) = 0;
    CSF_AT(uint32_t, lookupBytes, 0x08) = 0;
    CSF_AT(int, lookupBytes, 0x0C) = 10;

    ((void(__fastcall*)(void*, void*, void*))
        CSF_AT(void**, self, 0)[0x4C / 4])(self, nullptr, lookupBytes);

    for (int i = 0; i < 32; ++i)
    {
        uint8_t* binding = snapshotBytes + i * 20;
        CSF_AT(int, binding, 0x00) = -1;
        CSF_AT(int, binding, 0x04) = -1;
        CSF_AT(int, binding, 0x08) = 0;
        CSF_AT(float, binding, 0x0C) = 1.0f;
        CSF_AT(int, binding, 0x10) = 0;
    }

    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, self, 0)[0x0C / 4])(self, nullptr, snapshotBytes, 0);

    uint32_t requestedCount = g_BindingCount;
    CSF_AT(void*, changesBytes, 0x00) = nullptr;
    CSF_AT(uint32_t, changesBytes, 0x04) = 0;
    CSF_AT(uint32_t, changesBytes, 0x08) = 0;
    CSF_AT(int, changesBytes, 0x0C) = 10;

    uint32_t initializationCount = requestedCount;
    if (requestedCount > 0)
    {
        Cheats_ReserveBindingChanges(changesBytes, nullptr,
                                     static_cast<int>(requestedCount));
        initializationCount = g_BindingCount;
    }
    CSF_AT(uint32_t, changesBytes, 0x08) = requestedCount;

    if (initializationCount > 0)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        do
        {
            uint8_t* entry =
                static_cast<uint8_t*>(CSF_AT(void*, changesBytes, 0x00)) + offset;
            CSF_AT(int, entry, 0x00) = -1;
            CSF_AT(uint16_t, entry, 0x04) = 0;

            entry = static_cast<uint8_t*>(CSF_AT(void*, changesBytes, 0x00)) + offset;
            CSF_AT(uint32_t, entry, 0x00) = index++;
            offset += 516;
        } while (index < g_BindingCount);
    }

    uint32_t lookupCount = CSF_AT(uint32_t, lookupBytes, 0x08);
    CSF_AT(uint32_t, textBytes, 0x18) = 7;
    CSF_AT(uint32_t, textBytes, 0x14) = 0;
    CSF_AT(uint16_t, textBytes, 0x04) = 0;

    uint8_t* oldBinding = snapshotBytes;
    uint8_t* newBinding = reinterpret_cast<uint8_t*>(self) + 4;

    for (int remaining = 32; remaining != 0; --remaining)
    {
        int oldKey = CSF_AT(int, oldBinding, 0x00);
        int oldModifier = CSF_AT(int, oldBinding, 0x04);
        int newKey = CSF_AT(int, newBinding, 0x00);
        int newModifier = CSF_AT(int, newBinding, 0x04);

        int oldIndex = -1;
        if (lookupCount)
        {
            uint8_t* entry = static_cast<uint8_t*>(CSF_AT(void*, lookupBytes, 0x00));
            for (uint32_t i = 0; i < lookupCount; ++i, entry += 12)
            {
                if (CSF_AT(int, entry, 0x00) == oldKey &&
                    CSF_AT(int, entry, 0x04) == oldModifier)
                {
                    oldIndex = CSF_AT(int,
                        static_cast<uint8_t*>(CSF_AT(void*, lookupBytes, 0x00)) + 12 * i,
                        0x08);
                    break;
                }
            }
        }

        int newIndex = -1;
        if (lookupCount)
        {
            uint8_t* entry = static_cast<uint8_t*>(CSF_AT(void*, lookupBytes, 0x00));
            for (uint32_t i = 0; i < lookupCount; ++i, entry += 12)
            {
                if (CSF_AT(int, entry, 0x00) == newKey &&
                    CSF_AT(int, entry, 0x04) == newModifier)
                {
                    newIndex = CSF_AT(int,
                        static_cast<uint8_t*>(CSF_AT(void*, lookupBytes, 0x00)) + 12 * i,
                        0x08);
                    break;
                }
            }
        }

        if (oldIndex != -1)
        {
            int supportsRemapping =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, self, 0)[0x08 / 4])(self, nullptr);

            if (supportsRemapping && newIndex != -1)
            {
                if (oldIndex != newIndex)
                {
                    CSF_AT(int,
                        static_cast<uint8_t*>(CSF_AT(void*, changesBytes, 0x00)) +
                            516 * oldIndex,
                        0x00) = newIndex;
                }
            }
            else
            {
                uint32_t changeOffset = static_cast<uint32_t>(oldIndex) * 516;
                CSF_AT(int,
                    static_cast<uint8_t*>(CSF_AT(void*, changesBytes, 0x00)) + changeOffset,
                    0x00) = -1;

                TextInputSubobject_Init(textBytes, nullptr, &g_byte_82E65C);
                void* text =
                    ((void*(__fastcall*)(void*, void*, int, int, void*))
                        CSF_AT(void**, self, 0)[0x50 / 4])(
                            self, nullptr, newKey, newModifier, textBytes);

                char* characters;
                if (CSF_AT(uint32_t, text, 0x18) < 8u)
                    characters = static_cast<char*>(text) + 4;
                else
                    characters = CSF_AT(char*, text, 0x04);

                Sub_695680(
                    static_cast<char*>(CSF_AT(void*, changesBytes, 0x00)) +
                        changeOffset + 4,
                    characters);
            }
            lookupCount = CSF_AT(uint32_t, lookupBytes, 0x08);
        }

        oldBinding += 20;
        newBinding += 20;
    }

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* bindingStorage =
        ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, controls, 0)[0x68 / 4])(controls, nullptr);
    Cheats_ApplyBindingChanges(
        bindingStorage, nullptr, reinterpret_cast<uint32_t*>(changesBytes));

    if (CSF_AT(uint32_t, textBytes, 0x18) >= 8 &&
        CSF_AT(void*, textBytes, 0x04))
    {
        Mem_FreeDebug(CSF_AT(void*, textBytes, 0x04));
    }
    CSF_AT(uint32_t, textBytes, 0x18) = 7;
    CSF_AT(uint32_t, textBytes, 0x14) = 0;
    CSF_AT(uint16_t, textBytes, 0x04) = 0;

    if (CSF_AT(void*, changesBytes, 0x00))
    {
        Mem_Free(CSF_AT(void*, changesBytes, 0x00));
        CSF_AT(void*, changesBytes, 0x00) = nullptr;
    }

    if (CSF_AT(void*, lookupBytes, 0x00))
    {
        return reinterpret_cast<C_00860130*>(
            Mem_Free(CSF_AT(void*, lookupBytes, 0x00)));
    }
    return nullptr;
}

HOOK(0x00428EC0, C_008300c0_ApplyControlBindings, "auto");
