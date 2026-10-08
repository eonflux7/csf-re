#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Scene that owns the handle table.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// CHandleTable::Resolve: takes the address of a handle record, returns the resolved entry (0 if not found).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handleRecordAddr));

// 16-byte command block passed to the entry object's virtual method.
struct SceneEntityCommand {
    std::int32_t payloadSize;  // constant 12
    std::int32_t valueA;       // dword reached through argSlots[1]
    std::int32_t valueB;       // float reached through argSlots[2], truncated to int
    std::int32_t valueC;       // dword reached through argSlots[3]
};

static std::uint32_t ReadDword(std::uint32_t address) {
    return *reinterpret_cast<std::uint32_t*>(address);
}

static int __fastcall ApplyRecordCommandToSceneEntity(void* self, void* /*edx*/,
                                                      int /*unusedA*/, int /*unusedB*/,
                                                      std::uint32_t* argSlots) {
    // argSlots[0] points at a record whose +4 field is the handle holder.
    std::uint32_t handleHolder = ReadDword(argSlots[0] + 4);
    int result = static_cast<int>(ReadDword(handleHolder + 12));
    if (result < 0) {
        return result;
    }

    // The handle record lives 4 bytes into the holder.
    std::uint32_t handleRecord = handleHolder + 4;
    result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleRecord));
    if (result == 0) {
        return result;
    }

    void* resolvedEntry = reinterpret_cast<void*>(result);
    void* targetObject = CSF_AT(void*, resolvedEntry, 0x34);
    if (targetObject == nullptr) {
        return result;
    }

    SceneEntityCommand command;
    command.payloadSize = 12;
    command.valueA = static_cast<std::int32_t>(ReadDword(ReadDword(argSlots[1] + 4) + 4));
    float positionFloat = *reinterpret_cast<float*>(ReadDword(argSlots[2] + 4) + 4);
    command.valueB = static_cast<std::int32_t>(static_cast<std::int64_t>(positionFloat));
    command.valueC = static_cast<std::int32_t>(ReadDword(ReadDword(argSlots[3] + 4) + 4));

    // Virtual slot 0xA8 / 4 = 42 on the entry object: __thiscall, one pointer argument.
    using EntityCommandMethod = void(__fastcall*)(void* self, void* edx, SceneEntityCommand* command);
    ((EntityCommandMethod)CSF_AT(void**, targetObject, 0)[42])(targetObject, nullptr, &command);

    // The record is re-read after the virtual call, as the original does.
    std::uint32_t recordX = ReadDword(handleRecord + 4);
    CSF_AT(std::uint32_t, self, 0x60) = recordX;
    std::uint32_t recordY = ReadDword(handleRecord + 8);
    CSF_AT(std::uint32_t, self, 0x64) = recordY;
    CSF_AT(std::uint8_t, self, 0x82) = 0;
    return static_cast<int>(recordY);
}

HOOK(0x0045F050, ApplyRecordCommandToSceneEntity, "auto");
