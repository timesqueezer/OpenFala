#ifndef _PARTICLE_AFFECTORS_H_
#define _PARTICLE_AFFECTORS_H_

#include <boost/smart_ptr.hpp>
#include "utility/HashFuncs.h"
#include "AffectorBase.h"
#include "math/Algebra.h"

#include "SFML/Graphics.hpp"

BEGIN_UTILITY_NAMESPACE
	class CConfigData;
END_UTILITY_NAMESPACE

namespace part
{
	class IParticleEmitter;
	class CParticle;

	class CScaleInAffector : public IParticleAffector
	{
	public:
		CScaleInAffector(math::float32 fadeTime, math::float32 beginScale, math::float32 endScale);
		CScaleInAffector(const util::CConfigData &affectorScript);

		void Update(boost::shared_ptr<CParticle> &particle, math::float32 dt);
	private:
		math::float32 mFadeTime;
		math::float32 mSpeed;
		math::float32 mMaxScale;
	};

	class CScaleOutAffector : public IParticleAffector
	{
	public:
		CScaleOutAffector(math::float32 fadeTime, math::float32 endScale);
		CScaleOutAffector(const util::CConfigData &affectorScript);

		void Update(boost::shared_ptr<CParticle> &particle, math::float32 dt);
	private:
		math::float32 mFadeTime;
		math::float32 mMinScale;
	};

	class CFadeInAffector : public IParticleAffector
	{
	public:
		CFadeInAffector(math::float32 fadeTime, math::float32 startAlpha, math::float32 endAlpha);
		CFadeInAffector(const util::CConfigData &affectorScript);

		void Update(boost::shared_ptr<CParticle> &particle, math::float32 dt);
	private:
		math::float32 mFadeTime;
		math::float32 mSpeed;
		math::float32 mMaxAlpha;
	};

	class CFadeOutAffector : public IParticleAffector
	{
	public:
		CFadeOutAffector(math::float32 fadeTime, math::float32 endAlpha);
		CFadeOutAffector(const util::CConfigData &affectorScript);

		void Update(boost::shared_ptr<CParticle> &particle, math::float32 dt);
	private:
		math::float32 mFadeTime;
		math::float32 mMinAlpha;
	};

	class CForceAffector : public IParticleAffector
	{
	public:
		enum ForceType
		{
			FORCE_ADD,
		};
		static ForceType StringToForceType(const std::string &str);
	public:
		
		static const util::CHashKey FORCE_TYPE;
		static const util::CHashKey FORCE_VEC;

		explicit CForceAffector(const math::vec2f &force);
		CForceAffector(const util::CConfigData &affectorScript);
		
		virtual void SetParam(const util::CHashKey &param, const util::CVariant &variant);

		void Update(boost::shared_ptr<CParticle> &particle, math::float32 dt);
	private:
		math::vec2f mForce;
		ForceType mType;
	};

	class CColorFaderAffector : public IParticleAffector
	{
	public:
		explicit CColorFaderAffector(const math::CColorRGBA &clr);
		CColorFaderAffector(const util::CConfigData &affectorScript);

		void Update(boost::shared_ptr<CParticle> &particle, math::float32 dt);

	private:
		math::CColorRGBA mToColor;
	};
}

#endif //_PARTICLE_AFFECTORS_H_
