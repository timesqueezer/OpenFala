#ifndef _PARTICLE_EMITTERS_H_
#define _PARTICLE_EMITTERS_H_

#include "EmitterBase.h"

namespace part
{
	//A test emitter that emits particles from a single point
	class CPointEmitter : public IParticleEmitter
	{
	public:
		CPointEmitter() { }
		CPointEmitter(const util::CConfigData &emitterScript) : IParticleEmitter(emitterScript) {};
		~CPointEmitter() { this->Clean(); }

	protected:
		virtual void InitParticle(ParticlePtr &particle);
	};

	//A test emitter that emits particles from a line
	class CLineEmitter : public IParticleEmitter
	{
	public:
		CLineEmitter() { }
		CLineEmitter(const util::CConfigData &emitterScript);
		~CLineEmitter() { this->Clean(); }

		static const util::CHashKey END_POS;

		virtual void SetParam(const util::CHashKey &param, const util::CVariant &var);
		virtual util::CVariant GetParam(const util::CHashKey &param);

	protected:
		virtual void InitParticle(ParticlePtr &particle);
		math::vec2f mPosEnd;
	};

	//A test emitter that emits particles from a circle
	class CCircleEmitter : public IParticleEmitter
	{
	public:
		CCircleEmitter() { }
		CCircleEmitter(const util::CConfigData &emitterScript);
		~CCircleEmitter() { this->Clean(); }

		static const util::CHashKey RADIUS;

		virtual void SetParam(const util::CHashKey &param, const util::CVariant &var);
		virtual util::CVariant GetParam(const util::CHashKey &param);

	protected:
		virtual void InitParticle(ParticlePtr &particle);
		math::float32 mRadius;
	};
}

#endif //_PARTICLE_EMITTERS_H_
