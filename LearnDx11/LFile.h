#pragma once
#ifndef assert
#define assert(exp)
#endif

class LFile
{
public:
	LFile();
	~LFile();

	typedef std::vector<unsigned char> ByteArray;

	bool openRead(LPCWSTR lpName);
	template <typename T> size_t readAs(T& var);
	template <typename T> size_t readAs(T* pBuf, size_t cnt);
	template <typename T> T read();
	ByteArray readToEnd();

	bool openWrite(LPCWSTR lpName);
	template <typename T> size_t write(T& var);
	template <typename T> size_t write(T* pBuf, size_t cnt);

	fpos_t size() const { return m_size; }
	bool eof() const { return (0 != feof(m_hFile)); }

private:
	FILE* m_hFile = nullptr;
	fpos_t m_size = 0;
};

template <typename T>
size_t LFile::readAs(T& var)
{
	size_t sz = fread_s((void*)&var, sizeof(T), sizeof(T), 1, m_hFile);
	assert(1 == sz);
	return sz;
}

template <typename T>
size_t LFile::readAs(T* pBuf, size_t cnt)
{
	size_t sz = fread_s((void*)pBuf, sizeof(T) * cnt, sizeof(T), cnt, m_hFile);
	assert(cnt == sz);
	return sz;
}

template <typename T>
T LFile::read()
{
	T res = 0;
	readAs(res);
	return res;
}

template <typename T>
size_t LFile::write(T& var)
{
	size_t sz = fwrite((void*)&var, sizeof(T), 1, m_hFile);
	assert(sizeof(T) == sz);
	return sz;
}

template <typename T>
size_t LFile::write(T* pBuf, size_t cnt)
{
	size_t sz = fwrite(pBuf, sizeof(T), cnt, m_hFile);
	assert(sizeof(T) * cnt == sz);
	return sz;
}
