#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_EndGroup_695260, 0x00695260, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CoreZlibLog_WriteInteger, 0x006952F0, (void* self, void* edx, int a2, char* a3));
CSF_FUNC(int, __fastcall, CoreZlibLog_BeginGroup, 0x00695470, (void* self, void* edx, char* a2, int a3, char a4));

// List links are stored as 32-bit addresses inside the node.
static uint8_t* NodeFromAddress(uint32_t address)
{
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(address));
}

// Walks the .BDD_MATERIALES list owned by the loader (head pointer at this+0x5C)
// and writes each node's integer payload (+0x0C) to the log object a2.
// Node flag byte at +0x11 marks the sentinel/head node; links are +0x00, +0x04, +0x08.
static int __fastcall GameBddLoader_LogMaterialDatabase(void* self, void* /*edx*/, S_00694cd0* a2)
{
    CoreZlibLog_BeginGroup(a2, nullptr, const_cast<char*>(CSF_STR(0x840DFC, ".BDD_MATERIALES")), 1, 0);

    uint8_t* listHead = NodeFromAddress(CSF_AT(uint32_t, self, 0x5C));
    uint8_t* node = NodeFromAddress(CSF_AT(uint32_t, listHead, 0));

    if (node != listHead)
    {
        do
        {
            CoreZlibLog_WriteInteger(a2, nullptr, static_cast<int>(CSF_AT(uint32_t, node, 0x0C)), nullptr);

            if (!CSF_AT(uint8_t, node, 0x11))
            {
                uint8_t* linkAt8 = NodeFromAddress(CSF_AT(uint32_t, node, 0x08));
                if (CSF_AT(uint8_t, linkAt8, 0x11))
                {
                    uint8_t* candidate = NodeFromAddress(CSF_AT(uint32_t, node, 0x04));
                    while (!CSF_AT(uint8_t, candidate, 0x11))
                    {
                        if (node != NodeFromAddress(CSF_AT(uint32_t, candidate, 0x08)))
                            break;
                        node = candidate;
                        candidate = NodeFromAddress(CSF_AT(uint32_t, candidate, 0x04));
                    }
                    node = candidate;
                }
                else
                {
                    node = linkAt8;
                    uint8_t* leftmost = NodeFromAddress(CSF_AT(uint32_t, linkAt8, 0x00));
                    while (!CSF_AT(uint8_t, leftmost, 0x11))
                    {
                        node = leftmost;
                        leftmost = NodeFromAddress(CSF_AT(uint32_t, leftmost, 0x00));
                    }
                }
            }
        } while (node != NodeFromAddress(CSF_AT(uint32_t, self, 0x5C)));
    }

    return static_cast<int>(CoreZlibLog_EndGroup_695260(a2, nullptr));
}

HOOK(0x51B6F0, GameBddLoader_LogMaterialDatabase, "auto");
