#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(double, __fastcall, PlaneDistanceToPoint, 0x004024D0, (float* self, void* , float* point));
CSF_FUNC(void, __stdcall, FinalizeSpawnUpdate, 0x0064AF90, (int a1, int a2));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __stdcall, NetSession_GetEntryFlags, 0x0073CCE0, (int a1));
CSF_FUNC(bool, __fastcall, C_00858180_CommitSpawnAndForward, 0x0073FA90, (C_00858180* self, void* , char a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, char a18, unsigned int a19));
CSF_FUNC(bool, __fastcall, C_00858180_CommitSpawnRecordAndDispatchNetPayload, 0x00740410, (C_00858180* self, void* , char a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13, int a14, int a15, int a16, int a17, char a18, unsigned int a19));

CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);

static int __fastcall C_00834198_DispatchOrQueueSpawn(
    C_00834198* self, void* /*edx*/, std::uint8_t* record, float time)
{
    if (self->field_01b8)
    {
        void* interfaceObject = CSF_AT(void*, self, 0x18);
        const int type = CSF_AT(int, interfaceObject, 0x0C);

        if (type >= 5 && type <= 12)
        {
            std::uint8_t* owner = nullptr;
            if (interfaceObject)
            {
                owner = static_cast<std::uint8_t*>(interfaceObject)
                    - (type == 12 ? 120 : 112);
            }

            auto* position = reinterpret_cast<std::uint8_t*>(self->field_004c) + 0x84;

            if (owner)
            {
                int entryFlags;
                void* entry = CSF_AT(void*, record, 0x4C);
                if (entry)
                {
                    const int entryId = CSF_AT(int, entry, 0x18);
                    GetGlobalDword_73C020();
                    entryFlags = NetSession_GetEntryFlags(entryId);
                }
                else
                {
                    entryFlags = -1;
                }

                alignas(8) std::uint8_t originBytes[0x14];
                auto* origin = reinterpret_cast<float*>(originBytes);
                origin[0] = 0.0f;
                origin[1] = 0.0f;
                origin[2] = 0.0f;

                const std::int64_t milliseconds =
                    static_cast<std::int64_t>(time * g_flt_0082E25C);

                const int sourceId =
                    (reinterpret_cast<int (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, interfaceObject, 0)[2]))(
                            interfaceObject, nullptr);

                const int positionZ = CSF_AT(int, position, 0x08);
                const int positionY = CSF_AT(int, position, 0x04);
                const int positionX = CSF_AT(int, position, 0x00);

                const float distance = static_cast<float>(
                    PlaneDistanceToPoint(
                        reinterpret_cast<float*>(record + 0x20), nullptr, origin));
                int distanceBits;
                std::memcpy(&distanceBits, &distance, sizeof(distanceBits));

                auto* destination = reinterpret_cast<C_00858180*>(owner);
                if (type == 12)
                {
                    C_00858180_CommitSpawnRecordAndDispatchNetPayload(
                        destination, nullptr,
                        static_cast<char>(record[0]),
                        CSF_AT(int, record, 0x04),
                        CSF_AT(int, record, 0x08),
                        CSF_AT(int, record, 0x0C),
                        CSF_AT(int, record, 0x10),
                        CSF_AT(int, record, 0x14),
                        CSF_AT(int, record, 0x18),
                        CSF_AT(int, record, 0x1C),
                        CSF_AT(int, record, 0x2C),
                        CSF_AT(int, record, 0x30),
                        CSF_AT(int, record, 0x34),
                        distanceBits,
                        positionX, positionY, positionZ,
                        entryFlags, static_cast<char>(sourceId),
                        static_cast<unsigned int>(milliseconds));
                }
                else
                {
                    C_00858180_CommitSpawnAndForward(
                        destination, nullptr,
                        static_cast<char>(record[0]),
                        CSF_AT(int, record, 0x04),
                        CSF_AT(int, record, 0x08),
                        CSF_AT(int, record, 0x0C),
                        CSF_AT(int, record, 0x10),
                        CSF_AT(int, record, 0x14),
                        CSF_AT(int, record, 0x18),
                        CSF_AT(int, record, 0x1C),
                        CSF_AT(int, record, 0x2C),
                        CSF_AT(int, record, 0x30),
                        CSF_AT(int, record, 0x34),
                        distanceBits,
                        positionX, positionY, positionZ,
                        entryFlags, static_cast<char>(sourceId),
                        static_cast<unsigned int>(milliseconds));
                }
            }
        }
    }
    else
    {
        std::memcpy(&self->field_01bc, &time, sizeof(time));
        self->field_01ba = 1;
        std::memcpy(&self->field_01c0, record, 0x64);
    }

    int timeBits;
    std::memcpy(&timeBits, &time, sizeof(timeBits));
    FinalizeSpawnUpdate(
        static_cast<int>(reinterpret_cast<std::uintptr_t>(record)), timeBits);

    // The final callee is an empty ret 8; EAX still contains this.
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0049E3C0, C_00834198_DispatchOrQueueSpawn, "auto");
