#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, NetSession_ReleaseTransportStorage, 0x006471C0, (int a2));
CSF_FUNC(char, __fastcall, NetSession_SetConnectionState, 0x006510B0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ReleaseEmbeddedResources, 0x00652810, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, NetGatheringDdl_DestroyConnectionEntry, 0x006A9F80, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetGatheringDdl_ReleaseTransportResources, 0x007A14C0, (void* self, void* edx));

static char __fastcall S_006a8940_ResetGatheringConnections(
    S_006a8940* self, void* /*edx*/)
{
    char result = 0;

    if (self->field_0328)
    {
        for (int offset = 0; offset < 40; offset += 4)
        {
            void* entry = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0328), offset);

            if (entry)
            {
                void* session = CSF_AT(void*, entry, 0x38);
                if (session)
                {
                    NetSession_SetConnectionState(session, nullptr, 4);

                    session = CSF_AT(void*, entry, 0x38);
                    if (session)
                    {
                        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                            CSF_AT(void**, session, 0)[0])(
                                session, nullptr, 1);
                        CSF_AT(uint32_t, entry, 0x38) = 0;
                    }
                }

                NetGatheringDdl_ReleaseEmbeddedResources(
                    static_cast<uint8_t*>(entry) + 8, nullptr);

                const uint32_t transport = CSF_AT(uint32_t, entry, 0x28);
                if (transport)
                {
                    NetGatheringDdl_ReleaseTransportResources(
                        reinterpret_cast<void*>(transport), nullptr);
                    NetSession_ReleaseTransportStorage(
                        static_cast<int>(transport));
                    CSF_AT(uint32_t, entry, 0x28) = 0;
                }

                CSF_AT(uint32_t, entry, 4) = 0;

                void* remainingEntry = CSF_AT(
                    void*, reinterpret_cast<void*>(self->field_0328), offset);
                if (remainingEntry)
                {
                    NetGatheringDdl_DestroyConnectionEntry(
                        remainingEntry, nullptr);
                    Mem_Free(remainingEntry);
                    CSF_AT(uint32_t,
                           reinterpret_cast<void*>(self->field_0328),
                           offset) = 0;
                }
            }
        }

        if (self->field_0328)
        {
            result = static_cast<char>(
                Mem_Free(reinterpret_cast<void*>(self->field_0328)));
            self->field_0328 = 0;
        }
    }

    self->field_0008 = 2;
    self->field_000c = 0;
    self->field_0334 = 0;
    return result;
}

HOOK(0x006AA470, S_006a8940_ResetGatheringConnections, "auto");
