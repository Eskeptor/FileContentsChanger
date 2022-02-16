#pragma once

class CppUtil
{
public:
	// Get Exe Full Path
	static CString GetExePath();
	// Hex Data(CString) to Dec Data(int)
	static int HexToDec(CString strValue);

	// Read Ini File
	static CString INIReadString(CString strAppName, CString strKeyName, CString strFilePath, int nBufferSize = 8192);
	static int INIReadInt(CString strAppName, CString strKeyName, CString strFilePath);
	// Write Ini File
	static void INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, CString strValue);
	static void INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, int nValue);
	static void INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, double dValue);
	static void INIWriteString(CString strAppName, CString strKeyName, CString strFilePath, float fValue);

	// File Write
	static bool WriteFile(CString strPath, CString strText, int nEncode);
	static bool WriteFileANSI(CString strPath, CString strText);
	static bool WriteFileUTF8(CString strPath, CString strText, bool bWithBOM);
	static bool WriteFileUnicode(CString strPath, CString strText);
	// File Read (EUC-KR)
	static CString ReadFileANSI(CString strPath);
	// File Read (UTF-8)
	static CString ReadFileUTF8(CString strPath);
	// File Read (After Encoding Check)
	static CString ReadFileAll(CString strPath, int& nOutEncode);
	// Check File Encoding
	static int CheckFileEncoding(CString strPath);
	// Folder Check (If folder is not exist, folder create)
	static int FolderCheck(CString strDirPath);
	// Get File Size
	static int GetFileSize(CString strPath);

	// Get File Time Info
	static void GetFileTimes(CString strPath, SYSTEMTIME* outCreate, SYSTEMTIME* outAccess, SYSTEMTIME* outWrite);
	// Get Current System Time to CString
	static CString GetCurTime();
};