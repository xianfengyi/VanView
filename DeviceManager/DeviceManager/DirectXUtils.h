#ifndef DIRECTX_UTILS_H
#define DIRECTX_UTILS_H

#include <tchar.h>
#include <strsafe.h>
#include <dshow.h>
#include <atlbase.h>
#define __IDxtCompositor_INTERFACE_DEFINED__ 
#define __IDxtAlphaSetter_INTERFACE_DEFINED__ 
#define __IDxtJpeg_INTERFACE_DEFINED__ 
#define __IDxtKey_INTERFACE_DEFINED__ 
#include "qedit.h"
#include <comutil.h> //for _bstr_t

#include <string>
#include <vector>

struct DeviceInfo {
  std::wstring friendly_name;
  std::wstring device_path;
};

HRESULT FindMyCaptureDevice(IBaseFilter ** pF, BSTR bstrName);

HRESULT FindDevice(CLSID classid , IBaseFilter * * pF, BSTR bstrName);
/**
 * Takes a base filter and try to return a pointer to its IAMStreamConfig.
 *
 * @return a NULL pointer in the case it fails
 */
IAMStreamConfig * GetIAMStreamConfig(IBaseFilter * pFilter);

std::wstring str2wstr(const std::string& str);

std::string wstr2str( const std::wstring& wstr );
/**
 * @brief ö��ָ��ClassID�µ��豸�Ѻ����ƺ��豸����
 * @param clsidDeviceClass Ҫö�ٵ��豸ClassID
 * @param pWStringArray �洢�豸���Ƶ�����
 * @NumOfDevices �������������ÿ���豸�������������ƣ���˴�С���豸��������2
 */
HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass, std::vector<std::wstring> & wstringArray,int & nNumOfDevices);

HRESULT EnumerateDevices(REFGUID category, std::vector<DeviceInfo> &devices);

/**
 * @brief ��ָ����IPin���ӵ�Null Renderer
 */
HRESULT TrailConnect(IPin * pPin);

///**
// * @brief ö��ָ��ClassID�µ��豸�Ѻ����ƺ��豸����
// * @param clsidDeviceClass Ҫö�ٵ��豸ClassID
// * @param pWStringArray �洢�豸���Ƶ�����
// * @NumOfDevices �������������ÿ���豸�������������ƣ���˴�С���豸��������2
// */
//HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass,std::wstring * pWStringArray,int & NumOfDevices);

#endif	//DIRECTX_UTILS_H