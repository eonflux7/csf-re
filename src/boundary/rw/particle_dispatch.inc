CSF_FUNC(Frame*,__stdcall,particleFrameCreate,0x006235D0,());
CSF_FUNC(Atomic*,__cdecl,particleSetFrame,0x0058F300,(Atomic*,Frame*));
CSF_FUNC(Frame*,__cdecl,particleFrameUpdate,0x006236A0,(Frame*));
CSF_FUNC(rw::Matrix*,__cdecl,particleFrameLTM,0x00623720,(Frame*));
CSF_FUNC(Material*,__cdecl,particleMaterialTexture,0x00591560,(Material*,Texture*));

bool bindTankCursors(EmitterTank* p) {
    if(!p || !p->input || !p->atomic) return false;
    auto* t=getTank(p->atomic);if(!t) return false;
    p->dataFlags=0;int index=0;
    for(uint32_t flag=1;flag<=256;flag<<=1) {
        TankLock descriptor{};if(!tankLock(p->atomic,&descriptor,flag,0x40000000)) continue;
        p->input[index]=p->output[index]=static_cast<unsigned char*>(descriptor.data);
        p->strides[index++]=descriptor.stride;p->dataFlags|=flag;
    }
    return index==t->format.clusterCount;
}
bool autoTank(ParticleEmitter* e,EmitterTank* p) {
    if(p->atomic) return p->input!=nullptr;
    auto* s=static_cast<EmitterStandard*>(particleProperty(e,1));if(!s) return false;
    uint32_t flags=particleProperty(e,8) ? 8 : 1;
    if(particleProperty(e,2)) flags|=2;
    if(s->texture) flags|=particleProperty(e,3) ? 128 : 0x80000;
    if((flags&1) && particleProperty(e,5)) flags|=4;
    p->flags&=~0x20000;
    if(flags&1) {
        if(particleProperty(e,4)) flags|=32;
        else if(s->rotation>0.00001f || s->rotation< -0.00001f) { flags|=0x20000;p->flags|=0x20000; }
    }
    auto* a=tankCreate(s->maxParticles,flags,p->updateFlags);if(!a) return false;
    auto* frame=particleFrameCreate();if(!frame) { tankDestroy(a);return false; }
    frame->matrix.setIdentity();particleSetFrame(a,frame);
    auto* t=getTank(a);
    if(!(flags&2)) { std::memcpy(t->tail+8,&s->color,4);a->geometry->matList.materials[0]->color=s->color; }
    if(flags&0x80000) std::memcpy(t->tail+13,&s->uv0,16);
    if(!(flags&4)) std::memcpy(t->tail+5,&s->size,8);
    if(s->texture) particleMaterialTexture(a->geometry->matList.materials[0],s->texture);
    int update=p->updateFlags;
    if(!emitterSetTank(e,a)) {
        if(p->atomic!=a) {
            CSF_FUNC(int,__cdecl,freeFrame,0x00623B10,(Frame*));particleSetFrame(a,nullptr);freeFrame(frame);tankDestroy(a);
        }
        return false;
    }
    p->updateFlags=update;t->tail[0]=p->srcBlend;t->tail[1]=p->dstBlend;t->tail[2]=p->vertexAlpha;return true;
}
#if defined(__clang__)
__attribute__((no_sanitize("function")))
#endif
ParticleEmitter* emitterUpdate(Atomic* a,ParticleEmitter* e,void* delta) {
    if(!(e->flags&2)) return e;
    if(!e->particleClass) return nullptr;
    auto* tank=(e->flags&4) ? static_cast<EmitterTank*>(particleProperty(e,6)) : nullptr;
    if((e->flags&4) && (!tank || !autoTank(e,tank))) return nullptr;
    if((e->flags&0x10) && !invokeEmitter(e,1,a,delta)) return nullptr;
    e->boundingRadiusSquared=0;
    bool locked=false;int count=0;
    if(e->activeBatches && (e->flags&0x20)) {
        if(tank) { if(!bindTankCursors(tank)) return nullptr;tank->numParticles=0;locked=true; }
        // The original rebuilds this ring while visiting the detached one.
        auto* old=e->activeBatches;auto* first=old->next;auto* b=first;
        e->activeBatches=nullptr;
        do {
            auto* next=b->next;b->newParticle=-1;
            auto restoreRemaining=[&]() {
                ringInsert(e->activeBatches,b);
                for(auto* rest=next;rest!=first;) { auto* n=rest->next;ringInsert(e->activeBatches,rest);rest=n; }
            };
            if(b->numParticles>0) {
                auto* c=b->particleClass;
                for(int i=0;i<c->numCallbackSets;i++) {
                    auto cb=reinterpret_cast<BatchCallback>(c->callbacks[7*i]);
                    if(cb && cb(e,b,delta)!=b) {
                        // Preserve ownership of the unvisited ring on failure.
                        restoreRemaining();
                        return nullptr;
                    }
                }
                count+=b->numParticles;ringInsert(e->activeBatches,b);
            } else {
                if(!invokeBatch(e,b,3)) { restoreRemaining();return nullptr; }
                ringInsert(e->inactiveBatches,b);
            }
            b=next;
        } while(b!=first);
    } else count=e->numParticles;
    e->numParticles=count;
    if(tank && !locked) {
        if(!bindTankCursors(tank)) return nullptr;
        for(int i=0;i<getTank(tank->atomic)->format.clusterCount;i++) {
            tank->input[i]+=size_t(tank->numParticles)*tank->strides[i];
            tank->output[i]+=size_t(tank->numParticles)*tank->strides[i];
        }
        locked=true;
    }
    if((e->flags&8) && !invokeEmitter(e,0,a,delta)) return nullptr;
    if(tank && locked) {
        auto* t=getTank(tank->atomic);t->numParticles=tank->numParticles;t->dirty|=0x800000;tankUnlock(tank->atomic);
    }
    if((e->flags&0x10) && !invokeEmitter(e,2,a,delta)) return nullptr;
    if(e->boundingRadiusSquared>0) e->boundingRadiusSquared=float(particleSqrt(e->boundingRadiusSquared));
    if(e->boundingRadiusSquared>a->boundingSphere.radius) a->boundingSphere.radius=e->boundingRadiusSquared;
    return e;
}
Atomic* __cdecl particleAtomicUpdate(Atomic* a,void* delta) {
    auto* tail=atomicEmitter(a);if(!tail || !delta) return nullptr;
    a->boundingSphere.radius=0;auto* first=tail->next;auto* e=first;bool ok=true;
    do { auto* next=e->next;if(emitterUpdate(a,e,delta)!=e) { ok=false;break; }e=next; } while(e!=first);
    if(a->geometry && a->geometry->numMorphTargets) a->geometry->morphTargets[0].boundingSphere=a->boundingSphere;
    a->object.object.privateFlags|=1;return ok ? a : nullptr;
}
Atomic* __cdecl particleAtomicRender(Atomic* a) {
    auto* tail=atomicEmitter(a);if(!tail) return a;
    auto* first=tail->next;auto* e=first;
    do {
        auto* next=e->next;
        if(e->flags&2) {
            if((e->flags&0x40) && !invokeEmitter(e,3,a,nullptr)) return nullptr;
            if((e->flags&0x80) && e->activeBatches) {
                auto* b=e->activeBatches->next;auto* begin=b;
                do { auto* n=b->next;if(!invokeBatch(e,b,1)) return nullptr;b=n; } while(b!=begin);
            }
            if((e->flags&0x40) && !invokeEmitter(e,4,a,nullptr)) return nullptr;
        }
        e=next;
    } while(e!=first);
    return a;
}
Atomic* __cdecl particleAtomicCreate(ParticleClass* c) {
    auto* e=particleEmitterCreate(c,nullptr);if(!e) return nullptr;
    auto* g=geometryCreate(0,0,0);if(!g) { particleEmitterDestroy(e);return nullptr; }
    auto* a=tankAtomicCreate();if(!a) { tankGeometryDestroy(g);particleEmitterDestroy(e);return nullptr; }
    int off=plugins(Registry::Atomic).getPluginOffset(304);
    if(off<0 || !tankSetGeometry(a,g,0)) { tankGeometryDestroy(g);tankAtomicDestroy(a);particleEmitterDestroy(e);return nullptr; }
    tankGeometryDestroy(g);*reinterpret_cast<ParticleEmitter**>(reinterpret_cast<char*>(a)+off)=e;
    e->atomic=a;a->renderCB=particleAtomicRender;return a;
}
ParticleBatch* __cdecl subEmitterUpdate(ParticleEmitter* e,ParticleBatch* batch,void* dt) {
    auto* config=particleProperty(e,0x1000001);auto* tank=static_cast<EmitterTank*>(particleProperty(e,6));
    if(!config || !tank || !e->atomic || !e->particleClass) return batch;
    auto* child=payloadGet<ParticleEmitter*>(config,16);if(!child) { child=e->next;payloadSet(config,16,child); }
    auto* standard=static_cast<EmitterStandard*>(particleProperty(child,1));if(!standard) return nullptr;
    auto* frame=static_cast<Frame*>(e->atomic->object.object.parent);if(!frame) return nullptr;
    int sub=propOffset(e->particleClass->properties,0x1000001),velocity=propOffset(e->particleClass->properties,6);
    if(sub<0 || velocity<0 || !(tank->dataFlags&1) || !tank->input || !tank->input[0]) return nullptr;
    rw::Matrix saved=frame->matrix;
    auto* p=reinterpret_cast<unsigned char*>(batch)+batch->dataOffset;auto* position=tank->input[0];
    for(int i=0;i<batch->numParticles;i++,p+=e->particleClass->objectSize,position+=tank->strides[0]) {
        float age=payloadGet<float>(p,4),death=payloadGet<float>(p,8),next=payloadGet<float>(p,sub);
        if(!(next<death) || !(age>next)) continue;
        rw::V3d direction=payloadGet<rw::V3d>(p,velocity);particleNormalize(&direction,&direction);
        auto x=std::fabs(direction.x),y=std::fabs(direction.y),z=std::fabs(direction.z);rw::V3d axis{0,0,0};
        if(x>y) { if(y>z) axis.z=1;else axis.y=1; } else if(x>z) axis.z=1;else axis.x=1;
        rw::Matrix m{};m.right=particleCross(axis,direction);m.up=particleCross(direction,m.right);m.at=direction;m.pos=payloadGet<rw::V3d>(position);
        frame->matrix=m;particleFrameUpdate(frame);standard->ltm=particleFrameLTM(frame);
        float time=standard->currentTime,schedule=standard->nextEmit;
        standard->currentTime=standard->nextEmit=age;child->flags=(child->flags&~0x128u)|0x108;
        emitterUpdate(e->atomic,child,dt);child->flags=(child->flags&~0x128u)|0x20;
        standard->currentTime=time;standard->nextEmit=schedule;
        payloadSet<float>(p,sub,next+payloadGet<float>(p,sub+4));
    }
    frame->matrix=saved;particleFrameUpdate(frame);particleFrameLTM(frame);return batch;
}
