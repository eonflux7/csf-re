#include <cstdint>
#include "globals.h"
#include "hook.h"

// CHandleTable::Resolve(this = g_pCurrentScene, handle) -> entity pointer or 0
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads a handle record (owner -> record -> {status, handle slot}), resolves the
// handle in the current scene, and if the entity is type 28 with an attached
// child object, sends it command 41 and copies the record's two values into self.
static int __fastcall ApplyHandleRecordAndSendCommand41(void* self, void* /*edx*/,
                                                        int /*a2*/, int /*a3*/,
                                                        uint32_t* ownerRef)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(*ownerRef);
    uint32_t record = CSF_AT(uint32_t, owner, 4);
    int status = CSF_AT(int, reinterpret_cast<uint8_t*>(record), 12);
    int result = status;

    if (status >= 0)
    {
        uint8_t* handleRecord = reinterpret_cast<uint8_t*>(record + 4);
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(record + 4));
        if (result != 0)
        {
            uint8_t* entity = reinterpret_cast<uint8_t*>(result);
            uint32_t childObject = CSF_AT(uint32_t, entity, 0x34);
            if (childObject != 0 && CSF_AT(int, entity, 0x50) == 28)
            {
                int commandArg = 41;
                void* child = reinterpret_cast<void*>(childObject);
                // slot 0xA8 / 4 = 42
                result = reinterpret_cast<int(__fastcall*)(void*, void*, int*)>(
                    CSF_AT(void**, child, 0)[42])(child, nullptr, &commandArg);

                CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, handleRecord, 4);
                CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, handleRecord, 8);
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x460030, ApplyHandleRecordAndSendCommand41, "auto");
