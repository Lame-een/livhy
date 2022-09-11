#include "pch.h"
#include "pdf.h"
#include "objects/object.h"

ObjectsPDF::ObjectsPDF(const Vec3& origin)
	: m_Origin(origin)
{
}

float ObjectsPDF::Value(const Vec3& dir) const
{
	size_t size = m_Objects.size();
	assert(size != 0 && "No pdfs allocated.");

	if(size == 1)
	{
		return m_Objects[0]->PdfValue(m_Origin, dir);
	}

	float acc = 0.0f;
	float inv = 1.0f / size;
	for(size_t i = 0; i < size; i++)
	{
		acc += inv * m_Objects[i]->PdfValue(m_Origin, dir);
	}
	return acc;
}

Vec3 ObjectsPDF::Generate() const
{
	size_t size = m_Objects.size();
	assert(size != 0 && "No objects allocated.");

	if(size == 1)
	{
		return m_Objects[0]->Random(m_Origin);
	}
	int randIndex = lameutil::g_RandGen.getInt(0, (int)size);
	return m_Objects[randIndex]->Random(m_Origin);
}

void ObjectsPDF::AddObject(const Object* obj)
{
	m_Objects.push_back(obj);
}

void ObjectsPDF::AddObject(const std::vector<Object*>& objs)
{
	for(int i = 0; i < objs.size(); i++)
	{
		m_Objects.push_back(objs[i]);
	}
}



MixturePDF::MixturePDF(const PDF* material, const PDF* target, bool cleanup)
	: m_MaterialPdf(material), m_TargetsPdf(target), m_Cleanup(cleanup)
{
}

MixturePDF::~MixturePDF()
{
	if(m_Cleanup)
	{
		delete m_MaterialPdf;
		delete m_TargetsPdf;
	}
}

float MixturePDF::Value(const Vec3& dir) const
{
	return 0.5f * m_MaterialPdf->Value(dir) + 0.5f * m_TargetsPdf->Value(dir);
}

Vec3 MixturePDF::Generate() const
{
	int randIndex = lameutil::g_RandGen.getInt(0, 2);
	if(randIndex == 0)
	{
		return m_MaterialPdf->Generate();
	}
	else
	{
		return m_TargetsPdf->Generate();
	}
}



CosinePDF::CosinePDF(const Vec3& normal)
	: m_Base(normal)
{
}

float CosinePDF::Value(const Vec3& dir) const
{
	float cosine = glm::dot(dir, m_Base.w);
	return (cosine <= 0) ? 0 : cosine * c_1_Pi;
}

Vec3 CosinePDF::Generate() const
{
	return m_Base.local(RandomCosineDir());
}


const ONB& CosinePDF::Base()
{
	return m_Base;
}



SpecularPDF::SpecularPDF(const Vec3& normal, float shininess)
	: m_Base(normal), m_Shininess(shininess)
{
}

float SpecularPDF::Value(const Vec3& dir) const
{
	Vec3 exact = glm::reflect(dir, m_Base.w);
	float cosNAlpha = pow(glm::dot(dir, exact), m_Shininess);
	return (cosNAlpha <= 0) ? 0 : (m_Shininess + 1) * cosNAlpha * c_2_Pi;
}

Vec3 SpecularPDF::Generate() const
{
	return m_Base.local(RandomSpecularDir(m_Shininess));
}

const ONB& SpecularPDF::Base()
{
	return m_Base;
}
