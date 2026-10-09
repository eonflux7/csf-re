#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneSubsystem_LookupEntry, 0x00519AF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));

static int __fastcall IssueScaledHandleCommand(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* paramRefs)
{
    // Local command block passed to the entry's virtual command slot (16 bytes).
    struct CommandPayload {
        int commandKind;   // 25
        int handle;
        float scaledValue;
        int extra;         // -1
    };

    // paramRefs[0] -> record whose +4 word is the entry record.
    uint32_t entryRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(paramRefs[0]), 4);
    int result = CSF_AT(int, reinterpret_cast<void*>(entryRecord), 12);
    uint32_t handleRecordAddr = entryRecord + 4;

    if (result >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(handleRecordAddr));
        void* resolvedEntity = reinterpret_cast<void*>(result);

        if (resolvedEntity) {
            if (CSF_AT(int, resolvedEntity, 52)) {
                // Handle value comes from paramRefs[1] -> record -> +4.
                uint32_t selectorRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(paramRefs[1]), 4);
                int handleValue = CSF_AT(int, reinterpret_cast<void*>(selectorRecord), 4);

                CommandPayload payload{};
                payload.commandKind = 25;
                payload.handle = handleValue;
                payload.scaledValue = 0.0f;
                payload.extra = -1;

                void* subsystemBase = reinterpret_cast<void*>(GetValue8B0554());
                void* subsystemObj = CSF_AT(void*, subsystemBase, 8132);
                result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, subsystemObj, 0)[2])(subsystemObj, nullptr, handleValue);
                int resolvedIndex = result;

                if (resolvedIndex != -1) {
                    void* globalIntObj = reinterpret_cast<void*>(GetGlobalInt());
                    void* accessorObj = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(globalIntObj, nullptr));
                    int accessorState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, accessorObj, 0)[5])(accessorObj, nullptr);

                    void* subsystemBase2 = reinterpret_cast<void*>(GetValue8B0554());
                    void* subsystemObj2 = CSF_AT(void*, subsystemBase2, 8132);
                    int lookupEntry = SceneSubsystem_LookupEntry(subsystemObj2, nullptr, resolvedIndex);
                    result = AccessorState_ResolveSlot(reinterpret_cast<void*>(accessorState), nullptr, lookupEntry);

                    if (result) {
                        uint32_t floatRecord = CSF_AT(uint32_t, reinterpret_cast<void*>(paramRefs[2]), 4);
                        double unknownValue = C_Unknown_GetValue(reinterpret_cast<void*>(result), nullptr);
                        payload.scaledValue = static_cast<float>(unknownValue * CSF_AT(float, reinterpret_cast<void*>(floatRecord), 4));

                        void* commandTarget = CSF_AT(void*, resolvedEntity, 52);
                        result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, commandTarget, 0)[42])(commandTarget, nullptr, &payload);

                        CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, reinterpret_cast<void*>(handleRecordAddr), 4);
                        CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, reinterpret_cast<void*>(handleRecordAddr), 8);
                        CSF_AT(uint8_t, self, 0x82) = 0;
                    }
                }
            }
        }
    }

    return result;
}

HOOK(0x0045F8A0, IssueScaledHandleCommand, "auto");
