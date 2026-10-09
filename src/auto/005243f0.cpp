#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);

CSF_FUNC(uint32_t*, __fastcall, InitGlobalRequest_405A00, 0x00405A00, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SoundMessage_Initialize, 0x00405A50, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CSoundManager_PlaySoundInstance, 0x00521DF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, DispatchGlobalValueMessage, 0x00522330, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CSoundManager_StopSoundsForHandle, 0x00522560, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ApplyGlobalValueRequest, 0x00523940, (void* self, void* edx, void* a2, float a3));
CSF_FUNC(int, __fastcall, SubmitOrderRequest, 0x005242B0, (void* self, void* edx, void* a2, int a3));
CSF_FUNC(int, __fastcall, ObjectAttachment_HasSoundTrigger, 0x006B8140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, SceneLoader_GetObjectAttachment, 0x006B87A0, (int a1));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetResourceById, 0x007567C0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ResolveSlotIndexForState, 0x00756810, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// The packet also classifies this call target as a vtable reference.
[[maybe_unused]] static const auto g_PlaySoundInstanceTable = CSF_VTBL(0x00521DF0);

static uint32_t* __fastcall CSoundManager_HandleSceneEvent(
    CSoundManager* self, void* /*edx*/, uint32_t* event)
{
    alignas(4) uint8_t positionalRequest[0x38]{};
    alignas(4) uint8_t message[0x10]{};
    alignas(4) uint8_t request[0x3C]{};

    if (event[1] == 4)
    {
        bool shouldPlay = false;
        if (event[22])
        {
            int attachment = SceneLoader_GetObjectAttachment(event[22]);
            if (attachment)
                shouldPlay = static_cast<uint8_t>(
                    ObjectAttachment_HasSoundTrigger(
                        reinterpret_cast<void*>(attachment), nullptr)) != 0;
        }

        if (event[23] != uint32_t(-1))
        {
            if (event[24] != 9 || shouldPlay)
                shouldPlay = true;

            bool suppress = false;
            if (static_cast<int32_t>(event[7]) >= 0)
            {
                auto* object = reinterpret_cast<C_00833b88*>(
                    CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                         reinterpret_cast<int>(event + 5)));
                if (object && (object->field_0170 & 0x2000) &&
                    static_cast<uint8_t>(SceneObj_IsInactive(object, nullptr)))
                {
                    void* activity = reinterpret_cast<void*>(object->field_0038);
                    if (activity)
                    {
                        double value =
                            reinterpret_cast<double(__fastcall*)(void*, void*)>(
                                CSF_AT(void**, activity, 0)[196 / 4])(
                                    activity, nullptr);
                        suppress = value >= g_SoundActivityThreshold;
                    }
                }
            }

            if (!suppress && shouldPlay)
            {
                void* definitions = CSF_AT(void*,
                    reinterpret_cast<void*>(GetGlobalState_50D190()), 0xE8);
                InitGlobalRequest_405A00(request, nullptr);
                CSF_AT(uint32_t, request, 0x18) = event[13];
                CSF_AT(uint32_t, request, 0x1C) = event[14];
                CSF_AT(uint32_t, request, 0x20) = event[15];
                CSF_AT(int, request, 0) = 1;

                void* definition =
                    reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, definitions, 0)[8 / 4])(
                            definitions, nullptr, event[23]);
                CSF_AT(uint32_t, request, 0x08) =
                    CSF_AT(uint32_t, definition, 0x24);

                int instance = ApplyGlobalValueRequest(self, nullptr, request, -1.0f);
                if (instance)
                    CSoundManager_PlaySoundInstance(self, nullptr, instance);
            }
        }
    }

    if (event[1] == 26 || event[1] == 27)
    {
        auto* object = reinterpret_cast<uint32_t*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                 reinterpret_cast<int>(event + 2)));
        if (!object)
            return nullptr;

        int active = IsResolvedEntryActive(object[20]);
        if (!static_cast<uint8_t>(active))
            return reinterpret_cast<uint32_t*>(active);

        InitGlobalRequest_405A00(request, nullptr);
        uint32_t handlePart1 = event[3];
        uint32_t handlePart2 = event[4];
        int alternate = event[1] != 26;
        CSF_AT(int, request, 0) = 2;
        CSF_AT(uint32_t, request, 0x28) = handlePart1;
        CSF_AT(uint32_t, request, 0x2C) = handlePart2;
        int state = object[20];
        void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
        CSF_AT(int, request, 0x08) = ResolveSlotIndexForState(
            static_cast<uint8_t*>(global) + 0x22A4, nullptr, alternate, state);

        CSF_AT(uint32_t, message, 0x04) = CSF_AT(uint32_t, request, 0x28);
        CSF_AT(uint32_t, message, 0x08) = CSF_AT(uint32_t, request, 0x2C);
        CSF_AT(void**, message, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, message, 0x0C) = 1232348224;
        SubmitOrderRequest(self, nullptr, request, reinterpret_cast<int>(message));
    }

    uint32_t* result = reinterpret_cast<uint32_t*>(event[1]);
    if (event[1] == 14 || event[1] == 15)
    {
        CSF_AT(int, positionalRequest, 0) = 1;
        CSF_AT(int, positionalRequest, 0x04) = 1;
        CSF_AT(int, positionalRequest, 0x08) = -1;
        CSF_AT(int, positionalRequest, 0x0C) = -1;
        CSF_AT(int, positionalRequest, 0x10) = 0;
        CSF_AT(int, positionalRequest, 0x14) = 0;
        CSF_AT(uint32_t, positionalRequest, 0x18) = event[13];
        CSF_AT(uint32_t, positionalRequest, 0x1C) = event[14];
        CSF_AT(uint32_t, positionalRequest, 0x20) = event[15];
        CSF_AT(void**, positionalRequest, 0x24) = CSF_VTBL(0x0082D448);
        CSF_AT(int, positionalRequest, 0x28) = -1;
        CSF_AT(int, positionalRequest, 0x2C) = -1;
        CSF_AT(int, positionalRequest, 0x30) = 0;
        CSF_AT(uint8_t, positionalRequest, 0x34) = 0;
        CSF_AT(uint8_t, positionalRequest, 0x35) = 1;
        CSF_AT(uint8_t, positionalRequest, 0x36) = 1;

        CSF_AT(int, request, 0) = 2;
        CSF_AT(int, request, 0x04) = 3;
        CSF_AT(int, request, 0x08) = -1;
        CSF_AT(int, request, 0x0C) = -1;
        std::memset(request + 0x10, 0, 20);
        CSF_AT(void**, request, 0x24) = CSF_VTBL(0x0082D448);
        CSF_AT(int, request, 0x28) = -1;
        CSF_AT(int, request, 0x2C) = -1;
        CSF_AT(int, request, 0x30) = 0;
        CSF_AT(uint8_t, request, 0x34) = 0;
        CSF_AT(uint8_t, request, 0x35) = 1;
        CSF_AT(uint8_t, request, 0x36) = 1;

        auto* source = reinterpret_cast<uint32_t*>(event[16]);
        bool submit = false;
        bool chooseDefault = false;

        if (source)
        {
            CSF_AT(uint32_t, request, 0x28) = source[8];
            CSF_AT(uint32_t, request, 0x2C) = source[9];
            if (!event[17])
            {
                chooseDefault = true;
                if (!CSF_AT(uint8_t, event, 0x5C) && event[9] == 4)
                {
                    CSF_AT(int, positionalRequest, 0x08) =
                        CSF_AT(int, reinterpret_cast<void*>(source[11]), 0xC8);
                    if (CSF_AT(int, positionalRequest, 0x08) != -1)
                    {
                        chooseDefault = false;
                        submit = true;
                    }
                }
            }
            else
            {
                result = reinterpret_cast<uint32_t*>(source[20]);
                if (result == reinterpret_cast<uint32_t*>(14) ||
                    result == reinterpret_cast<uint32_t*>(15))
                {
                    void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                    result = reinterpret_cast<uint32_t*>(SceneLoader_GetResourceById(
                        static_cast<uint8_t*>(global) + 0x22A4, nullptr, 27));
                    CSF_AT(uint32_t*, request, 0x08) = result;
                    submit = true;
                }
                else if (result == reinterpret_cast<uint32_t*>(16))
                {
                    void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                    CSF_AT(int, positionalRequest, 0x08) = SceneLoader_GetResourceById(
                        static_cast<uint8_t*>(global) + 0x22A4, nullptr, 24);
                    global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                    result = reinterpret_cast<uint32_t*>(SceneLoader_GetResourceById(
                        static_cast<uint8_t*>(global) + 0x22A4, nullptr, 28));
                    CSF_AT(uint32_t*, request, 0x08) = result;
                    submit = true;
                }
            }
        }
        else
        {
            result = reinterpret_cast<uint32_t*>(event[17]);
            chooseDefault = !result;
        }

        if (chooseDefault)
        {
            if (CSF_AT(uint8_t, event, 0x5C))
            {
                void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                result = reinterpret_cast<uint32_t*>(SceneLoader_GetResourceById(
                    static_cast<uint8_t*>(global) + 0x22A4, nullptr, 18));
            }
            else
            {
                void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                result = reinterpret_cast<uint32_t*>(SceneLoader_GetResourceById(
                    static_cast<uint8_t*>(global) + 0x22A4, nullptr, 17));
            }
            CSF_AT(uint32_t*, positionalRequest, 0x08) = result;
            submit = true;
        }

        if (submit)
        {
            if (CSF_AT(int, positionalRequest, 0x08) != -1)
            {
                result = reinterpret_cast<uint32_t*>(
                    ApplyGlobalValueRequest(self, nullptr, positionalRequest, -1.0f));
                if (result)
                    result = reinterpret_cast<uint32_t*>(CSoundManager_PlaySoundInstance(
                        self, nullptr, reinterpret_cast<int>(result)));
            }
            if (CSF_AT(int, request, 0x08) != -1)
            {
                CSF_AT(uint32_t, message, 0x04) = CSF_AT(uint32_t, request, 0x28);
                CSF_AT(void**, message, 0) = CSF_VTBL(0x0082D448);
                CSF_AT(uint32_t, message, 0x08) = CSF_AT(uint32_t, request, 0x2C);
                CSF_AT(int, message, 0x0C) = 1232348256;
                result = reinterpret_cast<uint32_t*>(SubmitOrderRequest(
                    self, nullptr, request, reinterpret_cast<int>(message)));
                if (result)
                    result = reinterpret_cast<uint32_t*>(CSoundManager_PlaySoundInstance(
                        self, nullptr, reinterpret_cast<int>(result)));
            }
        }
    }

    if (event[1] == 35)
    {
        result = reinterpret_cast<uint32_t*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(event + 5)));
        if (result && (result[92] & 0x2000))
            result = reinterpret_cast<uint32_t*>(CSoundManager_StopSoundsForHandle(
                self, nullptr, reinterpret_cast<int>(result + 7)));
    }

    if (event[1] == 8)
    {
        result = reinterpret_cast<uint32_t*>(CHandleTable_Resolve(
            g_pCurrentScene, nullptr, reinterpret_cast<int>(event + 2)));
        uint32_t* object = result;
        if (object)
        {
            if (object[92] & 0x2000)
            {
                uint32_t* handle = object + 7;
                CSoundManager_StopSoundsForHandle(
                    self, nullptr, reinterpret_cast<int>(handle));
                result = reinterpret_cast<uint32_t*>(object[14]);
                if (result[4] != 1)
                {
                    int resource;
                    if (object[118])
                        resource = 6 - (CSF_AT(uint8_t,
                            reinterpret_cast<void*>(object[11]), 0x15C) != 0);
                    else
                        resource = 8 - (CSF_AT(uint8_t,
                            reinterpret_cast<void*>(object[11]), 0x15C) != 0);

                    InitGlobalRequest_405A00(request, nullptr);
                    CSF_AT(uint32_t, request, 0x28) = handle[1];
                    CSF_AT(uint32_t, request, 0x2C) = handle[2];
                    CSF_AT(int, request, 0) = 2;
                    CSF_AT(int, request, 0x04) = 2;
                    void* global = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
                    CSF_AT(int, request, 0x08) = SceneLoader_GetResourceById(
                        static_cast<uint8_t*>(global) + 0x22A4, nullptr, resource);
                    result = reinterpret_cast<uint32_t*>(
                        ApplyGlobalValueRequest(self, nullptr, request, -1.0f));
                    if (result)
                        return reinterpret_cast<uint32_t*>(CSoundManager_PlaySoundInstance(
                            self, nullptr, reinterpret_cast<int>(result)));
                }
            }
            else
            {
                int active = IsResolvedEntryActive(object[20]);
                result = reinterpret_cast<uint32_t*>(active);
                if (static_cast<uint8_t>(active))
                {
                    uint32_t* initialized = SoundMessage_Initialize(
                        message, nullptr, reinterpret_cast<int>(object + 7), 1232348224);
                    return reinterpret_cast<uint32_t*>(DispatchGlobalValueMessage(
                        self, nullptr, reinterpret_cast<int>(initialized)));
                }
            }
        }
    }
    return result;
}

HOOK(0x005243F0, CSoundManager_HandleSceneEvent, "auto");
