#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Scene object whose handle table is queried.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// CHandleTable::Resolve(this = scene, handle record address) -> resolved object or 0.
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// __thiscall with three stack args (a2, a3, a4) -> ret 12.
static int __fastcall SendPlacementCommandForHandle(void* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    // v4 = *(*a4 + 4)
    void* firstRef = reinterpret_cast<void*>(a4[0]);
    uint32_t v4 = CSF_AT(uint32_t, firstRef, 4);

    // result = *(v4 + 12), tested as a signed value below
    int result = static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(v4), 12));

    // Address of the 3-dword handle record (v6 = v4 + 4)
    uint32_t handleRecordAddress = v4 + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleRecordAddress));
        if (result != 0)
        {
            void* sceneEntity = reinterpret_cast<void*>(static_cast<uint32_t>(result));
            void* commandSink = reinterpret_cast<void*>(CSF_AT(uint32_t, sceneEntity, 52));
            if (commandSink != nullptr)
            {
                // Command message: type 11 followed by three 3-dword records and one scalar.
                uint32_t message[8] = {};
                message[0] = 11;

                uint32_t firstRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a4[1]), 4) + 4;
                message[1] = CSF_AT(uint32_t, reinterpret_cast<void*>(firstRecord), 0);
                message[2] = CSF_AT(uint32_t, reinterpret_cast<void*>(firstRecord), 4);
                message[3] = CSF_AT(uint32_t, reinterpret_cast<void*>(firstRecord), 8);

                uint32_t secondRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(a4[2]), 4) + 4;
                message[4] = CSF_AT(uint32_t, reinterpret_cast<void*>(secondRecord), 0);
                message[5] = CSF_AT(uint32_t, reinterpret_cast<void*>(secondRecord), 4);
                message[6] = CSF_AT(uint32_t, reinterpret_cast<void*>(secondRecord), 8);

                uint32_t thirdBlock = CSF_AT(uint32_t, reinterpret_cast<void*>(a4[3]), 4);
                message[7] = CSF_AT(uint32_t, reinterpret_cast<void*>(thirdBlock), 4);

                // Virtual slot 42 (byte offset 168) of the command sink, __thiscall, one stack arg.
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, commandSink, 0)[42])(
                    commandSink, nullptr, message);

                // Re-read the handle record after the call; the callee may have changed the scene.
                result = static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(handleRecordAddress), 4));
                CSF_AT(int, self, 96) = result;
                CSF_AT(int, self, 100) = static_cast<int>(
                    CSF_AT(uint32_t, reinterpret_cast<void*>(handleRecordAddress), 8));
                CSF_AT(uint8_t, self, 130) = 0;
            }
        }
    }

    return result;
}

HOOK(0x0045EF70, SendPlacementCommandForHandle, "auto");
