#include "pch.h"
#include "framework.h"
#include "CppUtil.h"
#include <codecvt>
#include <fstream>
#include <sstream>
#include <atlstr.h>

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
Get Exe Name
@param		bIncludeExe		true: Program.exe / false: Program
@return		Exe Name
*/
CString CppUtil::GetExeName(bool bIncludeExe)
{
	CString strExeName = _T("");

	TCHAR szTemp[_MAX_PATH + 1] = { 0, };
	GetModuleFileName(AfxGetApp()->m_hInstance, szTemp, _MAX_PATH);
	strExeName = szTemp;

	int nLength = strExeName.ReverseFind(_T('\\'));
	if (bIncludeExe)
	{
		strExeName = strExeName.Mid(nLength + 1, strExeName.GetLength() - nLength);
	}
	else
	{
		int nDot = strExeName.ReverseFind(_T('.'));
		strExeName = strExeName.Mid(nLength + 1, nDot - nLength - 1);
	}

	return strExeName;
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
	strValue.TrimLeft(_T("#"));

	char szBuff[16] = { 0, };
	WideCharToMultiByte(CP_ACP, 0, strValue, -1, szBuff, 8, 0, 0);

	return (int)strtol(szBuff, NULL, 16);
}


/**
Dec Data(int) to Hex Data(CString)
@param		nDec				Dec Data
@return		Hex Data
*/
CString CppUtil::DecToHex(int nDec)
{
	CString strHex = _T("");
	strHex.Format(_T("%X"), nDec);

	return strHex;
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
void CppUtil::INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, bool bValue)
{
	CString strValue = _T("");
	strValue = bValue ? _T("1") : _T("0");

	WritePrivateProfileString(strAppName, strKeyName, NULL, strFilePath);
	WritePrivateProfileString(strAppName, strKeyName, strValue, strFilePath);
}


/**
File Write
@param		strPath			File Path
@param		strText
@param		nEncode
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
			bResult = WriteFileUTF8(strPath, strText);
			break;
		case 2:		// Unicode
		case 3:
			bResult = WriteFileUnicode(strPath, strText);
			break;
	}

	return bResult;
}
bool CppUtil::WriteFileANSI(CString strPath, CString strText)
{
	CFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
	{
		return false;
	}

	CT2CA strANSI(strText);
	file.Write(strANSI, ::strlen(strANSI));
	file.Close();

	return true;
}
bool CppUtil::WriteFileUTF8(CString strPath, CString strText)
{
	CFile file;
	if (file.Open(strPath, CFile::modeCreate | CFile::modeWrite) == FALSE)
	{
		return false;
	}

	CT2CA strUTF8(strText, CP_UTF8);
	//CStringA strUTF8{ CT2A(strText, CP_UTF8) };
	//file.Write(strText, strUTF8.GetLength());
	file.Write(strUTF8, ::strlen(strUTF8));
	file.Close();

	return true;
}
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
File Exist
@param		strFilePath		File Path
@return		true / false
*/
bool CppUtil::FileCheck(CString strFilePath)
{
	bool bResult = false;

	CFileStatus fileStatus;
	if (CFile::GetStatus(strFilePath, fileStatus))
		bResult = true;

	return bResult;
}


/**
Get File Extension
@param		strFilePath		File Path
@return		Extension
*/
CString CppUtil::GetFileExtension(CString strFilePath)
{
	CString strResult = _T("");

	int nFind = strFilePath.ReverseFind(_T('.'));
	if (nFind != -1)
	{
		strResult = strFilePath.Mid(nFind + 1);
	}

	return strResult;
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


/**
RGB To Hex
@param		nR			Red
@param		nG			Green
@param		nB			Blue
@return		Hex Data
*/
UINT CppUtil::RGBToHex(const int& nR, const int& nG, const int& nB)
{
	if (nR > 255 ||
		nR < 0 ||
		nG > 255 ||
		nG < 0 ||
		nB > 255 ||
		nB < 0)
		return 0;

	return ((nR & 0xFF) << 16) + ((nG & 0xFF) << 8) + (nB & 0xFF);
}


/**
RGB To Hex
@param		nR			Red
@param		nG			Green
@param		nB			Blue
@param		nA			Alpha
@return		Hex Data
*/
UINT CppUtil::RGBAToHex(const int& nR, const int& nG, const int& nB, const int& nA)
{
	if (nR > 255 ||
		nR < 0 ||
		nG > 255 ||
		nG < 0 ||
		nB > 255 ||
		nB < 0)
		return 0;

	return ((nR & 0xFF) << 24) + ((nG & 0xFF) << 16) + ((nB & 0xFF) << 8) + (nA & 0xFF);
}


/**
Hex To RGB
@param		nHex		Hex Data
@param		nOutR		Red (Out Data)
@param		nOutG		Green (Out Data)
@param		nOutB		Blue (Out Data)
*/
void CppUtil::HexToRGB(const int& nHex, int& nOutR, int& nOutG, int& nOutB)
{
	nOutR = ((nHex >> 16) & 0xFF);
	nOutG = ((nHex >> 8) & 0xFF);
	nOutB = (nHex & 0xFF);
}


/**
Hex To RGB
@param		strHex		Hex Data
@param		nOutR		Red (Out Data)
@param		nOutG		Green (Out Data)
@param		nOutB		Blue (Out Data)
*/
void CppUtil::HexToRGB(const CString& strHex, int& nOutR, int& nOutG, int& nOutB)
{
	int nHex = HexToDec(strHex);
	HexToRGB(nHex, nOutR, nOutG, nOutB);
}


/**
RGB To HSV
@param		nR			Red
@param		nG			Green
@param		nB			Blue
@param		nOutH		Hue (Out Data)
@param		nOutS		Saturation (Out Data)
@param		nOutV		Value (Out Data)
*/
void CppUtil::RGBToHSV(const int& nR, const int& nG, const int& nB, int& nOutH, int& nOutS, int& nOutV)
{
	if (nR > 255 ||
		nR < 0 ||
		nG > 255 ||
		nG < 0 ||
		nB > 255 ||
		nB < 0)
		return;

	double dR = nR / 255.0;
	double dG = nG / 255.0;
	double dB = nB / 255.0;
	double dMax = max(max(dR, dG), dB);
	double dMin = min(min(dR, dG), dB);
	double dDiff = dMax - dMin;

	if (dDiff > 0.0)
	{
		if (dMax == dR)
			nOutH = (int)(60.0 * (fmod(((dG - dB) / dDiff), 6)));
		else if (dMax == dG)
			nOutH = (int)(60.0 * (((dB - dR) / dDiff) + 2.0));
		else if (dMax == dB)
			nOutH = (int)(60.0 * (((dR - dG) / dDiff) + 4.0));

		if (dMax > 0.0)
			nOutS = (int)round((dDiff / dMax * 100.0));
		else
			nOutS = 0;

		nOutV = (int)round((dMax * 100.0));
	}
	else
	{
		nOutH = 0;
		nOutS = 0;
		nOutV = (int)round(dMax * 100.0);
	}

	if (nOutH < 0)
		nOutH = 360 + nOutH;
}


/**
Hex To HSV
@param		nHex		Hex Data
@param		nOutH		Hue (Out Data)
@param		nOutS		Saturation (Out Data)
@param		nOutV		Value (Out Data)
*/
void CppUtil::HexToHSV(const int& nHex, int& nOutH, int& nOutS, int& nOutV)
{
	int nR = 0;
	int nG = 0;
	int nB = 0;
	HexToRGB(nHex, nR, nG, nB);
	RGBToHSV(nR, nG, nB, nOutH, nOutS, nOutV);
}


/**
HSV To RGB
@param		nH			Hue
@param		nS			Saturation
@param		nV			Value
@param		nOutR		Red (Out Data)
@param		nOutG		Green (Out Data)
@param		nOutB		Blue (Out Data)
*/
void CppUtil::HSVToRGB(const int& nH, const int& nS, const int& nV, int& nOutR, int& nOutG, int& nOutB)
{
	if (nH > 360 ||
		nH < 0 ||
		nS > 100 ||
		nS < 0 ||
		nV > 100 ||
		nV < 0)
		return;

	double dRange = ((double)(nH % 60) / 60.0) * 255.0;
	double dH = 0.0;
	double dP = 0.0;
	double dQ = 0.0;
	double dT = 0.0;

	dH = (double)nH;
	if (dH >= 360.0)
		dH = 0.0;
	dH /= 60.0;

	int nSel = (int)dH;

	double dVDiv100 = (double)nV / 100.0;
	double dSDiv100 = (double)nS / 100.0;
	dP = dVDiv100 * (1.0 - dSDiv100);
	dQ = dVDiv100 * (1.0 - (dSDiv100 * (dH - (double)nSel)));
	dT = dVDiv100 * (1.0 - (dSDiv100 * (1.0 - (dH - (double)nSel))));

	switch (nSel)
	{
	case 0:
		nOutR = (int)round(dVDiv100 * 255.0);
		nOutG = (int)round(dT * 255.0);
		nOutB = (int)round(dP * 255.0);
		break;
	case 1:
		nOutR = (int)round(dQ * 255.0);
		nOutG = (int)round(dVDiv100 * 255.0);
		nOutB = (int)round(dP * 255.0);
		break;
	case 2:
		nOutR = (int)round(dP * 255.0);
		nOutG = (int)round(dVDiv100 * 255.0);
		nOutB = (int)round(dT * 255.0);
		break;
	case 3:
		nOutR = (int)round(dP * 255.0);
		nOutG = (int)round(dQ * 255.0);
		nOutB = (int)round(dVDiv100 * 255.0);
		break;
	case 4:
		nOutR = (int)round(dT * 255.0);
		nOutG = (int)round(dP * 255.0);
		nOutB = (int)round(dVDiv100 * 255.0);
		break;
	case 5:
	default:
		nOutR = (int)round(dVDiv100 * 255.0);
		nOutG = (int)round(dP * 255.0);
		nOutB = (int)round(dQ * 255.0);
		break;
	}
}


/**
HSV To Hex
@param		nH			Hue
@param		nS			Saturation
@param		nV			Value
@return		Hex Data
*/
UINT CppUtil::HSVToHex(const int& nH, const int& nS, const int& nV)
{
	if (nH > 360 ||
		nH < 0 ||
		nS > 100 ||
		nS < 0 ||
		nV > 100 ||
		nV < 0)
		return 0;

	int nR = 0;
	int nG = 0;
	int nB = 0;
	HSVToRGB(nH, nS, nV, nR, nG, nB);

	return RGBToHex(nR, nG, nB);
}