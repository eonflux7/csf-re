#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(float*, __fastcall, SceneMissionSetup_SetBoundsCenter, 0x006AB5C0, (void* self, void* edx, float* a2));
CSF_FUNC(char, __fastcall, GrowEntryBuffer, 0x006E6B80, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GrowTwentyByteRecordArray, 0x006E7000, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, C_00863760_CopyFromDwordArray, 0x0070BAC0, (C_00863760* self, void* , uint32_t* a2));
CSF_FUNC(C_008637d8*, __fastcall, C_008637d8_ConstructFromValues, 0x0070C300, (C_008637d8* self, void* , uint32_t* sourceValues));

CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall C_008632c0_LoadRecords(
    C_008632c0* self, void* /*edx*/, uint32_t* source)
{
    uint32_t remaining = source[17];
    if (remaining)
    {
        void* entryArray = reinterpret_cast<uint8_t*>(self) + 0x28;
        if (remaining > CSF_AT(uint32_t, self, 0x2C))
        {
            GrowEntryBuffer(
                entryArray, nullptr,
                static_cast<int>(remaining - CSF_AT(uint32_t, self, 0x2C)));
        }

        uint32_t offset = 0;
        do
        {
            uint32_t* record = reinterpret_cast<uint32_t*>(source[15] + offset);
            uint32_t* object = nullptr;

            if (record[0] == 2)
            {
                auto* storage = reinterpret_cast<C_008637d8*>(
                    CoreMemory_AllocUntagged(40));
                if (storage)
                {
                    alignas(8) uint8_t valuesBytes[0x2C];
                    std::memset(valuesBytes, 0, 24);
                    object = reinterpret_cast<uint32_t*>(
                        C_008637d8_ConstructFromValues(
                            storage, nullptr,
                            reinterpret_cast<uint32_t*>(valuesBytes)));
                }

                reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*)>(
                    CSF_AT(void**, object, 0)[0x18 / 4])(
                        object, nullptr, record + 4);
                SceneMissionSetup_SetBoundsCenter(
                    reinterpret_cast<uint8_t*>(object) + 8, nullptr,
                    reinterpret_cast<float*>(record + 1));
            }
            else if (record[0] == 0)
            {
                auto* storage = reinterpret_cast<C_00863760*>(
                    CoreMemory_AllocUntagged(24));
                if (storage)
                {
                    alignas(8) uint8_t valuesBytes[0x20];
                    CSF_AT(float, valuesBytes, 0) = g_ZeroVectorX;
                    CSF_AT(float, valuesBytes, 4) = g_ZeroVectorY;
                    uint32_t value = record[7];
                    CSF_AT(float, valuesBytes, 8) = g_ZeroVectorZ;
                    CSF_AT(uint32_t, valuesBytes, 12) = value;
                    object = C_00863760_CopyFromDwordArray(
                        storage, nullptr,
                        reinterpret_cast<uint32_t*>(valuesBytes));
                }
            }

            uint32_t identifier = record[9];
            if (identifier != uint32_t(-1))
            {
                identifier |= 0x80000000;
            }
            else if (record[8] != uint32_t(-1))
            {
                void* owner = reinterpret_cast<void*>(self->field_0008);
                void* resolver =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, owner, 0)[0x68 / 4])(owner, nullptr);
                identifier =
                    reinterpret_cast<uint32_t(__fastcall*)(void*, void*, uint32_t)>(
                        CSF_AT(void**, resolver, 0)[0x68 / 4])(
                            resolver, nullptr, record[8]) | 0x80000000;
            }
            else
            {
                identifier = record[11];
                if (identifier == uint32_t(-1))
                {
                    void* owner = reinterpret_cast<void*>(self->field_0008);
                    identifier =
                        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, uint32_t)>(
                            CSF_AT(void**, owner, 0)[0x88 / 4])(
                                owner, nullptr, record[10]);
                }
            }

            uint32_t value12 = record[12];
            uint32_t value13 = record[13];
            if (identifier == uint32_t(-1))
            {
                if (object)
                {
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, object, 0)[0 / 4])(
                            object, nullptr, 1);
                }
            }
            else
            {
                if (CSF_AT(uint32_t, self, 0x30) >=
                    CSF_AT(uint32_t, self, 0x2C))
                {
                    GrowEntryBuffer(
                        entryArray, nullptr,
                        static_cast<int>(CSF_AT(uint32_t, self, 0x34)));
                }

                uint32_t count = CSF_AT(uint32_t, self, 0x30);
                auto* destination = reinterpret_cast<uint32_t*>(
                    CSF_AT(uint32_t, self, 0x28) + 16 * count);
                CSF_AT(uint32_t, self, 0x30) = count + 1;
                destination[0] = reinterpret_cast<uint32_t>(object);
                destination[1] = identifier;
                destination[2] = value12;
                destination[3] = value13;
            }

            offset += 56;
        } while (--remaining);
    }

    remaining = source[21];
    if (remaining)
    {
        void* recordArray = reinterpret_cast<uint8_t*>(self) + 0x3C;
        if (remaining > CSF_AT(uint32_t, self, 0x40))
        {
            GrowTwentyByteRecordArray(
                recordArray, nullptr,
                static_cast<int>(remaining - CSF_AT(uint32_t, self, 0x40)));
        }

        uint32_t offset = 0;
        do
        {
            uint32_t* record = reinterpret_cast<uint32_t*>(source[19] + offset);
            uint32_t recordType = record[0];
            uint32_t identifier = record[2];

            if (identifier != uint32_t(-1))
            {
                identifier |= 0x80000000;
            }
            else if (record[1] != uint32_t(-1))
            {
                void* owner = reinterpret_cast<void*>(self->field_0008);
                void* resolver =
                    reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, owner, 0)[0x68 / 4])(owner, nullptr);
                identifier =
                    reinterpret_cast<uint32_t(__fastcall*)(void*, void*, uint32_t)>(
                        CSF_AT(void**, resolver, 0)[0x68 / 4])(
                            resolver, nullptr, record[1]) | 0x80000000;
            }
            else
            {
                identifier = record[4];
                if (identifier == uint32_t(-1))
                {
                    void* owner = reinterpret_cast<void*>(self->field_0008);
                    identifier =
                        reinterpret_cast<uint32_t(__fastcall*)(void*, void*, uint32_t)>(
                            CSF_AT(void**, owner, 0)[0x88 / 4])(
                                owner, nullptr, record[3]) | 0x80000000;
                }
            }

            uint32_t value5 = record[5];
            uint32_t value6 = record[6];
            uint32_t value7 = record[7];
            if (CSF_AT(uint32_t, self, 0x44) >=
                CSF_AT(uint32_t, self, 0x40))
            {
                GrowTwentyByteRecordArray(
                    recordArray, nullptr,
                    static_cast<int>(CSF_AT(uint32_t, self, 0x48)));
            }

            uint32_t count = CSF_AT(uint32_t, self, 0x44);
            uint32_t buffer = CSF_AT(uint32_t, self, 0x3C);
            CSF_AT(uint32_t, self, 0x44) = count + 1;
            auto* destination = reinterpret_cast<uint32_t*>(buffer + 20 * count);
            destination[0] = recordType;
            destination[1] = identifier;
            destination[2] = value5;
            destination[3] = value6;
            destination[4] = value7;

            offset += 32;
        } while (--remaining);
    }

    int result = CSF_AT(int, reinterpret_cast<void*>(self->field_0050), 92);
    self->field_0054 = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x00706E80, C_008632c0_LoadRecords, "auto");
