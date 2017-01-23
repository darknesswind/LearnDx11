#include "stdafx.h"
#include "LFile.h"

LFile::LFile()
{
}


LFile::~LFile()
{
	if (m_hFile)
		fclose(m_hFile);
}

bool LFile::openRead(LPCWSTR lpName)
{
	if (m_hFile)
		return false;

	errno_t err = _wfopen_s(&m_hFile, lpName, L"rb");
	if (0 != err)
		return false;

	fseek(m_hFile, 0, SEEK_END);
	fgetpos(m_hFile, &m_size);
	fseek(m_hFile, 0, SEEK_SET);
	return true;
}

bool LFile::openWrite(LPCWSTR lpName)
{
	if (m_hFile)
		return false;

	errno_t err = _wfopen_s(&m_hFile, lpName, L"wb");
	if (0 != err)
		return false;

	return true;
}

LFile::ByteArray LFile::readToEnd()
{
	fpos_t cur = 0;
	fgetpos(m_hFile, &cur);
	fpos_t sizeToRead = m_size - cur;
	ByteArray buf(sizeToRead);
	fread_s(buf.data(), sizeToRead, sizeToRead, 1, m_hFile);
	return buf;
}
