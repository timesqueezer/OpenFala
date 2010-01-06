#include "particle/Emitters.h"
#include "particle/Affectorfactory.h"
#include "utility/Random.h"
#include "utility/ConfigData.h"
#include "utility/Variant.h"

namespace part
{
	const util::CHashKey CLineEmitter::END_POS("end_pos");
	const util::CHashKey CCircleEmitter::RADIUS("radius");

	CLineEmitter::CLineEmitter(const util::CConfigData &emitterScript) :
		IParticleEmitter(emitterScript)
	{
		emitterScript.GetVariant("line_end_pos").GetValue(mPosEnd);
	}

	void CLineEmitter::InitParticle(ParticlePtr &particle)
	{
		math::vec2f ws_pos = mPos + mPositionOffset;
		math::vec2f dir = this->mDirection.Rotate(util::Rand::Get()->Randomf(-this->mEmisionAngle, this->mEmisionAngle));
		dir.Normalize();
		math::vec2f ws_end_pos = ws_pos + this->mPosEnd;
		math::vec2f normalVec(ws_pos - ws_end_pos);
		math::float32 len = normalVec.Length();
		normalVec.Normalize();
		//len = static_cast<math::float32>(rand()%static_cast<math::int32>(len));
		len = util::Rand::Get()->Randomf(len);
		math::float32 speed = util::Rand::Get()->Randomf(mParticleSpeed-mParticleSpeedVariation, mParticleSpeed+mParticleSpeedVariation);

		math::CColorRGBA clr = RandomiseColor();

		particle->Init(ws_pos + (normalVec * len), dir, speed, util::Rand::Get()->Randomf(this->mLiveTime-mLiveTimeVariation, this->mLiveTime+mLiveTimeVariation), clr, 0, mScale);
	}

	void CLineEmitter::SetParam(const util::CHashKey &param, const util::CVariant &var)
	{
		var.GetValue(mPosEnd);
	}
	util::CVariant CLineEmitter::GetParam(const util::CHashKey &param)
	{
		return util::CVariant::NULLVar;
	}

	void CPointEmitter::InitParticle(ParticlePtr &particle)
	{
		math::vec2f ws_pos = mPos + mPositionOffset;
		math::vec2f dir = this->mDirection.Rotate(util::Rand::Get()->Randomf(-this->mEmisionAngle, this->mEmisionAngle));
		dir.Normalize();
		math::float32 speed = util::Rand::Get()->Randomf(mParticleSpeed-mParticleSpeedVariation, mParticleSpeed+mParticleSpeedVariation);

		math::CColorRGBA clr = RandomiseColor();

		particle->Init(ws_pos, dir, speed, util::Rand::Get()->Randomf(this->mLiveTime-mLiveTimeVariation, this->mLiveTime+mLiveTimeVariation), clr, 0, mScale);
	}

	CCircleEmitter::CCircleEmitter(const util::CConfigData &emitterScript) :
		IParticleEmitter(emitterScript)
	{
		emitterScript.GetVariant("radius").GetValue(mRadius);
	}

	void CCircleEmitter::InitParticle(ParticlePtr &particle)
	{
		math::vec2f ws_pos = mPos + mPositionOffset;
		math::vec2f dir = this->mDirection.Rotate(util::Rand::Get()->Randomf(-this->mEmisionAngle, this->mEmisionAngle));
		dir.Normalize();

		math::vec2f spawnPos;
		
		math::float32 randomNr = util::Rand::Get()->Randomf(0.f, math::TWO_PI);
		spawnPos.x = mRadius * math::mathf::Sin(randomNr);
		spawnPos.y = mRadius * math::mathf::Cos(randomNr);

		spawnPos += ws_pos;
		math::float32 speed = util::Rand::Get()->Randomf(mParticleSpeed-mParticleSpeedVariation, mParticleSpeed+mParticleSpeedVariation);

		math::CColorRGBA clr = RandomiseColor();


		particle->Init(spawnPos, dir, speed, util::Rand::Get()->Randomf(this->mLiveTime-mLiveTimeVariation, this->mLiveTime+mLiveTimeVariation), clr, 0, mScale);
	}

	void CCircleEmitter::SetParam(const util::CHashKey &param, const util::CVariant &var)
	{
		var.GetValue(mRadius);
	}

	util::CVariant CCircleEmitter::GetParam(const util::CHashKey &param)
	{
		return util::CVariant::NULLVar;
	}
}
