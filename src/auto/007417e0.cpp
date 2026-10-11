#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Sub_64B250, 0x0064B250, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateAuthoritativeState, 0x0064C980, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetCogibleDDL_UpdateReplicaState, 0x0064CA10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, C_74B200_SetFlagAndReturnTrue, 0x0074B200, (uint8_t* self, void*));
CSF_FUNC(char, __fastcall, C_0074b2a0_SetFlag, 0x0074B2A0, (void* self, void*));
CSF_FUNC(char, __fastcall, SetFlag20C, 0x00751810, (void* self, void*));
CSF_FUNC(char, __fastcall, C_751990_SetFlagAndReturnTrue, 0x00751990, (void* self, void*));

static int __fastcall NetCogibleDDL_UpdateAndTrackChanges(
    uint8_t* self, void* /*edx*/, int /*updateArgument*/)
{
    if (CSF_AT(int32_t, self, 0x6C) == -1)
        CSF_AT(int32_t, self, 0x6C) = CSF_AT(int32_t, self, -340);

    CSF_AT(int32_t, self, 0x0C) = 0;

    if (static_cast<uint8_t>(Sub_64B250(self - 1712, nullptr)))
        NetCogibleDDL_UpdateAuthoritativeState(self - 1712, nullptr);
    else
        NetCogibleDDL_UpdateReplicaState(self - 1712, nullptr);

    void* sentinel = CSF_AT(void*, self, 0x30);
    void* node = CSF_AT(void*, sentinel, 0x00);
    if (node != sentinel)
    {
        do
        {
            void* payload = CSF_AT(void*, node, 0x08);
            if (payload && CSF_AT(uint8_t, payload, 0x10))
            {
                void* removedNode = node;
                bool isSentinel = node == CSF_AT(void*, self, 0x30);
                node = CSF_AT(void*, node, 0x00);
                if (!isSentinel)
                {
                    CSF_AT(void*, CSF_AT(void*, removedNode, 0x04), 0x00) =
                        CSF_AT(void*, removedNode, 0x00);
                    CSF_AT(void*, CSF_AT(void*, removedNode, 0x00), 0x04) =
                        CSF_AT(void*, removedNode, 0x04);
                    Mem_FreeDebug(removedNode);
                    --CSF_AT(uint32_t, self, 0x34);
                }
                Mem_Free(payload);
            }
            else
            {
                node = CSF_AT(void*, node, 0x00);
            }
        } while (node != CSF_AT(void*, self, 0x30));
    }

    sentinel = CSF_AT(void*, self, 0x24);
    node = CSF_AT(void*, sentinel, 0x00);
    if (node != sentinel)
    {
        do
        {
            void* payload = CSF_AT(void*, node, 0x08);
            if (payload && CSF_AT(uint8_t, payload, 0x24))
            {
                void* removedNode = node;
                bool isSentinel = node == CSF_AT(void*, self, 0x24);
                node = CSF_AT(void*, node, 0x00);
                if (!isSentinel)
                {
                    CSF_AT(void*, CSF_AT(void*, removedNode, 0x04), 0x00) =
                        CSF_AT(void*, removedNode, 0x00);
                    CSF_AT(void*, CSF_AT(void*, removedNode, 0x00), 0x04) =
                        CSF_AT(void*, removedNode, 0x04);
                    Mem_FreeDebug(removedNode);
                    --CSF_AT(uint32_t, self, 0x28);
                }
                Mem_Free(payload);
            }
            else
            {
                node = CSF_AT(void*, node, 0x00);
            }
        } while (node != CSF_AT(void*, self, 0x24));
    }

    if (static_cast<uint8_t>(Sub_64B250(self - 1712, nullptr)))
    {
        uint16_t wordValue = CSF_AT(uint16_t, self, -606);
        if (wordValue != CSF_AT(uint16_t, self, 0x38))
        {
            CSF_AT(uint16_t, self, 0x38) = wordValue;
            C_751990_SetFlagAndReturnTrue(self - 606, nullptr);
        }

        int32_t integerValue = CSF_AT(int32_t, self, -8);
        if (integerValue != CSF_AT(uint16_t, self, 0x3A))
        {
            CSF_AT(uint16_t, self, 0x3A) = static_cast<uint16_t>(integerValue);
            C_0074b2a0_SetFlag(self - 8, nullptr);
        }

        wordValue = CSF_AT(uint16_t, self, -36);
        bool changed = wordValue != CSF_AT(uint16_t, self, -32);
        CSF_AT(uint16_t, self, -32) = wordValue;
        if (changed)
        {
            C_751990_SetFlagAndReturnTrue(self - 36, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        wordValue = CSF_AT(uint16_t, self, -48);
        changed = wordValue != CSF_AT(uint16_t, self, -44);
        CSF_AT(uint16_t, self, -44) = wordValue;
        if (changed)
        {
            C_751990_SetFlagAndReturnTrue(self - 48, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        wordValue = CSF_AT(uint16_t, self, -24);
        changed = wordValue != CSF_AT(uint16_t, self, -20);
        CSF_AT(uint16_t, self, -20) = wordValue;
        if (changed)
        {
            C_751990_SetFlagAndReturnTrue(self - 24, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        wordValue = CSF_AT(uint16_t, self, -30);
        changed = wordValue != CSF_AT(uint16_t, self, -26);
        CSF_AT(uint16_t, self, -26) = wordValue;
        if (changed)
        {
            C_751990_SetFlagAndReturnTrue(self - 30, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        wordValue = CSF_AT(uint16_t, self, -18);
        changed = wordValue != CSF_AT(uint16_t, self, -14);
        CSF_AT(uint16_t, self, -14) = wordValue;
        if (changed)
        {
            C_751990_SetFlagAndReturnTrue(self - 18, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        wordValue = CSF_AT(uint16_t, self, -42);
        changed = wordValue != CSF_AT(uint16_t, self, -38);
        CSF_AT(uint16_t, self, -38) = wordValue;
        if (changed)
        {
            C_751990_SetFlagAndReturnTrue(self - 42, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        uint8_t byteValue = CSF_AT(uint8_t, self, -612);
        changed = byteValue != CSF_AT(uint8_t, self, -610);
        CSF_AT(uint8_t, self, -610) = byteValue;
        if (changed)
        {
            C_74B200_SetFlagAndReturnTrue(self - 612, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        byteValue = CSF_AT(uint8_t, self, -621);
        changed = byteValue != CSF_AT(uint8_t, self, -619);
        CSF_AT(uint8_t, self, -619) = byteValue;
        if (changed)
        {
            C_74B200_SetFlagAndReturnTrue(self - 621, nullptr);
            CSF_AT(uint8_t, self, 0x1D) = 1;
        }

        integerValue = CSF_AT(int32_t, self, -72);
        changed = integerValue != CSF_AT(int32_t, self, -64);
        CSF_AT(int32_t, self, -64) = integerValue;
        if (changed)
            C_0074b2a0_SetFlag(self - 72, nullptr);

        int32_t previousValue = CSF_AT(int32_t, self, -52);
        integerValue = CSF_AT(int32_t, self, -60);
        CSF_AT(int32_t, self, -52) = integerValue;
        if (integerValue != previousValue)
            C_0074b2a0_SetFlag(self - 60, nullptr);

        byteValue = CSF_AT(uint8_t, self, -624);
        changed = byteValue != CSF_AT(uint8_t, self, -622);
        CSF_AT(uint8_t, self, -622) = byteValue;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(self - 624, nullptr);

        byteValue = CSF_AT(uint8_t, self, -609);
        changed = byteValue != CSF_AT(uint8_t, self, -607);
        CSF_AT(uint8_t, self, -607) = byteValue;
        if (changed)
            C_74B200_SetFlagAndReturnTrue(self - 609, nullptr);

        wordValue = CSF_AT(uint16_t, self, -618);
        changed = wordValue != CSF_AT(uint16_t, self, -614);
        CSF_AT(uint16_t, self, -614) = wordValue;
        if (changed)
            C_751990_SetFlagAndReturnTrue(self - 618, nullptr);
    }

    int32_t trackedValue = CSF_AT(int32_t, self, -340);
    if (trackedValue != CSF_AT(int32_t, self, 0x6C))
    {
        CSF_AT(int32_t, self, 0x6C) = trackedValue;
        if (static_cast<uint8_t>(Sub_64B250(self - 1712, nullptr)))
            SetFlag20C(self - 600, nullptr);
        else
            CSF_AT(uint8_t, self, 0x68) = 1;
    }

    int result = CSF_AT(int8_t, self, -621);
    if (CSF_AT(int32_t, self, 0x70) != result)
    {
        CSF_AT(int32_t, self, 0x70) = result;
        CSF_AT(uint8_t, self, 0x74) = 1;
    }
    return result;
}

HOOK(0x007417E0, NetCogibleDDL_UpdateAndTrackChanges, "auto");
