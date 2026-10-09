#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_008b0678, 0x008B0678);
CSF_GLOBAL(void*, g_pMenuManager, 0x008B0570);

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void*));
CSF_FUNC(void, __fastcall, sub_5228C0, 0x005228C0, (void* self, void* edx, float a2, float a3));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(int, __fastcall, sub_525160, 0x00525160, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_5269B0, 0x005269B0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int*, __fastcall, sub_526CF0, 0x00526CF0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(unsigned int, __fastcall, sub_526F70, 0x00526F70, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static void __fastcall CSoundManager_ClearSounds(CSoundManager* self, void* /*edx*/)
{
    if (g_008b0678)
        sub_525160(reinterpret_cast<void*>(Get_008b0678()), nullptr);

    // The first three words also provide the full buffer passed for slot sounds.
    unsigned int slotState[3] = {};
    unsigned int sound = 0;

    if (self->field_0018)
    {
        uint32_t offset = 0;
        slotState[2] = self->field_0018;
        do
        {
            sound = CSF_AT(unsigned int,
                reinterpret_cast<void*>(self->field_0010 + offset), 4);
            if (sound)
            {
                void* object = reinterpret_cast<void*>(sound);
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);
                object = reinterpret_cast<void*>(sound);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr, 0);
                sub_526CF0(reinterpret_cast<void*>(self->m_instancePool), nullptr, &sound);
            }
            offset += 12;
            --slotState[2];
        } while (slotState[2]);
    }

    uint32_t allocation = self->field_0010;
    self->field_0014 = 0;
    self->field_0018 = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_0010 = 0;
    }

    if (self->field_0028)
    {
        uint32_t offset = 0;
        slotState[2] = self->field_0028;
        do
        {
            sound = CSF_AT(unsigned int,
                reinterpret_cast<void*>(self->field_0020 + offset), 4);
            if (sound)
            {
                void* object = reinterpret_cast<void*>(sound);
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);
                object = reinterpret_cast<void*>(sound);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr, 0);
                sub_526CF0(reinterpret_cast<void*>(self->m_instancePool), nullptr, &sound);
            }
            offset += 24;
            --slotState[2];
        } while (slotState[2]);
    }

    allocation = self->field_0020;
    self->field_0024 = 0;
    self->field_0028 = 0;
    if (allocation)
    {
        uint32_t count = CSF_AT(uint32_t, reinterpret_cast<void*>(allocation - 4), 0);
        uint32_t cursor = allocation + 24 * count;
        if (static_cast<int32_t>(count - 1) >= 0)
        {
            cursor += 8;
            do
            {
                cursor -= 24;
                --count;
                CSF_AT(void**, reinterpret_cast<void*>(cursor), 0) = CSF_VTBL(0x0082D410);
            } while (count);
        }
        Mem_Free(reinterpret_cast<void*>(allocation - 4));
        self->field_0020 = 0;
    }

    if (self->field_0038)
    {
        uint32_t offset = 0;
        slotState[2] = self->field_0038;
        do
        {
            sound = CSF_AT(unsigned int,
                reinterpret_cast<void*>(self->field_0030 + offset), 4);
            if (sound)
            {
                void* object = reinterpret_cast<void*>(sound);
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);
                object = reinterpret_cast<void*>(sound);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr, 0);
                sub_526CF0(reinterpret_cast<void*>(self->m_instancePool), nullptr, &sound);
            }
            offset += 24;
            --slotState[2];
        } while (slotState[2]);
    }

    allocation = self->field_0030;
    self->field_0034 = 0;
    self->field_0038 = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->field_0030 = 0;
    }

    if (self->field_0044)
    {
        void* object = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);
        object = reinterpret_cast<void*>(self->field_0044);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr, 0);
        object = reinterpret_cast<void*>(self->field_0044);
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            self->field_0044 = 0;
        }
    }

    if (self->m_musicStream)
    {
        void* object = reinterpret_cast<void*>(self->m_musicStream);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);
        object = reinterpret_cast<void*>(self->m_musicStream);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, object, 0)[0x50 / 4])(object, nullptr, 0);
        object = reinterpret_cast<void*>(self->m_musicStream);
        if (object)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 1);
            self->m_musicStream = 0;
        }
    }

    if (self->field_0054)
    {
        slotState[2] = 0;
        slotState[1] = self->field_0054;
        do
        {
            void* slot = reinterpret_cast<void*>(self->m_slots + slotState[2]);
            if (CSF_AT(uint8_t, slot, 0))
            {
                void* instance = CSF_AT(void*, slot, 8);
                slotState[0] = CSF_AT(unsigned int, instance, 0x60);
                if (slotState[0])
                {
                    void* object = reinterpret_cast<void*>(slotState[0]);
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0x2C / 4])(object, nullptr);
                    sub_5269B0(CSF_AT(void*, slot, 8), nullptr, 0);
                    sub_526CF0(reinterpret_cast<void*>(self->m_instancePool), nullptr, slotState);
                }
            }

            void* instance = CSF_AT(void*, slot, 8);
            if (instance)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, instance, 0)[0])(instance, nullptr, 1);
                CSF_AT(uint32_t, slot, 8) = 0;
            }
            slotState[2] += 12;
            --slotState[1];
        } while (slotState[1]);
    }

    allocation = self->m_slots;
    self->field_0050 = 0;
    self->field_0054 = 0;
    if (allocation)
    {
        Mem_Free(reinterpret_cast<void*>(allocation));
        self->m_slots = 0;
    }

    if (g_pMenuManager)
    {
        int state = GetGlobalState_50D190();
        MenuSystem_ToggleChildFlag(reinterpret_cast<void*>(state), nullptr);
    }

    if (self->m_instancePool)
        sub_526F70(reinterpret_cast<void*>(self->m_instancePool), nullptr);

    const bool restoreVolumes = self->field_0078 != 0;
    self->field_0079 = 0;
    if (restoreVolumes)
    {
        self->field_0078 = 0;
        sub_5228C0(self, nullptr, 1.0, 1.0);
    }
}

HOOK(0x005233D0, CSoundManager_ClearSounds, "auto");
