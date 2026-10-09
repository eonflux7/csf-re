#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int*, __fastcall, MissionScript_ClearNameLists, 0x004696C0,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MissionScript_ReserveNameLists, 0x00469970,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_AppendNameList, 0x0046A2B0,
         (void* self, void* edx, uint32_t* a3));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060,
         (void* a1, int a2));

static int __stdcall MissionScript_SerializeNameLists(uint32_t* self,
                                                     int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const bool saving = CSF_AT(uint8_t, context, 0x14) != 0;
    const uint8_t updateExisting = CSF_AT(uint8_t, context, 0x15);
    uint32_t count = 0;

    if (saving)
    {
        const uint32_t begin = CSF_AT(uint32_t, self, 0x04);
        count = begin ? (CSF_AT(uint32_t, self, 0x08) - begin) >> 4 : 0;
    }

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, uint32_t*, int))
                       CSF_AT(void**, serializer, 0)[120 / 4])(
            serializer, nullptr, &count, contextAddress);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;

    if (failed)
    {
        CSF_AT(uint8_t, context, 0x00) = 1;
        return 1;
    }

    if (saving)
    {
        uint32_t limit = count;
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < limit)
        {
            void* entry = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x04) + offset);
            const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (ready)
            {
                CSF_AT(uint8_t, context, 0x15) = updateExisting;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Scene_SerializeNameList(entry, contextAddress);
                limit = count;
            }
            ++index;
            offset += 16;
        }
    }
    else if (updateExisting)
    {
        uint32_t index = 0;
        uint32_t offset = 0;
        while (index < count)
        {
            void* entry = reinterpret_cast<void*>(
                CSF_AT(uint32_t, self, 0x04) + offset);
            const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (ready)
            {
                CSF_AT(uint8_t, context, 0x15) = updateExisting;
                CSF_AT(uint8_t, context, 0x16) = 0;
                Scene_SerializeNameList(entry, contextAddress);
            }
            ++index;
            offset += 16;
        }
    }
    else
    {
        MissionScript_ClearNameLists(self, nullptr);
        MissionScript_ReserveNameLists(self, nullptr, count);

        uint32_t index = 0;
        if (count)
        {
            uint32_t offset = 0;
            uint32_t emptyEntry[4] = {0, 0, 0, 10};
            do
            {
                MissionScript_AppendNameList(self, nullptr, emptyEntry);
                void* entry = reinterpret_cast<void*>(
                    CSF_AT(uint32_t, self, 0x04) + offset);
                const bool ready = CSF_AT(uint8_t, context, 0x00) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (ready)
                {
                    CSF_AT(uint8_t, context, 0x15) = 0;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Scene_SerializeNameList(entry, contextAddress);
                }
                offset += 16;
                ++index;
            } while (index < count);
        }
    }

    if (CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        if (((bool(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, serializer, 0)[124 / 4])(
                serializer, nullptr, contextAddress))
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x0046A440, MissionScript_SerializeNameLists, "auto");
