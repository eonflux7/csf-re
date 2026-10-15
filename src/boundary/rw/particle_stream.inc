CSF_FUNC(int,__cdecl,particleFindChunk,0x00629AE0,(Stream*,uint32_t,uint32_t*,uint32_t*));
CSF_FUNC(Stream*,__cdecl,particleChunkWrite,0x00629A80,(Stream*,uint32_t,uint32_t,uint32_t,uint16_t));
CSF_FUNC(rw::Matrix*,__cdecl,particleMatrixRead,0x0063BF20,(Stream*,rw::Matrix*));
ParticleEmitter* __cdecl standardClone(Atomic*,ParticleEmitter* s,ParticleEmitter* d) {
    auto* a=static_cast<EmitterStandard*>(particleProperty(s,1));auto* b=static_cast<EmitterStandard*>(particleProperty(d,1));
    if(a && b) { *b=*a;if(b->texture) { ++b->texture->refCount;textureNative(b->texture); } }
    for(auto p:{std::array<int,2>{2,64},{3,64},{8,116},{5,32},{4,16}}) {
        auto* from=particleProperty(s,p[0]);auto* to=particleProperty(d,p[0]);if(from && to) std::memcpy(to,from,p[1]);
    }
    auto* x=static_cast<EmitterTank*>(particleProperty(s,6));auto* y=static_cast<EmitterTank*>(particleProperty(d,6));
    if(x && y) { y->dataFlags=x->dataFlags;y->updateFlags=x->updateFlags;y->maxParticles=x->maxParticles;
        y->flags=x->flags;y->platformFlags=x->platformFlags;y->srcBlend=x->srcBlend;y->dstBlend=x->dstBlend;y->vertexAlpha=x->vertexAlpha; }
    return d;
}
ParticleEmitter* __cdecl standardStreamSize(Atomic*,ParticleEmitter* e,int* size) {
    *size=0;auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(s) {
        *size=156;if(s->texture) { auto* t=textureNative(s->texture);if(!t) return nullptr;*size+=t->streamGetSize()+12; }
    }
    for(auto p:{std::array<int,2>{2,64},{3,64},{8,128},{5,32},{4,16},{6,20}})
        if(particleProperty(e,p[0])) *size+=p[1];
    return e;
}
template<bool Read> ParticleEmitter* __cdecl standardStream(Atomic*,ParticleEmitter* e,Stream* stream) {
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));
    if(s) {
        // Pointer-bearing members occur only after this byte-compatible prefix.
        for(auto r:{std::array<int,2>{0,4},{4,4},{16,12},{28,12},{40,12},{52,4},{56,4},
                {68,4},{72,4},{76,4},{80,4},{84,4},{88,4},{92,12},{104,12},{116,12}})
            if(!propertyIO(stream,reinterpret_cast<char*>(s)+r[0],r[1],Read)) return nullptr;
        uint32_t color=(uint32_t(s->color.red)<<24)|(uint32_t(s->color.green)<<16)|(uint32_t(s->color.blue)<<8)|s->color.alpha;
        if(!propertyIO(stream,&color,4,Read)) return nullptr;
        if(Read) s->color={uint8_t(color>>24),uint8_t(color>>16),uint8_t(color>>8),uint8_t(color)};
        // UV0/UV1 and four reserved words (+84..+A3).
        if(!propertyIO(stream,&s->uv0,32,Read)) return nullptr;
        int hasTexture=s->texture ? 1 : 0;if(!propertyIO(stream,&hasTexture,4,Read)) return nullptr;
        if(Read) {
            if(hasTexture>0 && particleFindChunk(stream,6,nullptr,nullptr)) {
                auto* texture=textureStreamRead(stream);if(!texture) return nullptr;s->texture=texture;
            }
        } else if(hasTexture && !textureStreamWrite(s->texture,stream)) return nullptr;
        if((!Read || particleStreamVersion>=3) && !propertyIO(stream,&s->rotation,4,Read)) return nullptr;
    }
    for(int id:{2,3,8,5,4,6}) {
        void* p=particleProperty(e,id);if(!p) continue;
        if(id==6) {
            auto* t=static_cast<EmitterTank*>(p);
            for(auto* word:{&t->flags,&t->platformFlags,&t->srcBlend,&t->dstBlend,&t->vertexAlpha})
                if(!propertyIO(stream,word,4,Read)) return nullptr;
        } else if(id==8) {
            if(Read) {
                if(!particleFindChunk(stream,13,nullptr,nullptr) || !particleMatrixRead(stream,static_cast<rw::Matrix*>(p))) return nullptr;
            } else {
                auto m=payloadGet<rw::Matrix>(p);
                if((m.flags&3)==3) {
                    CSF_FUNC(rw::Matrix*,__cdecl,ortho,0x006246A0,(rw::Matrix*,const rw::Matrix*));ortho(&m,&m);
                }
                uint32_t words[13];std::memcpy(words,&m.right,12);std::memcpy(words+3,&m.up,12);
                std::memcpy(words+6,&m.at,12);std::memcpy(words+9,&m.pos,12);words[12]=m.flags&3;
                if(!particleChunkWrite(stream,13,64,0x37002,0x37) || !particleChunkWrite(stream,1,52,0x37002,0x37) || !propertyIO(stream,words,52,false)) return nullptr;
            }
            if(!propertyIO(stream,static_cast<char*>(p)+64,52,Read)) return nullptr;
        } else {
            int bytes=id==2 || id==3 ? 64 : id==5 ? 32 : 16;
            if(!propertyIO(stream,p,bytes,Read)) return nullptr;
        }
    }
    return e;
}
