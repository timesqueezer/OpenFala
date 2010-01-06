#include "particle/Affectors.h"
#include "particle/EmitterBase.h"
#include "utility/Text.h"
#include "utility/ConfigData.h"
#include "utility/Variant.h"

namespace part
{
	//ScaleIn
	CScaleInAffector::CScaleInAffector(math::float32 fadeTime, math::float32 beginScale, math::float32 endScale) :
		mFadeTime(fadeTime), mSpeed(0), mMaxScale(endScale)
	{ 
		math::float32 scaleDif = mMaxScale - beginScale;
		mSpeed = scaleDif/mFadeTime;
	}

	CScaleInAffector::CScaleInAffector(const util::CConfigData &affectorScript)
	{
		affectorScript.GetVariant("fade_time").GetValue(mFadeTime);
		affectorScript.GetVariant("end_scale").GetValue(mMaxScale);

		math::float32 beginScale;

		affectorScript.GetVariant("begin_scale").GetValue(beginScale);

		math::float32 scaleDif = mMaxScale - beginScale;
		mSpeed = scaleDif/mFadeTime;
	}

	void CScaleInAffector::Update(boost::shared_ptr<CParticle> &particle, math::float32 dt)
	{
		if(particle->GetStartLiveTime()-mFadeTime < particle->GetLiveTime())
		{
			particle->SetScale(particle->GetScale() + mSpeed*dt);
			if(particle->GetScale() > mMaxScale) 
				particle->SetScale(mMaxScale);
		}
	}

	//ScaleOut
	CScaleOutAffector::CScaleOutAffector(math::float32 fadeTime, math::float32 endScale) :
		mFadeTime(fadeTime), mMinScale(endScale)
	{
		
	}

	CScaleOutAffector::CScaleOutAffector(const util::CConfigData &affectorScript)
	{
		affectorScript.GetVariant("fade_time").GetValue(mFadeTime);
		affectorScript.GetVariant("end_scale").GetValue(mMinScale);
	}

	void CScaleOutAffector::Update(boost::shared_ptr<CParticle> &particle, math::float32 dt)
	{
		if(particle->GetLiveTime()-mFadeTime < 0.f)
		{
			particle->SetScale(particle->GetScale() - (particle->GetScale()/particle->GetLiveTime())*dt);
		}
	}

	//Force

	const util::CHashKey CForceAffector::FORCE_TYPE("force_type");
	const util::CHashKey CForceAffector::FORCE_VEC("force_vec");

	CForceAffector::ForceType CForceAffector::StringToForceType(const std::string &str)
	{
		std::string lowercase = util::CText::StringToLower(str);
		if(lowercase == "add")
			return CForceAffector::FORCE_ADD;

		ASSERT_MSG(false, "Ther is no such force type defined");
		return CForceAffector::FORCE_ADD;
	}

	CForceAffector::CForceAffector(const math::vec2f &force) : 
		mForce(force), mType(CForceAffector::FORCE_ADD)
	{
		
	}

	CForceAffector::CForceAffector(const util::CConfigData &affectorScript)
	{
		mType = CForceAffector::StringToForceType(affectorScript.GetVariant("force_type").GetStringValue());
		affectorScript.GetVariant("force").GetValue(mForce);
	}

	void CForceAffector::Update(boost::shared_ptr<CParticle> &particle, math::float32 dt)
	{
		math::vec2f current = particle->GetDir() * particle->GetSpeed();
		math::vec2f force = mForce * dt;
		math::vec2f accumulated;
		switch(mType)
		{
		case FORCE_ADD:
			accumulated = current + force;
			break;
		default:
			ASSERT_MSG(false, "Unknown forcetype");
		}
		particle->SetDir( accumulated.NormalizeCopy() );
		particle->SetSpeed( math::mathf::FastSqrt(accumulated.LengthSquared()) );
	}

	void CForceAffector::SetParam(const util::CHashKey &param, const util::CVariant &variant)
	{
		if(param == CForceAffector::FORCE_TYPE)
		{
			math::int32 val;
			variant.GetValue(val);
			mType = static_cast<CForceAffector::ForceType>(val);
		}
		else if(param == CForceAffector::FORCE_VEC)
		{
			variant.GetValue(mForce);
		}
	}


	//FadeIn
	CFadeInAffector::CFadeInAffector(math::float32 fadeTime, math::float32 startAlpha, math::float32 endAlpha) :
		mFadeTime(fadeTime), mMaxAlpha(endAlpha)
	{
		math::float32 fadeDif = endAlpha - startAlpha;
		mSpeed = fadeDif/fadeTime;
	}

	CFadeInAffector::CFadeInAffector(const util::CConfigData &affectorScript)
	{
		affectorScript.GetVariant("fade_time").GetValue(mFadeTime);
		math::float32 alpha = 0;
		affectorScript.GetVariant("end_alpha").GetValue(alpha);
		mMaxAlpha = alpha;
		math::float32 startAlpha = 0;
		affectorScript.GetVariant("start_alpha").GetValue(startAlpha);
		math::float32 fadeDif = mMaxAlpha - startAlpha;
		mSpeed = fadeDif/mFadeTime;
	}

	void CFadeInAffector::Update(boost::shared_ptr<CParticle> &particle, math::float32 dt)
	{
		math::float32 alpha_beg = particle->GetAlpha();
		if(/*particle->GetStartLiveTime()-mFadeTime < particle->GetLiveTime()*/particle->GetAlpha() < mMaxAlpha)
		{
			math::float32 alpha = mSpeed*dt;
			particle->SetAlpha(particle->GetAlpha() + alpha);
			if(particle->GetAlpha() > mMaxAlpha)
				particle->SetAlpha(mMaxAlpha);
		}
	}

	//FadeOut
	CFadeOutAffector::CFadeOutAffector(math::float32 fadeTime, math::float32 endAlpha) :
		mFadeTime(fadeTime), mMinAlpha(endAlpha)
	{

	}

	CFadeOutAffector::CFadeOutAffector(const util::CConfigData &affectorScript)
	{
		affectorScript.GetVariant("fade_time").GetValue(mFadeTime);
		math::float32 alpha = 0;
		affectorScript.GetVariant("end_alpha").GetValue(alpha);
		mMinAlpha = alpha;
	}

	void CFadeOutAffector::Update(boost::shared_ptr<CParticle> &particle, math::float32 dt)
	{
		if(particle->GetLiveTime()-mFadeTime < 0.f)
		{
			math::float32 alpha = particle->GetAlpha() - (particle->GetAlpha()/particle->GetLiveTime())*dt;
			particle->SetAlpha(alpha);
			if(this->mMinAlpha > alpha)
				particle->SetAlpha(mMinAlpha);
		}
	}

	//Color Fader
	CColorFaderAffector::CColorFaderAffector(const math::CColorRGBA &clr) :
		mToColor(clr)
	{}

	CColorFaderAffector::CColorFaderAffector(const util::CConfigData &affectorScript)
	{
		affectorScript.GetVariant("to_color").GetValue(mToColor);
	}

	void CColorFaderAffector::Update(boost::shared_ptr<CParticle> &particle, math::float32 dt)
	{
		math::float32 scalar = abs(particle->GetLiveTime() - particle->GetStartLiveTime()) / particle->GetStartLiveTime();
		particle->SetColor(math::CMath<math::CColorRGBA>::Lerp(particle->GetStartColor(), this->mToColor, scalar));
	}
}
