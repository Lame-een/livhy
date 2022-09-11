#include "pch.h"
#include "object.h"


Object::Object(bool cull, bool samplingTarget)
	: m_Cull(cull), m_SamplingTarget(samplingTarget)
{
}

Object::Object(const Material* mat, bool cull, bool samplingTarget)
	: m_Mat(mat), m_Cull(cull), m_SamplingTarget(samplingTarget)
{
	if(mat->IsTransmissive()){
		m_Cull = false;
	}
}


bool Object::Hit(const Ray& ray, float tMin, float tMax, HitInfo& hitInfo) const
{
	return true;
}

bool Object::BoundingBox(AABB& outputBox) const
{
	return true;
}

Vec3 Object::Random(Vec3 point) const
{
	return Vec3(1.0f, 0.0f, 0.0f);
}

float Object::PdfValue(const Vec3& origin, const Vec3& dir) const
{
	return 0.0f;
}

bool Object::IsSamplingTarget() const
{
	return m_SamplingTarget;
}

const Material* Object::GetMaterial() const
{
	return m_Mat;
}