#include "stdafx.h"
#include "DirectXUtils.h"


#include <iostream>
using std::string;
using std::wstring;

/** 
 * @author zhenHua.sun
 * @date  2010-09-27
 * @brief �������ַ����д�����ַ���ƥ�亯������Ҫ��������ͷ�豸
 * @param incomeDeviceName ���ն˴�����������ͷ����
 * @param currentDeviceName ��ǰ�Ƚϵ�����ͷ����
 * @return ���ƥ���򷵻�0
 */

int wstrCmpSpecial( wstring currentDeviceName , wstring incomeDeviceName  )
{
	//�Դ���?���ַ����������⴦������?���ֵ�λ���������������Ҫ��ͬ���滻
	wstring specialSign = L"?";
	wstring newSign = L"_";

	size_t pos = 0;         
	while(true)   
	{   
		pos = incomeDeviceName.find( L"?" ,  pos);   
		if (pos == wstring::npos) 
		{
			//���û�ҵ�?�Ļ��򲻽������⴦��
			break;
		}
		incomeDeviceName.replace(pos, specialSign.length() , newSign); 
		currentDeviceName.replace( pos , specialSign.length() , newSign);
		pos += newSign.length();  
	}   
	return wcscmp( incomeDeviceName.c_str() , currentDeviceName.c_str() );
}


HRESULT FindMyCaptureDevice(IBaseFilter **pF, BSTR bstrName) {
	HRESULT hr = E_FAIL;
	CComPtr < IBaseFilter > pFilter;
	CComPtr < ICreateDevEnum > pSysDevEnum;
	CComPtr < IEnumMoniker > pEnumCat = NULL;

	// Create the System Device Enumerator.
	pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pSysDevEnum) {
		return E_FAIL;
	}

	// Obtain a class enumerator for the video compressor category.
	pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, & pEnumCat, 0);
	if (!pEnumCat) {
		return E_FAIL;
	}

	pEnumCat->Reset();

	// Enumerate the monikers.
	//CComPtr<IMoniker> pMoniker;

	while (true) {
		//while(pMoniker && pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		CComPtr < IMoniker > pMoniker;
		ULONG cFetched;
		CComPtr < IPropertyBag > pProp;

		HRESULT hr_work = pEnumCat->Next(1, & pMoniker, & cFetched);
		if (hr_work != S_OK) {
			break;
		}

		hr = pMoniker->BindToStorage(0, 0,
		IID_IPropertyBag, (void * *) & pProp);

		if (hr != S_OK) {
			continue;
		}

		VARIANT varName;
		VariantInit(& varName); // Try to match the friendly name.
		hr = pProp->Read(L"FriendlyName", & varName, 0);
		if (SUCCEEDED(hr) && ( wstrCmpSpecial( varName.bstrVal ,bstrName )==0)) {
			hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void * *) & pFilter);
			break;
		}
		VariantClear(& varName);
		pMoniker = NULL; // Release for the next loop.
	}
	if (pFilter) {
		*pF = pFilter;
		(*pF)->AddRef(); // Add ref on the way out.
	}
	return hr;
}

HRESULT FindDevice( CLSID classid , IBaseFilter * * pF, BSTR bstrName) {
	HRESULT hr = E_FAIL;
	CComPtr < IBaseFilter > pFilter;
	CComPtr < ICreateDevEnum > pSysDevEnum;
	CComPtr < IEnumMoniker > pEnumCat = NULL;

	// Create the System Device Enumerator.
	pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pSysDevEnum) {
		return E_FAIL;
	}

	// Obtain a class enumerator for the video compressor category.
	pSysDevEnum->CreateClassEnumerator( classid, & pEnumCat, 0);
	if (!pEnumCat) {
		return E_FAIL;
	}

	pEnumCat->Reset();

	// Enumerate the monikers.
	//CComPtr<IMoniker> pMoniker;

	while (true) {
		//while(pMoniker && pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		CComPtr < IMoniker > pMoniker;
		ULONG cFetched;
		CComPtr < IPropertyBag > pProp;

		HRESULT hr_work = pEnumCat->Next(1, & pMoniker, & cFetched);
		if (hr_work != S_OK) {
			break;
		}

		hr = pMoniker->BindToStorage(0, 0,
			IID_IPropertyBag, (void * *) & pProp);

		if (hr != S_OK) {
			continue;
		}

		VARIANT varName;
		VariantInit(& varName); // Try to match the friendly name.
		hr = pProp->Read(L"FriendlyName", & varName, 0);
		if (SUCCEEDED(hr) && ( wstrCmpSpecial( varName.bstrVal ,bstrName )==0)) {
			hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void * *) & pFilter);
			break;
		}
		VariantClear(& varName);
		pMoniker = NULL; // Release for the next loop.
	}
	if (pFilter) {
		*pF = pFilter;
		(*pF)->AddRef(); // Add ref on the way out.
	}
	return hr;
}

IAMStreamConfig * GetIAMStreamConfig(IBaseFilter * pFilter) {
	IEnumPins * pEnum = NULL;

	HRESULT hr = pFilter->EnumPins(& pEnum);
	if (FAILED(hr)) {
		return NULL;
	}

	IPin * pPin = NULL;
	while (pEnum->Next(1, & pPin, NULL) == S_OK) {
        //��Щ��Ƶ�豸��Ҫ������һ�β��ܻ����ȷ��ʽ
        TrailConnect(pPin);
		IAMStreamConfig * pIAMS = NULL;
		hr = pPin->QueryInterface(IID_IAMStreamConfig, (void * *) & pIAMS);
		if (hr == S_OK) {
			return pIAMS;
		}
		pPin->Release();
	}
	pEnum->Release();
	return NULL;
}

//���ߺ���
std::wstring str2wstr(const std::string& str)
{
	setlocale(LC_ALL, "chs"); 
	const char* _Source = str.c_str();
	size_t _Dsize = str.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring wstr = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, "C");
	return wstr;
}

std::string wstr2str( const std::wstring& wstr )
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = wstr.c_str();
	size_t _Dsize = 2 * wstr.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string str = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());

    return str;
}

/**
 * @brief ö��ָ��ClassID�µ��豸�Ѻ����ƺ��豸����
 * @param clsidDeviceClass Ҫö�ٵ��豸ClassID
 * @param pWStringArray �洢�豸���Ƶ�����
 * @NumOfDevices �������������ÿ���豸�������������ƣ���˴�С���豸��������2
 */
HRESULT  ListCaptureDevice(const CLSID & clsidDeviceClass, std::vector<std::wstring> & wstringArray,int & nNumOfDevices)
{
	HRESULT hr = E_FAIL;
	CComPtr < IBaseFilter > pFilter;
	CComPtr < ICreateDevEnum > pSysDevEnum;
	CComPtr < IEnumMoniker > pEnumCat = NULL;

	// Create the System Device Enumerator.
	pSysDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if (!pSysDevEnum) {
		return E_FAIL;
	}

	// Obtain a class enumerator for the video compressor category.
	pSysDevEnum->CreateClassEnumerator(clsidDeviceClass, & pEnumCat, 0);
	if (!pEnumCat) {
		return E_FAIL;
	}

	pEnumCat->Reset();

	nNumOfDevices = 0;

	while (true) {
		//while(pMoniker && pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		CComPtr < IMoniker > pMoniker;
		ULONG cFetched;
		CComPtr < IPropertyBag > pProp;

		HRESULT hr_work = pEnumCat->Next(1, & pMoniker, & cFetched);
		if (hr_work != S_OK) {
			break;
		}

		hr = pMoniker->BindToStorage(0, 0,
			IID_IPropertyBag, (void * *) & pProp);

		if (hr != S_OK) {
			continue;
		}

		VARIANT varName;
		VariantInit(& varName); // Try to match the friendly name.
		hr = pProp->Read(L"Description", &varName, 0);
		if (FAILED(hr))
		{
			hr = pProp->Read(L"FriendlyName", &varName, 0); //�豸�Ѻ�����
		}
		if (SUCCEEDED(hr))
		{
			// Add it to the application's list box.
			//	char displayName[1024];
			//	WideCharToMultiByte(CP_ACP,0,varName.bstrVal,-1,displayName,1024,"",NULL);

			wstringArray.push_back(varName.bstrVal);//varName.bstrVal;
			++nNumOfDevices;
			VariantClear(&varName); 

			WCHAR * wszDiaplayName=NULL;
			pMoniker->GetDisplayName(NULL,NULL,&wszDiaplayName);
			wstringArray.push_back(wszDiaplayName);
			++nNumOfDevices;
			CoTaskMemFree(wszDiaplayName);
		}
		VariantClear(& varName);
		pMoniker = NULL; // Release for the next loop.
	}
	return hr;
}
//-----------------------------------------------------------------------------------

HRESULT EnumerateDevices( REFGUID category, std::vector<DeviceInfo> &devices) {
    devices.clear();
    CoInitialize(NULL); //��Ϊ���ܻ��ڲ�ͬ�̵߳��ô˷�����ÿ���̶߳�Ҫ��ʼ�����������������һ�����ظ�����������
    ICreateDevEnum *pDevEnum;
    IEnumMoniker *pEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, 
                                NULL,
                                CLSCTX_INPROC_SERVER,
                                IID_PPV_ARGS(&pDevEnum));

    if (SUCCEEDED(hr)) 
    {
        hr = pDevEnum->CreateClassEnumerator(category, &pEnum,0);
        pDevEnum->Release();
        if(hr == S_FALSE)
        {
            hr = VFW_E_NOT_FOUND;
            return hr;
        }
    }

    IMoniker *pMoniker = NULL;
    while (pEnum->Next(1, &pMoniker, NULL) == S_OK) 
    {
        IPropertyBag *pPropBag;
        HRESULT hr = pMoniker->BindToStorage(0,0,IID_PPV_ARGS(&pPropBag));
        if(FAILED(hr)) 
        {
	        pMoniker->Release();
	        continue;
        }

        VARIANT var;
        VariantInit(&var);

        hr = pPropBag->Read(L"FriendlyName", &var,0);
        DeviceInfo info;
        if (SUCCEEDED(hr)) 
        {
	        info.friendly_name = var.bstrVal;
        }
        hr = pPropBag->Read(L"DevicePath", &var,0);
        if(SUCCEEDED(hr)) 
        {
	        info.device_path = var.bstrVal;
        }
        devices.push_back(info);

        VariantClear(&var);
        pPropBag->Release();
        pMoniker->Release();
    }
    pEnum->Release();
    return hr;
}

HRESULT TrailConnect( IPin * pPin )
{
    HRESULT hr;
    if (!pPin) return E_INVALIDARG;
    //���PIN����
    {
        PIN_DIRECTION ThisPinDir;
        pPin->QueryDirection(&ThisPinDir);
        if (ThisPinDir != PINDIR_OUTPUT) return E_INVALIDARG;
    }

    //���NULL Renderer
    const CLSID nullRenderId = CLSID_NullRenderer; // {0xC1F400A4, 0x3F08, 0x11D3, {0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37}};
    CComPtr<IBaseFilter> pNullRenderer = 0;
    hr = CoCreateInstance(nullRenderId,0,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)(&pNullRenderer));
    if (FAILED(hr) || !pNullRenderer) return E_FAIL;

    //���Null Renderer������PIN
    CComPtr<IPin> pPinRenderer;
    CComPtr<IEnumPins> pEnum;
    hr = pNullRenderer->EnumPins(&pEnum);
    if (FAILED(hr)) return E_FAIL;
    while (pEnum->Next(1, &pPinRenderer, NULL) == S_OK)
    {
        PIN_DIRECTION ThisPinDir;
        pPinRenderer->QueryDirection(&ThisPinDir);
        if (ThisPinDir == PINDIR_INPUT) break;
    }
    if (!pPinRenderer) return E_FAIL;

    //������
    hr = pPin->Connect(pPinRenderer, NULL);
    hr = pPin->Disconnect();
    return S_OK;
}
