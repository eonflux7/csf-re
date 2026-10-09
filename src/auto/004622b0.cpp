#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Callee: __thiscall, 5 stack args (ret 20). self = 36-byte draw command buffer.
CSF_FUNC(uint32_t*, __fastcall, InitDrawCommandRecord, 0x00445FE0, (void* self, void* edx, int a2, int a3, uint32_t* a4, int a5, int a6));

// Callee: __thiscall, 1 stack arg (ret 4). self = scene render queue object.
CSF_FUNC(uint32_t*, __fastcall, RenderQueuePushRecord, 0x00446D30, (void* self, void* edx, uint32_t* a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __stdcall SubmitPositionedDrawCommand(int /*unusedA1*/, int /*unusedA2*/, uint32_t* argRefs)
{
    // Draw mode selector: argRefs[0] -> +4 -> +4
    uint32_t firstRef = argRefs[0];
    uint32_t modeHolder = *reinterpret_cast<uint32_t*>(firstRef + 4);
    uint32_t modeValue = *reinterpret_cast<uint32_t*>(modeHolder + 4);

    int drawModeMask = 0;
    switch (modeValue) {
        case 0:
            drawModeMask = 0;
            break;
        case 1:
            drawModeMask = 2;
            break;
        case 2:
            drawModeMask = 4;
            break;
        case 3:
            drawModeMask = 8;
            break;
        default:
            break;
    }

    // Position record: argRefs[1] -> +4 -> +4, three dwords copied by value
    uint32_t positionHolder = *reinterpret_cast<uint32_t*>(argRefs[1] + 4);
    uint32_t* positionRecord = reinterpret_cast<uint32_t*>(positionHolder + 4);
    uint32_t positionCopy[3];
    positionCopy[0] = positionRecord[0];
    positionCopy[1] = positionRecord[1];
    positionCopy[2] = positionRecord[2];

    // Trailing int args: argRefs[3] chain is read first, then argRefs[2] chain
    uint32_t extraArgBHolder = *reinterpret_cast<uint32_t*>(argRefs[3] + 4);
    int extraArgB = static_cast<int>(*reinterpret_cast<uint32_t*>(extraArgBHolder + 4));
    uint32_t extraArgAHolder = *reinterpret_cast<uint32_t*>(argRefs[2] + 4);
    int extraArgA = static_cast<int>(*reinterpret_cast<uint32_t*>(extraArgAHolder + 4));

    // Tagged object header: vtable tag followed by two -1 words
    uint32_t commandRecord[3];
    commandRecord[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    commandRecord[1] = 0xFFFFFFFFu;
    commandRecord[2] = 0xFFFFFFFFu;

    uint8_t drawCommand[36];
    InitDrawCommandRecord(drawCommand, nullptr,
                          drawModeMask,
                          reinterpret_cast<int>(commandRecord),
                          positionCopy,
                          extraArgA,
                          extraArgB);

    // Push onto the current scene's render queue (pointer stored at +0xB0)
    void* sceneRenderQueue = CSF_AT(void*, g_pCurrentScene, 0xB0);
    return reinterpret_cast<int>(RenderQueuePushRecord(sceneRenderQueue, nullptr,
                                                       reinterpret_cast<uint32_t*>(drawCommand)));
}

HOOK(0x004622B0, SubmitPositionedDrawCommand, "auto");
