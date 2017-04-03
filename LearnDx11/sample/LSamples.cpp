#include "stdafx.h"
#include "LSamples.h"
#include "ColorBoxSample.h"
#include "MontainSample.h"
#include "ShapesSample.h"

LSamples::LSamples(LD3DApplication* pApp)
	: m_pApp(pApp)
	, m_activeSample(nullptr)
{
}


LSamples::~LSamples()
{
}

void LSamples::init()
{
	m_samples.push_back(new ColorBoxSample(m_pApp));
	m_samples.push_back(new MontainSample(m_pApp));
	m_samples.push_back(new ShapesSample(m_pApp));
}

void LSamples::select(size_t idx)
{
	if (idx >= m_samples.size())
		return;

	m_activeSample = m_samples[idx];
	m_activeSample->create();
}
