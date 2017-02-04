#pragma once

class LD3DApplication;
class LSampleBase;
class LSamples
{
public:
	LSamples(LD3DApplication* pApp);
	~LSamples();

	void init();
	void select(size_t idx);
	LSampleBase* activeSample() { return m_activeSample; }

private:
	LSampleBase* m_activeSample;
	std::vector<LSampleBase*> m_samples;
	LD3DApplication* m_pApp;
};

