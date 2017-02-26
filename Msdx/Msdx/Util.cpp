#include "StdAfx.h"
#include "DxDefinitions.h"
#include "Util.h"
#include "Log.h"

#include <errors.h>

using namespace msdx;

Util::Util(void)
{
}

Util::~Util(void)
{
}

std::wstring Util::str2wstr(const std::string & str)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	int size = sizeof(wchar_t) * (str.size() + 1);
	wchar_t *buff = (wchar_t*) malloc(size);
	memset(buff, NULL, size);
	mbstowcs(buff, str.c_str(), str.size() + 1);
	std::wstring wstr(buff);
	free(buff);

	setlocale(LC_ALL, curLocale.c_str());
	return wstr;
}

std::string Util::wstr2str(const std::wstring & wstr)
{
	std::string curLocale = setlocale(LC_ALL, NULL); 
	setlocale(LC_ALL, "chs"); 

	int size = 2 * sizeof(char) * (wstr.size() + 1);
	char *buff = (char*) malloc(size);
	memset(buff, NULL, size);
	wcstombs(buff, wstr.c_str(), size - 1);
	std::string str(buff);
	free(buff);

	setlocale(LC_ALL, curLocale.c_str());
	return str;
}

BOOL Util::wstrCmpWithUnknownChar( const std::wstring &str1, const std::wstring &str2 )
{
	if (str1.size() != str2.size())
	{
		return FALSE;
	}

	for (unsigned int i = 0; i < str1.size(); ++i)
	{
		if (str1[i] == '?' || str2[i] == '?') continue;
		if (str1[i] != str2[i]) return FALSE;
	}

	return TRUE;
}


//²Î¿¼×Ôhttp://msdn.microsoft.com/en-us/library/windows/desktop/dd390991(v=vs.85).aspx
const char * Util::GetCrossbarPhysicalPinName( long lType )
{
    switch (lType) 
    {
    case PhysConn_Auto:                   return "Auto";
    case PhysConn_Video_Tuner:            return "Video Tuner";
    case PhysConn_Video_Composite:        return "Video Composite";
    case PhysConn_Video_SVideo:           return "S-Video";
    case PhysConn_Video_RGB:              return "Video RGB";
    case PhysConn_Video_YRYBY:            return "Video YRYBY";
    case PhysConn_Video_SerialDigital:    return "Video Serial Digital";
    case PhysConn_Video_ParallelDigital:  return "Video Parallel Digital"; 
    case PhysConn_Video_SCSI:             return "Video SCSI";
    case PhysConn_Video_AUX:              return "Video AUX";
    case PhysConn_Video_1394:             return "Video 1394";
    case PhysConn_Video_USB:              return "Video USB";
    case PhysConn_Video_VideoDecoder:     return "Video Decoder";
    case PhysConn_Video_VideoEncoder:     return "Video Encoder";

    case PhysConn_Audio_Tuner:            return "Audio Tuner";
    case PhysConn_Audio_Line:             return "Audio Line";
    case PhysConn_Audio_Mic:              return "Audio Microphone";
    case PhysConn_Audio_AESDigital:       return "Audio AES/EBU Digital";
    case PhysConn_Audio_SPDIFDigital:     return "Audio S/PDIF";
    case PhysConn_Audio_SCSI:             return "Audio SCSI";
    case PhysConn_Audio_AUX:              return "Audio AUX";
    case PhysConn_Audio_1394:             return "Audio 1394";
    case PhysConn_Audio_USB:              return "Audio USB";
    case PhysConn_Audio_AudioDecoder:     return "Audio Decoder";

    default:                              return "Unknown Type";
    }    
}

