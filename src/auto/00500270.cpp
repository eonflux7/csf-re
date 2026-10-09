#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __cdecl, SceneSubObjectTest, 0x0041E380, (void* a1));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(const char*, __fastcall, C_0082fbe8_GetConstantName, 0x0050F6D0, (C_0082fbe8* , void*));
CSF_FUNC(int, __fastcall, Table_GetRecord, 0x0075CF00, (void* self, void* edx, int a2));

static int __cdecl SceneLoader_CheckSectorSubObjects()
{
    auto* scene = reinterpret_cast<void*>(GetValue8B0554());
    auto* table = CSF_AT(C_0083e0d8*, scene, 0x1FD8);

    std::uint32_t recordCount = 0;
    const std::uint32_t recordsBegin = table->field_0008;
    if (recordsBegin)
    {
        recordCount = static_cast<std::uint32_t>(
            static_cast<std::int32_t>(table->field_000c - recordsBegin) / 72);
    }

    for (std::uint32_t index = 0; index < recordCount; ++index)
    {
        auto* record = reinterpret_cast<void*>(
            Table_GetRecord(table, nullptr, static_cast<int>(index)));

        const int type = CSF_AT(int, record, 0x20);
        if (type == 3 || type == 2)
        {
            const std::uint32_t objectsBegin =
                CSF_AT(std::uint32_t, record, 0x38);
            if (objectsBegin)
            {
                std::uint32_t remaining = static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        CSF_AT(std::uint32_t, record, 0x3C) - objectsBegin) / 44);

                std::uint32_t offset = 0;
                while (remaining)
                {
                    auto* object = reinterpret_cast<C_0082fbe8*>(
                        CSF_AT(std::uint32_t, record, 0x38) + offset);

                    if (!SceneSubObjectTest(
                            reinterpret_cast<std::uint8_t*>(object) + 4))
                    {
                        C_0082fbe8_GetConstantName(object, nullptr);
                    }

                    offset += 44;
                    --remaining;
                }
            }
        }
    }

    return static_cast<std::int32_t>(recordCount);
}

HOOK(0x00500270, SceneLoader_CheckSectorSubObjects, "auto");
