#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __cdecl, IsSubObjectFlagSet, 0x00761060, (int a1));
CSF_FUNC(int, __cdecl, IsResolvedEntryActive, 0x00755880, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(char, __fastcall, HudRefreshOverlayPanels, 0x0042E690, (void* self, void* edx));

// Converts a masked 0..255 channel to float; negative inputs get the unsigned bias.
static float ByteValueToFloatWithBias(int byteValue) {
    float converted = static_cast<float>(byteValue);
    if (byteValue < 0) {
        converted += g_UnsignedIntegerBias;
    }
    return converted;
}

static char __fastcall Hud_UpdateWidgetTint(void* self, void* /*edx*/) {
    bool useStateTint = false;

    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint8_t* sceneMenu = reinterpret_cast<uint8_t*>(sceneMenuContext);

    if (sceneMenuContext != 0) {
        if (CSF_AT(int, sceneMenu, 0x278) >= 0) {
            int resolvedEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                     static_cast<int>(sceneMenuContext + 0x270));
            if (resolvedEntry != 0) {
                void* controller = CSF_AT(void*, sceneMenu, 0x44);
                C_008319c0* subObject =
                    ((C_008319c0*(__fastcall*)(void*, void*))CSF_AT(void**, controller, 0)[54])(controller, nullptr);
                if (subObject != nullptr) {
                    int subObjectField74 = C_008319c0_GetSubObjectField74(subObject, nullptr);
                    if (static_cast<uint8_t>(IsSubObjectFlagSet(subObjectField74)) != 0) {
                        useStateTint = true;
                    } else {
                        uint32_t subObjectTintFlag = CSF_AT(uint32_t, subObject, 0x70);
                        useStateTint = false;
                        if (subObjectTintFlag != 0) {
                            useStateTint = true;
                        }
                    }

                    uint8_t* resolvedEntryBase = reinterpret_cast<uint8_t*>(resolvedEntry);
                    bool entryInactive =
                        !static_cast<uint8_t>(IsResolvedEntryActive(CSF_AT(int, resolvedEntryBase, 0x50)));
                    useStateTint = entryInactive || useStateTint;
                }
            }
        }
    }

    uint8_t* globalValues = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
    int packedTint = CSF_AT(int, globalValues, 0x60);

    if (useStateTint) {
        int stateIndex = CSF_AT(int, sceneMenu, 0x26C) - 1;
        if (stateIndex != 0) {
            int nextStateIndex = stateIndex - 1;
            if (nextStateIndex != 0) {
                if (nextStateIndex == 1) {
                    uint8_t* stateGlobals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
                    packedTint = CSF_AT(int, stateGlobals, 0x64);
                }
            } else {
                uint8_t* stateGlobals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
                packedTint = CSF_AT(int, stateGlobals, 0x6C);
            }
        } else {
            uint8_t* stateGlobals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
            packedTint = CSF_AT(int, stateGlobals, 0x68);
        }
    }

    float colorChannels[5];
    for (int i = 0; i < 24; i += 12) {
        uint8_t* entryArray = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x140));
        void* entry = CSF_AT(void*, entryArray, i);
        if (entry != nullptr) {
            uint32_t packedColor = static_cast<uint32_t>(packedTint);
            uint32_t blueChannel = packedColor & 0xFFu;

            colorChannels[0] = ByteValueToFloatWithBias(static_cast<int>((packedColor >> 16) & 0xFFu));
            colorChannels[1] = ByteValueToFloatWithBias(static_cast<int>((packedColor >> 8) & 0xFFu));
            // The original stores the raw integer bits of the blue byte into the fifth slot.
            std::memcpy(&colorChannels[4], &blueChannel, sizeof(blueChannel));
            colorChannels[2] = ByteValueToFloatWithBias(static_cast<int>(blueChannel));
            colorChannels[3] = 255.0f;

            ((void(__fastcall*)(void*, void*, float*))CSF_AT(void**, entry, 0)[26])(entry, nullptr, colorChannels);
        }
    }

    return HudRefreshOverlayPanels(self, nullptr);
}

HOOK(0x430BB0, Hud_UpdateWidgetTint, "auto");
