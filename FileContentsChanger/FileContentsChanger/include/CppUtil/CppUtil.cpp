#include "pch.h"
#include "CppUtil.h"
#include <codecvt>
#include <fstream>
#include <sstream>

/**
Get Exe Full Path
@param
@return		Exe Full Path
*/
CString CppUtil::GetExePath()
{
	CString strExePath = _T("");

	TCHAR szTemp[_MAX_PATH + 1] = { 0, };
	GetModuleFileName(AfxGetApp()->m_hInstance, szTemp, _MAX_PATH);
	strExePath = szTemp;

	int nLength = strExePath.ReverseFind(_T('\\'));
	if (nLength > 0)
		strExePath = strExePath.Left(nLength + 1);

	return strExePath;
}


/**
Hex Data(CString) to Dec Data(int)
@param		strValue			Hex Data
@return		Dec Data
*/
int CppUtil::HexToDec(CString strValue)
{
	strValue.TrimLeft(_T("0x"));
	strValue.TrimLeft(_T("0X"));

	char szBuff[16] = { 0, };
	WideCharToMultiByte(CP_ACP, 0, strValue, -1, szBuff, 8, 0, 0);

	return (int)strtol(szBuff, NULL, 16);
}


/**
Read Ini File to CString Data
@param		strAppName			AppName
@param		strKeyName			KeyName
@param		strFilePath			Ini File Path
@param		nBufferSize			Buffer Size(Default : 8192)
@return		str					Result
*/
CString CppUtil::INIReadString(CString strAppName, CString strKeyName, CString strFilePath, int nBufferSize)
{
	TCHAR* szBuffer = new TCHAR[nBufferSize];
	memset(szBuffer, 0, sizeof(TCHAR) * nBufferSize);

	GetPrivateProfileString(strAppName, strKeyName, _T(""), szBuffer, nBufferSize, strFilePath);
	CString str;
	str.Format(_T("%s"), szBuffer);

	delete[] szBuffer;
	szBuffer = nullptr;
	return str;
}


/**
Read Ini File to int Data
@param		strAppName			AppName
@param		strKeyName			KeyName
@param		strFilePath			Ini File Path
@return		nResult				Result
*/
int CppUtil::INIReadInt(CString strAppName, CString strKeyName, CString strFilePath)
{
	return (int)GetPrivateProfileInt(strAppName, strKeyName, 0, strFilePath);
}


/**
Write Ini File
@param		strAppName			AppName
@param		strKeyName			KeyName
@param		strFilePath			Ini File Path
@param		strValue			Write Data
@return
*/
void CppUtil::INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, CString strValue)
{
	WritePrivateProfileString(strAppName, strKeyName, NULL, strFilePath);
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}
void CppUtil::INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, int nValue)
{
	CString strValue = _T("");
	strValue.Format(_T("%d"), nValue);

	WritePrivateProfileString(strAppName, strKeyName, NULL, strFilePath);
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}
void CppUtil::INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, double dValue)
{
	CString strValue = _T("");
	strValue.Format(_T("%f"), dValue);

	WritePrivateProfileString(strAppName, strKeyName, NULL, strFilePath);
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}
void CppUtil::INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, float fValue)
{
	CString strValue = _T("");
	strValue.Format(_T("%f"), fValue);

	WritePrivateProfileString(strAppName, strKeyName, NULL, strFilePath);
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}


/**
File Write
@param		strPath			File Path
@param		strText			Write Stuff
@param		nEncode			Encoding
@return		true/false
*/
bool CppUtil::WriteFile(CString strPath, CString strText, int nEncode)
{
	bool bResult = false;

	if (strPath.IsEmpty())
	{
		TRACE(_T("ReadFileANSI : pFile = NULL\n"));
		return bResult;
	}

	switch (nEncode)
	{
		case 0:		// ANSI
			bResult = WriteFileANSI(strPath, strText);
			break;
		case 1:		// UTF-8
			bResult = WriteFileUTF8(strPath, strText, true);
			break;
		case 2:		// Unicode
		case 3:
			bResult = WriteFileUnicode(strPath, strText);
			break;
	}

	return bResult;
}


/**
File Write (ANSI)
@param		strPath			File Path
@param		strText			Write Stuff
@return		true/false
*/
bool CppUtil::WriteFileANSI(CString strPath, CString strText)
{
	CFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
	{
		return false;
	}
	file.Write(CT2A(strText), strText.GetLength());
	file.Close();
	
	return true;
}


/**
File Write (UTF8)
@param		strPath			File Path
@param		strText			Write Stuff
@return		true/false
*/
bool CppUtil::WriteFileUTF8(CString strPath, CString strText, bool bWithBOM)
{
	CFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
	{
		return false;
	}
	file.Write(strText, strText.GetLength() * sizeof(TCHAR));
	file.Close();

	return true;
}


/**
File Write (Unicode)
@param		strPath			File Path
@param		strText			Write Stuff
@return		true/false
*/
bool CppUtil::WriteFileUnicode(CString strPath, CString strText)
{
	WORD wUnicodeMark = 0xFEFF;

	CFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
	{
		return false;
	}
	file.Write(&wUnicodeMark, sizeof(wUnicodeMark));
	file.Write(strText, strText.GetLength() * sizeof(TCHAR));
	file.Close();

	return true;
}


/**
File Read (ANSI)
@param		strPath			File Path
@return		Read Data
*/
CString CppUtil::ReadFileANSI(CString strPath)
{
	if (strPath.IsEmpty())
	{
		TRACE(_T("ReadFileANSI : pFile = NULL\n"));
		return _T("");
	}

	CString strResult = _T("");
	std::ifstream ifStream;
	ifStream.open(strPath);
	std::string strTemp;
	if (ifStream.is_open())
	{
		ifStream.seekg(0, std::ios::end);
		int nSize = (int)ifStream.tellg();
		strTemp.resize(nSize);
		ifStream.seekg(0, std::ios::beg);
		ifStream.read(&strTemp[0], nSize);

		strResult = strTemp.c_str();
	}

	return strResult;
}


/**
File Read (UTF-8)
@param		strPath			File Path
@return		Read Data
*/
CString CppUtil::ReadFileUTF8(CString strPath)
{
	if (strPath.IsEmpty())
	{
		TRACE(_T("ReadFileUTF8 : pFile = NULL\n"));
		return _T("");
	}

	CString strResult = _T("");
	std::wifstream wif(strPath);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();

	strResult = wss.str().c_str();
	return strResult;
}


/**
File Read (After Encoding Check)
@param		strPath			File Path
@param		nOutEncode		Encoding
@return		Read Data
*/
CString CppUtil::ReadFileAll(CString strPath, int& nOutEncode)
{
	CString strResult = _T("");
	int nEncoding = CheckFileEncoding(strPath);
	if (nEncoding == 0)
		strResult = ReadFileANSI(strPath);
	else if (nEncoding == 1)
		strResult = ReadFileUTF8(strPath);
	else
		strResult = ReadFileUTF8(strPath);

	nOutEncode = nEncoding;

	return strResult;
}


/**
Check File Encoding
@param		strPath			File Path
@return		0				ANSI
			1				UTF8 with BOM
			2				Unicode (Big Endian)
			3				Unicode
*/
int CppUtil::CheckFileEncoding(CString strPath)
{
	int nEncode = 0;
	unsigned char uniTxt[] = { 0xFF, 0xFE };
	unsigned char endianTxt[] = { 0xFE, 0xFF };
	unsigned char utf8Txt[] = { 0xEF, 0xBB };

	DWORD dwBytesRead = 0;
	HANDLE hFile = NULL;
	try
	{
		hFile = CreateFile(strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			hFile = NULL;
			return -1;
		}

		BYTE* lpHeader = new BYTE[2];
		BOOL bRead = ReadFile(hFile, lpHeader, 2, &dwBytesRead, NULL);
		CloseHandle(hFile);

		if (bRead != 0)
		{
			if (lpHeader[0] == uniTxt[0] &&
				lpHeader[1] == uniTxt[1])
				nEncode = 3;	// Unicode
			else if (lpHeader[0] == endianTxt[0] &&
					 lpHeader[1] == endianTxt[1])
				nEncode = 2;	// Unicode (Big Endian)
			else if (lpHeader[0] == utf8Txt[0] &&
					 lpHeader[1] == utf8Txt[1])
				nEncode = 1;	// UTF8 with BOM
			else
				nEncode = 0;	// ANSI
		}
		
		delete[] lpHeader;
	}
	catch (CException& e)
	{
		TCHAR szException[1024] = { 0, };
		e.GetErrorMessage(szException, 1024);
		TRACE(szException);

		CloseHandle(hFile);
		hFile = NULL;
	}
	return nEncode;
}


/**
Folder Check (If folder is not exist, folder create)
@param		strDirPath			Folder Path
@return		-1					Create Failed
			0					Aleady Exist Folder
			1					Create Success
*/
int CppUtil::FolderCheck(CString strDirPath)
{
	CFileFind file;
	CString strPath = _T("");
	strPath.Format(_T("%s*.*"), strDirPath.GetString());
	BOOL bResult = file.FindFile(strPath);

	if (!bResult)
	{
		strPath.Format(_T("%s\\"), strDirPath.GetString());
		bResult = CreateDirectory(strPath, NULL);

		if (!bResult)
			return -1;
		return 1;
	}

	return 0;
}


/**
Get File Size
@param		strPath			File Path
@return		nSize			File Size(byte)
*/
int CppUtil::GetFileSize(CString strPath)
{
	int nSize = 0;

	if (strPath.IsEmpty())
		return nSize;

	std::ifstream ifStream(strPath, std::ios::in | std::ios::binary);
	if (ifStream.is_open())
	{
		ifStream.seekg(0, std::ios::end);
		nSize = (int)ifStream.tellg();
		ifStream.close();
	}

	return nSize;
}


/**
Get File Time Info
@param		strPath			File Path
@param		outCreate		Create Time
@param		outAccess		Access Time
@param		outWrite		Write Time
@return
*/
void CppUtil::GetFileTimes(CString strPath, SYSTEMTIME* outCreate, SYSTEMTIME* outAccess, SYSTEMTIME* outWrite)
{
	if (strPath.IsEmpty())
		return;

	HANDLE hFile = NULL;
	try
	{
		hFile = CreateFile(strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME timeCreate;
			FILETIME timeAccess;
			FILETIME timeWrite;

			BOOL bRet = GetFileTime(hFile, &timeCreate, &timeAccess, &timeWrite);
			if (bRet == FALSE)
				return;

			SYSTEMTIME stimeCreate;
			SYSTEMTIME stimeAccess;
			SYSTEMTIME stimeWrite;

			if (outCreate != nullptr)
			{
				bRet = FileTimeToSystemTime(&timeCreate, &stimeCreate);
				if (bRet)
					SystemTimeToTzSpecificLocalTime(NULL, &stimeCreate, outCreate);
			}
			if (outAccess != nullptr)
			{
				bRet = FileTimeToSystemTime(&timeAccess, &stimeAccess);
				if (bRet)
					SystemTimeToTzSpecificLocalTime(NULL, &stimeAccess, outAccess);
			}
			if (outWrite != nullptr)
			{
				bRet = FileTimeToSystemTime(&timeWrite, &stimeWrite);
				if (bRet)
					SystemTimeToTzSpecificLocalTime(NULL, &stimeWrite, outWrite);
			}
		}
		CloseHandle(hFile);
	}
	catch (CException& e)
	{
		TCHAR szException[1024] = { 0, };
		e.GetErrorMessage(szException, 1024);
		TRACE(szException);

		CloseHandle(hFile);
		hFile = NULL;
	}
}


/**
Get Current System Time to CString
@param
@return		strCurTime		Current Time
*/
CString CppUtil::GetCurTime()
{
	CString strCurTime = _T("");

	SYSTEMTIME time;
	GetLocalTime(&time);
	strCurTime.Format(_T("%04u-%02u-%02u %02u:%02u:%02u:%03u"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	return strCurTime;
}