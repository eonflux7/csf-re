# RW offset evidence

This is a conservative grep audit, NOT complete interprocedural type recovery. Boundary-local taints (returns or simple arguments) and name-only candidates are deliberately separate. A file is counted once per offset. Reassignments/aliases need manual review; these are not a reaching-definition proof. For exact carriers and matches see RW_PORT_OFFSETS.csv. Offsets that refer to the game wrapper rather than an RW object are possible in the candidate column.

| Structure | Boundary locals: offset (files) | Name-only candidates: offset (files) |
|---|---|---|
| RpAtomic | `0x4` (3), `0x18` (2), `0x48` (1) | `0x0` (2), `0x4` (9), `0xc` (2), `0x18` (16), `0x28` (3), `0x48` (7), `0x50` (4), `0x70` (1) |
| RpClump | `0x4` (10) | `0x0` (18), `0x4` (15), `0x8` (2), `0xc` (1), `0x9c` (1), `0xa0` (1), `0xa4` (1), `0xa8` (1), `0xac` (1), `0xb0` (1) |
| RpGeometry | `0x8` (1) | `0x0` (15), `0x4` (1), `0x8` (2), `0xb` (2), `0xc` (1), `0x10` (2), `0x14` (1), `0x18` (3), `0x1c` (1), `0x20` (15), `0x24` (7), `0x28` (2), `0x2c` (4), `0x30` (3), `0x34` (1), `0x40` (1), `0x5c` (2), `0x60` (1), `0x64` (1) |
| RpHAnimHierarchy | none | `0x0` (2), `0x4` (11), `0x10` (9), `0x14` (2), `0x20` (13), `0x24` (1), `0x30` (1), `0x34` (1), `0x38` (1) |
| RpLight | none | `0x0` (17), `0x1` (2), `0x4` (8), `0x8` (5), `0xc` (5), `0x14` (3), `0x18` (1), `0x1c` (2), `0x20` (3), `0x24` (2), `0x28` (1) |
| RpMaterial | `0x0` (2), `0x4` (2) | `0x0` (9), `0x4` (5), `0x8` (1), `0x10` (1), `0x18` (3), `0x1c` (1), `0x20` (2), `0x24` (1), `0x28` (2), `0x2c` (1), `0x34` (1), `0x44` (1), `0x48` (1), `0x4c` (1), `0x70` (1), `0x74` (1), `0x78` (1), `0x7c` (1), `0x80` (1) |
| RpSkin | none | `0x0` (18), `0x18` (1), `0xf0` (1) |
| RpWorld | `0x8` (1) | none |
| RpWorld/sector | none | `0x0` (20), `0x4` (13), `0x6` (1), `0x8` (10), `0xb` (3), `0xc` (9), `0x10` (9), `0x14` (2), `0x18` (1), `0x1c` (2), `0x20` (8), `0x24` (1), `0x25` (1), `0x28` (9), `0x30` (3), `0x34` (3), `0x38` (2), `0x3c` (2), `0x40` (3), `0x44` (1), `0x48` (2), `0x4c` (2), `0x50` (5), `0x54` (5), `0x58` (5), `0x5c` (5), `0x60` (3), `0x64` (3), `0x80` (1), `0x82` (1), `0x84` (2), `0x91` (1), `0xb4` (2), `0xb8` (1), `0xc0` (2), `0xc4` (1), `0xc8` (1), `0xcc` (1), `0xd4` (1), `0xf4` (1), `0xf8` (1), `0x100` (1), `0x108` (1), `0x110` (1), `0x118` (1), `0x130` (1), `0x134` (1), `0x138` (1), `0x13c` (1), `0x140` (1), `0x144` (1), `0x148` (1), `0x15c` (1), `0x170` (1), `0x180` (1), `0x184` (1), `0x19c` (1), `0x1ac` (1), `0x1b0` (1), `0x1f0` (1), `0x220` (1), `0x224` (1), `0x228` (1), `0x310` (1), `0x33c` (1), `0x1fd8` (1) |
| RtAnimAnimation | `0x10` (2) | `0x0` (34), `0x4` (6), `0x8` (4), `0xc` (7), `0xd` (1), `0xe` (1), `0x10` (10), `0x14` (5), `0x18` (3), `0x1c` (3), `0x1d` (1), `0x20` (6), `0x24` (9), `0x28` (2), `0x29` (1), `0x2a` (1), `0x2b` (1), `0x2c` (2), `0x30` (2), `0x34` (5), `0x38` (1), `0x3c` (1), `0x40` (1), `0x44` (1), `0x48` (1), `0x54` (1), `0x58` (5), `0x59` (2), `0x5a` (2), `0x5b` (1), `0x5c` (1), `0x5d` (1), `0x1c4` (1), `0x1d8` (1), `0x1e0` (1) |
| RtAnimInterpolator | none | `0x0` (5), `0x4` (4), `0x24` (1) |
| RwCamera | `0x4` (1) | `0x0` (14), `0x4` (6), `0x8` (2), `0xc` (1), `0x10` (3), `0x14` (3), `0x18` (2), `0x1c` (3), `0x20` (4), `0x24` (2), `0x28` (3), `0x2c` (1), `0x30` (1), `0x34` (1), `0x38` (1), `0x3c` (1), `0x40` (2), `0x44` (2), `0x48` (2), `0x60` (3), `0x64` (5) |
| RwFrame | `0x4` (3), `0x10` (3), `0x14` (3), `0x18` (3), `0x1c` (3), `0x20` (3), `0x24` (3), `0x28` (3), `0x30` (3), `0x34` (3), `0x38` (3), `0x40` (3), `0x44` (3), `0x48` (3) | `0x0` (29), `0x2` (2), `0x4` (36), `0x8` (11), `0xa` (1), `0xc` (8), `0xe` (2), `0x10` (10), `0x12` (2), `0x14` (11), `0x18` (16), `0x1c` (12), `0x20` (12), `0x21` (1), `0x22` (1), `0x24` (9), `0x28` (4), `0x2c` (1), `0x30` (6), `0x34` (6), `0x38` (7), `0x3c` (2), `0x40` (7), `0x44` (5), `0x48` (4), `0x50` (2), `0x7c` (1), `0x8c` (1), `0x90` (2), `0x98` (2), `0xab` (1) |
| RwImage | `0x4` (2), `0x8` (2), `0xc` (1), `0x10` (1), `0x14` (2) | `0x0` (1), `0x4` (3), `0x8` (3), `0xc` (2), `0x10` (1), `0x14` (2) |
| RwMatrix | `0x0` (6), `0x4` (5), `0x8` (5), `0xc` (1), `0x10` (3), `0x14` (4), `0x18` (3), `0x20` (5), `0x24` (5), `0x28` (6), `0x30` (5), `0x34` (5), `0x38` (5) | `0x0` (118), `0x4` (122), `0x8` (34), `0xc` (22), `0x10` (16), `0x14` (17), `0x18` (11), `0x20` (33), `0x24` (33), `0x28` (35), `0x30` (23), `0x34` (22), `0x38` (21), `0x44` (1), `0x48` (1), `0x4c` (1), `0x50` (1) |
| RwRaster | `0xc` (3), `0x10` (3), `0x14` (1), `0x23` (1) | `0xc` (8), `0x10` (9), `0x14` (6), `0x23` (3) |
| RwTexture | `0x30` (1), `0x31` (1), `0x34` (1), `0x38` (1), `0x3c` (1), `0x40` (1), `0x44` (1), `0x48` (1), `0x4c` (1), `0x50` (4), `0x51` (1), `0x54` (3) | `0x0` (34), `0x4` (5), `0x8` (2), `0xc` (2), `0x14` (1), `0x18` (2), `0x30` (1), `0x31` (1), `0x34` (1), `0x38` (1), `0x3c` (1), `0x40` (1), `0x44` (1), `0x48` (1), `0x4c` (1), `0x50` (4), `0x51` (1), `0x54` (6) |

## Direct device vtable grep

These are candidates, not an exhaustive device-flow proof. The manually confirmed methods are in RW_PORT.md.

- `0045f9a0.cpp`: `CSF_AT(void**, inputDevice, 0)[2]`
- `0045f9a0.cpp`: `CSF_AT(void**, targetComponent, 0)[42]`
- `004d3d30.cpp`: `CSF_AT(void**, provider, 0)[0x3C / 4]`
- `004d4680.cpp`: `CSF_AT(void**, skin, 0)[0x3C / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, target, 0)[0x28 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, owner, 0)[0x80 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, materialTable, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, factory, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x40 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x28 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x10 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, factory, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x40 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x28 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, parameterTable, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, registry, 0)[0x38 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, registry, 0)[0x3C / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, soundObject, 0)[0xC4 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, definitions, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, factory, 0)[0x8 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x40 / 4]`
- `0051d3d0.cpp`: `CSF_AT(void**, effect, 0)[0x28 / 4]`
- `00526780.cpp`: `CSF_AT(void**, voice, 0)[0x28 / 4]`
- `00526780.cpp`: `CSF_AT(void**, voice, 0)[0x28 / 4]`
- `005268c0.cpp`: `CSF_AT(void**, globalObjA, 0)[0x4C / 4]`
- `005268c0.cpp`: `CSF_AT(void**, globalObjB, 0)[0x44 / 4]`
- `005268c0.cpp`: `CSF_AT(void**, voice, 0)[0x1C / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[8 / 4]`
- `00527630.cpp`: `CSF_AT(void**, manager, 0)[0xC / 4]`
- `00527630.cpp`: `CSF_AT(void**, device, 0)[0x6C / 4]`
- `00527630.cpp`: `CSF_AT(void**, device, 0)[0x84 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x4 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x54 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x40 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x40 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x4C / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x1C / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x14 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x38 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x14 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x14 / 4]`
- `00527630.cpp`: `CSF_AT(void**, voice, 0)[0x38 / 4]`
- `00527dc0.cpp`: `CSF_AT(void**, audioDevice, 0)[0x34 / 4]`
- `00527dc0.cpp`: `CSF_AT(void**, voiceObject, 0)[0x8 / 4]`
- `006bd3d0.cpp`: `CSF_AT(void**, self, 0)[0x38 / 4]`
- `006bd3d0.cpp`: `CSF_AT(void**, clumpManager, 0)[0x88 / 4]`
- `006bd3d0.cpp`: `CSF_AT(void**, sceneController, 0)[0x38 / 4]`
- `006cd0e0.cpp`: `CSF_AT(void**, renderResources, 0)[1]`
- `006cd0e0.cpp`: `CSF_AT(void**, worldResources, 0)[1]`
- `006cdaa0.cpp`: `CSF_AT(void**, accessor, 0)[0x10 / 4]`
- `006cdaa0.cpp`: `CSF_AT(void**, textureManager, 0)[0x10 / 4]`
- `006cdaa0.cpp`: `CSF_AT(void**, world, 0)[0x2C / 4]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[32]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[4]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[22]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[21]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[22]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[14]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[16]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[8]`
- `006d3db0.cpp`: `CSF_AT(void**, cameraObject, 0)[24]`
- `006d3db0.cpp`: `CSF_AT(void**, serializer, 0)[31]`
- `006dc2c0.cpp`: `CSF_AT(void**, device, 0)[0x5C / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, texture, 0)[0x48 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, device, 0)[0x84 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, surface, 0)[0x34 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, surface, 0)[0x38 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, surface, 0)[0x8 / 4]`
- `006dc2c0.cpp`: `CSF_AT(void**, texture, 0)[0x8 / 4]`
- `006dc560.cpp`: `CSF_AT(void**, device, 0)[0x54 / 4]`
- `006dc750.cpp`: `deviceVtable[0x1C / 4]`
- `006dc9b0.cpp`: `CSF_AT(void**, renderDevice, 0)[0x74 / 4]`
- `006e4e90.cpp`: `CSF_AT(void**, controller, 0)[8 / 4]`
- `006e4e90.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `006e4e90.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `006f61b0.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `006f61b0.cpp`: `CSF_AT(void**, device, 0)[0xE4 / 4]`
- `00709810.cpp`: `CSF_AT(void**, file, 0)[7]`
- `00709810.cpp`: `CSF_AT(void**, file, 0)[2]`
- `00709810.cpp`: `CSF_AT(void**, obj, 0)[3]`
- `00709810.cpp`: `CSF_AT(void**, obj, 0)[2]`
- `00709bc0.cpp`: `CSF_AT(void**, owner, 0)[2]`
- `00709bc0.cpp`: `CSF_AT(void**, lightingSource, 0)[9]`
- `00709bc0.cpp`: `CSF_AT(void**, frameProvider, 0)[35]`
- `0070d700.cpp`: `CSF_AT(void**, oldCamera, 0)[4 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, reflectionCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[148 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[140 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, reflectionCamera, 0)[16 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, frame, 0)[148 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, sourceCamera, 0)[36 / 4]`
- `0070d700.cpp`: `CSF_AT(void**, reflectionCamera, 0)[4 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x20 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x28 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x1C / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x70 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, controls, 0)[0x24 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x68 / 4]`
- `0070ed20.cpp`: `CSF_AT(void**, self, 0)[0x5C / 4]`
- `0070f630.cpp`: `CSF_AT(void**, file, 0)[8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `0070f630.cpp`: `CSF_AT(void**, self, 0)[0xA8 / 4]`
- `00710850.cpp`: `CSF_AT(void**, self, 0)[0xD0 / 4]`
- `00710df0.cpp`: `CSF_AT(void**, self, 0)[49]`
- `00710ff0.cpp`: `CSF_AT(void**, device, 0)[3]`
- `00710ff0.cpp`: `CSF_AT(void**, outBuffer, 0)[0]`
- `00710ff0.cpp`: `CSF_AT(void**, outBuffer, 0)[2]`
- `00712f00.cpp`: `deviceVtable[0x20 / 4]`
- `00712f00.cpp`: `deviceVtable[0x20 / 4]`
- `00712f80.cpp`: `CSF_AT(void**, device, 0)[0x18 / 4]`
- `00713030.cpp`: `CSF_AT(void**, device, 0)[8]`
- `007130b0.cpp`: `CSF_AT(void**, device, 0)[kDeviceSlotUnacquire]`
- `007130b0.cpp`: `CSF_AT(void**, device, 0)[kDeviceSlotAcquire]`
- `007130b0.cpp`: `CSF_AT(void**, device, 0)[kDeviceSlotAcquire]`
- `007132f0.cpp`: `CSF_AT(void**, input, 0)[12 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[44 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[52 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[24 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, keyboard, 0)[28 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, input, 0)[12 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, mouse, 0)[44 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, mouse, 0)[52 / 4]`
- `007132f0.cpp`: `CSF_AT(void**, mouse, 0)[28 / 4]`
