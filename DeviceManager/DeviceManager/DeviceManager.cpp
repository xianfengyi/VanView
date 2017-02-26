// DeviceManager.cpp : 定义 DLL 应用程序的导出函数。
//
#include "stdafx.h"

#include <stdio.h>
#include <algorithm>

#include "DeviceManager.h"
#include "DirectXUtils.h"

using std::vector;
using std::string;
using std::wstring;

//vista以上版本支持
#include <mmdeviceapi.h>
#include <propsys.h>
#include <Endpointvolume.h>
#include <Audioclient.h>
#include <Functiondiscoverykeys_devpkey.h>


typedef BOOL (_stdcall *DHCPNOTIFYPROC)(LPWSTR , LPWSTR, BOOL , DWORD,DWORD,DWORD,int );

DeviceManager::DeviceManager() {
  CoInitialize(NULL);
}

DeviceManager::~DeviceManager() {
  CoUninitialize();
}


void DeviceManager::GetVideoCaptureDevices( vector<string>* deviceList )
{
	if( deviceList==NULL )
		return;

	deviceList->clear();

	vector<wstring> Devices;
	int deviceNum =0;

	HRESULT result = ListCaptureDevice(CLSID_VideoInputDeviceCategory, Devices ,deviceNum );
	if( SUCCEEDED(result) )
	{
		wstring s1(L"PCIe SD");
		wstring s2(L"Timeleak sd");
		for( int i=0 ; i<deviceNum ; i+=2 )
		{
			wstring str1head = Devices[i].substr(0,7);
			wstring str2head = Devices[i].substr(0,11);
			if ( str1head == s1 || str2head == s2);
			else
				deviceList->push_back( wstr2str(Devices[i]) );
		}
	}
    else
	{
		printf("获取摄像头设备发生错误！");
	}

}

// 重载此函数用以多视频采集输入情况
void DeviceManager::GetVideoCaptureDevices(VCapDeviceList* deviceList) 
{
    deviceList->clear();

    vector<DeviceInfo> devices;
    int counter = 0;

    HRESULT hr = EnumerateDevices(CLSID_VideoInputDeviceCategory, devices);
    for (int i = 0; i < devices.size(); ++i) 
    {
        VCapDevice captureDevice;
        if (devices[i].friendly_name.substr(0,7) != L"PCIe SD" ||
            devices[i].friendly_name.substr(0,11) != L"Timeleak sd") 
        {
            captureDevice.friendlyName = wstr2str(devices[i].friendly_name);
            captureDevice.devicePath = wstr2str(devices[i].device_path);
            captureDevice.deviceNo = counter;
            deviceList->push_back(captureDevice);
            ++counter;
        }
    }
}

void DeviceManager::GetAudioCaptureDevices( vector<string>* deviceList )
{
	if( deviceList==NULL )
		return;

	deviceList->clear();

	vector<wstring> Devices;
	int deviceNum =0;

	HRESULT result = ListCaptureDevice(CLSID_AudioInputDeviceCategory, Devices ,deviceNum );
	if( SUCCEEDED(result) )
	{
		for( int i=0 ; i<deviceNum ; i+=2 )
		{
			deviceList->push_back( wstr2str(Devices[i]) );
		}
	}
    else
	{
		printf("获取摄像头设备发生错误！");
	}
}

void DeviceManager::GetAudioOutputDevices( vector<string>* deviceList )
{
	if( deviceList==NULL )
		return;

	deviceList->clear();

	vector<wstring> Devices;
	int deviceNum =0;

	HRESULT result = ListCaptureDevice(CLSID_AudioRendererCategory, Devices ,deviceNum );
	if( SUCCEEDED(result) )
	{
		for( int i=0 ; i<deviceNum ; i+=2 )
		{
            deviceList->push_back( wstr2str(Devices[i]) );
		}
	}
    else
	{
		printf("获取摄像头设备发生错误！");
	}
}

void DeviceManager::GetCrossbarDeviceList( const string captureDevice, vector<string> *deviceList )
{
	if( deviceList==NULL )
		return;

	deviceList->clear();

	int numOfDevice = 0;
	vector<wstring> deviceCaptureList;
	HRESULT result = ListCaptureDevice( AM_KSCATEGORY_CROSSBAR , deviceCaptureList , numOfDevice );
	if( FAILED( result ) )
		return ;

    //保留friendlyName和displayName，底层要displayName
	for( int deviceCount=0 ; deviceCount<numOfDevice ; ++deviceCount )
	{
		string deviceName = wstr2str( deviceCaptureList[deviceCount] );
		deviceList->push_back( deviceName );
	}
}

void DeviceManager::GetCrossbarInputType( const std::string &crossbarDisplayName , vector<string> *typeList )
{
    //转换为宽字符串
    std::wstring w_crossbarName = str2wstr( crossbarDisplayName );
    int numOfDevice = 0;
    vector<wstring> deviceCaptureList;
    HRESULT result = ListCaptureDevice( AM_KSCATEGORY_CROSSBAR , deviceCaptureList , numOfDevice );
    if( FAILED( result ) )
	    return ;

    IMoniker *pMoniker=NULL;
    IBaseFilter *m_pCrossbarFilter=0;
    IBindCtx *pBindCtx;
    HRESULT hr=CreateBindCtx(0,&pBindCtx);
    ULONG chEaten=0;
    //hr= MkParseDisplayName(pBindCtx,deviceCaptureList[deviceCount+1].c_str(),&chEaten,&pMoniker);
    hr= MkParseDisplayName(pBindCtx,w_crossbarName.c_str(),&chEaten,&pMoniker);
    pBindCtx->Release();
    if(FAILED(hr))
    {
	    return ;
    }
    pMoniker->AddRef();
    hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&m_pCrossbarFilter);
    if (m_pCrossbarFilter == NULL) 
    {
	    pMoniker->Release();
	    return ;
    }

    //获取crossbar filter的输入针脚
    IAMCrossbar *m_pCrossBar;
    hr=m_pCrossbarFilter->QueryInterface(IID_IAMCrossbar,(void   **)&m_pCrossBar);
    if( FAILED(hr) )
    {
	    pMoniker->Release();
	    m_pCrossbarFilter->Release();
	    return;
    }

    long outCount = -1; //输出针脚计数
    long inCount = -1; //输入针脚计数
    m_pCrossBar->get_PinCounts(&outCount,&inCount);

    for(int i=0;i<inCount;i++)
    {
	    long index = 0;
	    //pinType保存的是输入pin的物理类型,如Svideo等
	    long pinType;		

	    m_pCrossBar->get_CrossbarPinInfo(TRUE,i,&index,&pinType );
	    switch( pinType )
	    {
	    case PhysConn_Video_Composite:
		    typeList->push_back("Composite");
		    break;
	    case PhysConn_Video_SVideo:
		    typeList->push_back("SVideo");
		    break;
	    case PhysConn_Video_Tuner:
		    typeList->push_back("Tuner");
		    break;
	    }
    }

    //释放资源
    pMoniker->Release();
    m_pCrossbarFilter->Release();
    m_pCrossBar->Release();
}

void DeviceManager::GetVideoDeviceMediaType( const std::string &deviceName , vector<string>* typeList )
{
	char media_type_info[56];
	HRESULT hr;
	IAMStreamConfig* pStreamConfig = NULL;
	IBaseFilter*  pCaptureDevice = NULL;
	CComBSTR bstrName(deviceName.c_str());
	hr = FindMyCaptureDevice(&pCaptureDevice, bstrName);
	if ((hr != S_OK) || !pCaptureDevice) {
		return ;
	}

	//initialize IAMStreamConfig
	pStreamConfig = GetIAMStreamConfig(pCaptureDevice);
	if (!pStreamConfig) {
		return ;
	}

	int iCount, iSize;
	hr = pStreamConfig->GetNumberOfCapabilities(&iCount, &iSize);

	VIDEO_STREAM_CONFIG_CAPS scc;
	if (sizeof(scc) != iSize) {
		printf("wrong config structure");
		pStreamConfig->Release();
		return ;
	}
	//vector<string> resolutionList;

	for (int i = 0; i < iCount; i++) 
    {
		AM_MEDIA_TYPE * pmt = NULL;
		hr = pStreamConfig->GetStreamCaps(i, &pmt, reinterpret_cast<BYTE *>(&scc));
		if (hr == S_OK)
        {

			VIDEOINFOHEADER * pvi = (VIDEOINFOHEADER *) pmt->pbFormat;

			if( pmt->subtype == MEDIASUBTYPE_YUY2 
				|| pmt->subtype==MEDIASUBTYPE_RGB24 
				)
			{
				if( pvi->bmiHeader.biWidth<300 || pvi->bmiHeader.biHeight<200 )
					continue;

				int fps = 10000000 / pvi->AvgTimePerFrame;
				if( fps<10 )		//低于15帧的不要
					continue;

				sprintf_s( media_type_info , "%dX%dX%d", pvi->bmiHeader.biWidth , pvi->bmiHeader.biHeight, fps );
				std::string mediaTypeInfo = media_type_info;

				//判断是否已经存在，如果已经存在则不添加
				vector<string>::iterator it;
				for( it = typeList->begin() ; it!=typeList->end() ; it++ )
				{
					if( *it == mediaTypeInfo )
					{
						break;
					}
				}
				if( it==typeList->end() )
				{	
					//resolutionList.push_back(mediaTypeInfo);
					//对每种分辨率，取枚取得到的第一个帧率
					//sprintf_s(media_type_info, "X%d", pvi->AvgTimePerFrame ? 10000000 / pvi->AvgTimePerFrame : 0);
					//mediaTypeInfo += media_type_info;
					typeList->push_back(mediaTypeInfo);
				}
			}
		}
	}

	pCaptureDevice->Release();
	pStreamConfig->Release();

}

/**
 * @brief GetAudioOutputVolumn用于获取设备输出音量
 * @remark 不指定设备可能导致获取的音量为0
 */
int DeviceManager::GetAudioOutputVolumn( const std::string &deviceName )
{
    //对于DirectSound设备，获取其对应波形设备音量
    std::string deviceFriendlyName = deviceName;
    if (deviceName.find("DirectSound: ") == 0) 
    {
        //设备名以"DirectSound: "开始
        deviceFriendlyName = deviceName.substr(strlen("DirectSound: "));
    }

	OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
		//vista或win7，采取不同的方式获取音量
		IMMDeviceEnumerator* pEnumerator;
		IMMDeviceCollection* pCollection = NULL;
		IMMDevice *pDevice = NULL;
		IPropertyStore *pProperties=NULL;  
		IAudioEndpointVolume *pVolumeAPI=NULL;  
		UINT deviceCount = 0;

		float fVolume = -1;

		CoInitializeEx( NULL , COINIT_MULTITHREADED );

		HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
		if (hr != S_OK)  
		{  
			printf("CoCreateInstance Failed!\n");  
			return 0;  
		}  

		//hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
		hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
		if (hr != S_OK)  
		{  
			printf("EnumAudioEndpoints Failed!\n");  
			goto releasepEnumerator;  
		}  

		hr = pCollection->GetCount(&deviceCount);  
		if (hr != S_OK)  
		{  
			printf("GetCount Failed!\n");  
			goto releasepCollection;  
		}  

		for (UINT dev=0;dev<deviceCount;dev++)  
		{  
			pDevice = NULL;  
			hr = pCollection->Item(dev,&pDevice);  
			if (hr == S_OK)  
			{  

				hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
				if (hr == S_OK)  
				{  
					PROPVARIANT varName;  
					PropVariantInit(&varName);  
					hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
					if( SUCCEEDED(hr) )
					{
                        if (varName.vt != VT_LPWSTR || 
                            (wstr2str(varName.pwszVal).find(deviceFriendlyName) != 0)) //传入的值可能不完整，需要前部分匹配
                        {
                          continue;
                        }
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
						if (hr==S_OK)  
						{  
							hr = pVolumeAPI->GetMasterVolumeLevelScalar( &fVolume );
                            if (S_OK == hr) break;
						} 
					}

					SAFE_RELEASE(pProperties);  
				}  

				SAFE_RELEASE(pDevice);  
			}  
		}  

        releasepCollection:  
		SAFE_RELEASE(pCollection);  
        releasepEnumerator:  
		SAFE_RELEASE(pEnumerator);  
		return  fVolume*100;
	}
    else
	{
		MMRESULT          rc;     // 多媒体函数返回结果变量
		HMIXER            hMixer; // 混合器设备句柄
		MIXERLINE         mxl;    // 音频线路标准状态信息结构体
		MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
		MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体

		// 打开混合器设备
		rc = mixerOpen(&hMixer, // 返回的设备句柄
			0,       // 单声卡的设备ID为零
			0,       // 不使用回调机制
			0,       // 回调函数参数
			0);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID

		// 打开混合器设备无错的话，则
		if (MMSYSERR_NOERROR == rc)
		{
			// MIXERLINE 结构体变量清零
			ZeroMemory(&mxl, sizeof(MIXERLINE));

			mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
			// 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

			// 取得混合器设备的指定线路信息
			rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
				&mxl,
				// 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
				MIXER_GETLINEINFOF_COMPONENTTYPE);

			// 取得混合器设备的指定线路信息成功的话，则
			if (MMSYSERR_NOERROR == rc)
			{
				// MIXERCONTROL 结构体变量清零
				ZeroMemory(&mxc, sizeof(MIXERCONTROL));

				mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本

				// MIXERLINECONTROLS 结构体变量清零
				ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));

				mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本

				mxlc.dwLineID = mxl.dwLineID; // 上面取得的声卡音频输出线路标识
				// 控制类型为控制音量
				mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
				mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
				mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
				mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小

				// 取得控制器信息
				rc = mixerGetLineControls((HMIXEROBJ)hMixer,
					&mxlc,
					MIXER_GETLINECONTROLSF_ONEBYTYPE);

				// 取得控制器信息成功的话，则
				if (MMSYSERR_NOERROR == rc)
				{
					// 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
					MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
					MIXERCONTROLDETAILS_SIGNED volStruct; // 音量结构体变量(就一个成员量)

					// MIXERCONTROLDETAILS 结构体变量清零
					ZeroMemory(&mxcd, sizeof(mxcd));

					mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
					mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
					mxcd.paDetails = &volStruct;        // 音量结构体变量指针
					mxcd.cbDetails = sizeof(volStruct); // 音量结构体变量字节大小
					mxcd.cChannels = 1;                 // 取得或设置全部通道

					//  获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum. 

					// 获得音量值
					rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
						&mxcd,
						MIXER_GETCONTROLDETAILSF_VALUE);

					if (MMSYSERR_NOERROR == rc)
					{
						long step = ( mxc.Bounds.lMaximum - mxc.Bounds.lMinimum ) /100;

						int index = ( volStruct.lValue - mxc.Bounds.lMinimum) /step ;
						printf("音量：%X", volStruct.lValue);
						return index;
					}
				}
			}
		}
	}

	return -1;
}

int DeviceManager::SetAudioOutputVolumn( int volumnIndex )
{
	OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
		//vista或win7，采取不同的方式获取音量
		IMMDeviceEnumerator* pEnumerator;
		IMMDeviceCollection* pCollection = NULL;
		IMMDevice *pDevice = NULL;
		IPropertyStore *pProperties=NULL;  
		IAudioEndpointVolume *pVolumeAPI=NULL;  
		UINT deviceCount = 0;

		float fVolume = -1;

		CoInitializeEx( NULL , COINIT_MULTITHREADED );

		HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
		if (hr != S_OK)  
		{  
			printf("CoCreateInstance Failed!\n");  
			return 0;  
		}  

		//hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
		hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
		if (hr != S_OK)  
		{  
			printf("EnumAudioEndpoints Failed!\n");  
			goto releasepEnumerator;  
		}  

		hr = pCollection->GetCount(&deviceCount);  
		if (hr != S_OK)  
		{  
			printf("GetCount Failed!\n");  
			goto releasepCollection;  
		}  

		for (UINT dev=0;dev<deviceCount;dev++)  
		{  
			pDevice = NULL;  
			hr = pCollection->Item(dev,&pDevice);  
			if (hr == S_OK)  
			{  

				hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
				if (hr == S_OK)  
				{  
					PROPVARIANT varName;  
					PropVariantInit(&varName);  
					hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
					if( SUCCEEDED(hr) )
					{
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
						if (hr==S_OK)  
						{  
							float volumnLevel = (float)volumnIndex /100;
							hr = pVolumeAPI->SetMasterVolumeLevelScalar( volumnLevel, NULL );
						} 
					}
					SAFE_RELEASE(pProperties);  

				}  

				SAFE_RELEASE(pDevice);  
			}  
		}  

releasepCollection:  

		SAFE_RELEASE(pCollection);  

releasepEnumerator:  

		SAFE_RELEASE(pEnumerator);  

		return  fVolume*100;
	}
    else
	{
		MMRESULT          rc;     // 多媒体函数返回结果变量
		HMIXER            hMixer; // 混合器设备句柄
		MIXERLINE         mxl;    // 音频线路标准状态信息结构体
		MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
		MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体

		// 打开混合器设备
		rc = mixerOpen(&hMixer, // 返回的设备句柄
			0,       // 单声卡的设备ID为零
			0,       // 不使用回调机制
			0,       // 回调函数参数
			0);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID

		// 打开混合器设备无错的话，则
		if (MMSYSERR_NOERROR == rc)
		{
			// MIXERLINE 结构体变量清零
			ZeroMemory(&mxl, sizeof(MIXERLINE));

			mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
			// 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

			// 取得混合器设备的指定线路信息
			rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
				&mxl,
				// 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
				MIXER_GETLINEINFOF_COMPONENTTYPE);

			// 取得混合器设备的指定线路信息成功的话，则
			if (MMSYSERR_NOERROR == rc)
			{
				// MIXERCONTROL 结构体变量清零
				ZeroMemory(&mxc, sizeof(MIXERCONTROL));

				mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本

				// MIXERLINECONTROLS 结构体变量清零
				ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));

				mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本

				mxlc.dwLineID = mxl.dwLineID; // 上面取得的声卡音频输出线路标识
				// 控制类型为控制音量
				mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
				mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
				mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
				mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小

				// 取得控制器信息
				rc = mixerGetLineControls((HMIXEROBJ)hMixer,
					&mxlc,
					MIXER_GETLINECONTROLSF_ONEBYTYPE);

				// 取得控制器信息成功的话，则
				if (MMSYSERR_NOERROR == rc)
				{
					// 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
					MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
					MIXERCONTROLDETAILS_SIGNED volStruct; // 音量结构体变量(就一个成员量)

					// MIXERCONTROLDETAILS 结构体变量清零
					ZeroMemory(&mxcd, sizeof(mxcd));

					mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
					mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
					mxcd.paDetails = &volStruct;        // 音量结构体变量指针
					mxcd.cbDetails = sizeof(volStruct); // 音量结构体变量字节大小
					mxcd.cChannels = 1;                 // 取得或设置全部通道

					//  获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum. 

					// 获得音量值
					rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
						&mxcd,
						MIXER_GETCONTROLDETAILSF_VALUE);

					if (MMSYSERR_NOERROR == rc)
					{
						long step = ( mxc.Bounds.lMaximum - mxc.Bounds.lMinimum ) /100;
						volStruct.lValue = volumnIndex*step;	 // 想要设置的值  
						rc = mixerSetControlDetails((HMIXEROBJ)hMixer,  
							&mxcd,  
							MIXER_SETCONTROLDETAILSF_VALUE);  

						if (MMSYSERR_NOERROR == rc)  
						{  
							return 1;
						}  
					}
				}
			}
		}
	}
	
	return -1;
}

int DeviceManager::GetAudioInputVolumn( const std::string &deviceName )
{
	OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
		//vista或win7，采取不同的方式获取音量
		IMMDeviceEnumerator* pEnumerator;
		IMMDeviceCollection* pCollection = NULL;
		IMMDevice *pDevice = NULL;
		IPropertyStore *pProperties=NULL;  
		IAudioEndpointVolume *pVolumeAPI=NULL;  
		UINT deviceCount = 0;

		float fVolume = -1;

		CoInitializeEx( NULL , COINIT_MULTITHREADED );

		HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
		if (hr != S_OK)  
		{  
			printf("CoCreateInstance Failed!\n");  
			return 0;  
		}  

		hr = pEnumerator->EnumAudioEndpoints( eCapture , DEVICE_STATE_ACTIVE , &pCollection );
		if (hr != S_OK)  
		{  
			printf("EnumAudioEndpoints Failed!\n");  
			goto releasepEnumerator;  
		}  

		hr = pCollection->GetCount(&deviceCount);  
		if (hr != S_OK)  
		{  
			printf("GetCount Failed!\n");  
			goto releasepCollection;  
		}  

		for (UINT dev=0;dev<deviceCount;dev++)  
		{  
			pDevice = NULL;  
			hr = pCollection->Item(dev,&pDevice);  
			if (hr == S_OK)  
			{  

				hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
				if (hr == S_OK)  
				{  
					PROPVARIANT varName;  
					PropVariantInit(&varName);  
					hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
					if( SUCCEEDED(hr) )
					{
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
						if (hr==S_OK)  
						{  
							hr = pVolumeAPI->GetMasterVolumeLevelScalar( &fVolume );
						} 
					}

					SAFE_RELEASE(pProperties);  

				}  

				SAFE_RELEASE(pDevice);  
			}  
		}  

releasepCollection:  

		SAFE_RELEASE(pCollection);  

releasepEnumerator:  

		SAFE_RELEASE(pEnumerator);  

		return  fVolume*100;
	}
    else
	{
		MMRESULT          rc;     // 多媒体函数返回结果变量
		HMIXER            hMixer; // 混合器设备句柄
		MIXERLINE         mxl;    // 音频线路标准状态信息结构体
		MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
		MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体

		//枚举所有声卡驱动，这个枚举很重要。很多代码调用mixOpen时只是用了第一个设备，即
		//mixerOpen(&hMixer, 0 , 0 ,0 )，但是我在这个设备下调用mixerGetLineInfo出问题了，
		//可能还是跟声卡驱动有关。
		for( int deviceID =0 ; true ; deviceID++ )
		{
			// 打开混合器设备
			rc = mixerOpen(&hMixer, // 返回的设备句柄
				deviceID,       // 单声卡的设备ID为零
				0,       // 不使用回调机制
				0,       // 回调函数参数
				MIXER_OBJECTF_MIXER);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID
			if( MMSYSERR_NOERROR!=rc )
			{
				//跳出循环
				break;
			}
			// 打开混合器设备无错的话，则

			// MIXERLINE 结构体变量清零
			ZeroMemory(&mxl, sizeof(MIXERLINE));

			mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
			// 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

			// 取得混合器设备的指定线路信息
			rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
				&mxl,
				// 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
				MIXER_OBJECTF_HMIXER|MIXER_GETLINEINFOF_COMPONENTTYPE);
			if( rc!=MMSYSERR_NOERROR )
			{
				//无法获取声音输入线路，尝试其他声卡
				continue;
			}

			// 取得混合器设备的指定线路信息成功的话，则

			// 将连接数保存 
			DWORD   dwConnections   =   mxl.cConnections; 
			// 准备获取麦克风设备的ID 
			DWORD   dwLineID = -1; 
			for( DWORD i=0 ; i<dwConnections; i++ )
			{ 
				// 枚举每一个设备，当Source的ID等于当前的迭代记数 
				mxl.dwSource = i;
				// 根据SourceID获得连接的信息 
				rc = mixerGetLineInfo(   (HMIXEROBJ)hMixer,   &mxl, 
					MIXER_OBJECTF_HMIXER   |   MIXER_GETLINEINFOF_SOURCE   ); 
				// 判断函数执行错误 
				if( MMSYSERR_NOERROR==rc )
				{ 
					// 如果当前设备类型是麦克风，则跳出循环。 
					if( mxl.dwComponentType==MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE )
					{ 
						dwLineID = mxl.dwLineID; 
						break; 
					}
				}
			}
			// 如果没有找到，返回失败。 
			if( dwLineID==-1 )
			{ 
				//尝试其他声卡
				continue;
			}

			// MIXERCONTROL 结构体变量清零
			ZeroMemory(&mxc, sizeof(MIXERCONTROL));

			mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本

			// MIXERLINECONTROLS 结构体变量清零
			ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));

			mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本

			mxlc.dwLineID = dwLineID; // 上面取得的声卡音频输入线路标识
			// 控制类型为控制音量
			mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
			mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
			mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
			mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小

			// 取得控制器信息
			rc = mixerGetLineControls((HMIXEROBJ)hMixer,
				&mxlc,
				MIXER_GETLINECONTROLSF_ONEBYTYPE);

			// 取得控制器信息成功的话，则
			if (MMSYSERR_NOERROR == rc)
			{
				// 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
				MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
				MIXERCONTROLDETAILS_SIGNED volStruct; // 音量结构体变量(就一个成员量)

				// MIXERCONTROLDETAILS 结构体变量清零
				ZeroMemory(&mxcd, sizeof(mxcd));

				mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
				mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
				mxcd.paDetails = &volStruct;        // 音量结构体变量指针
				mxcd.cbDetails = sizeof(volStruct); // 音量结构体变量字节大小
				mxcd.cChannels = 1;                 // 取得或设置全部通道

				//  获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum. 

				// 获得音量值
				rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
					&mxcd,
					MIXER_GETCONTROLDETAILSF_VALUE);

				if (MMSYSERR_NOERROR == rc)
				{
					long step = ( mxc.Bounds.lMaximum - mxc.Bounds.lMinimum ) /100;

					int index = ( volStruct.lValue - mxc.Bounds.lMinimum) /step ;
					mixerClose( hMixer );
					return index;
				}
			}
		}

		if( hMixer )
			mixerClose( hMixer );
	}

	return -1;
}

int DeviceManager::SetAudioInputVolumn( const std::string &deviceName , int volumnIndex )
{
	OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
		//vista或win7，采取不同的方式获取音量
		IMMDeviceEnumerator* pEnumerator;
		IMMDeviceCollection* pCollection = NULL;
		IMMDevice *pDevice = NULL;
		IPropertyStore *pProperties=NULL;  
		IAudioEndpointVolume *pVolumeAPI=NULL;  
		UINT deviceCount = 0;

		float fVolume = -1;

		CoInitializeEx( NULL , COINIT_MULTITHREADED );

		HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
		if (hr != S_OK)  
		{  
			printf("CoCreateInstance Failed!\n");  
			return 0;  
		}  

		hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
		//hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
		if (hr != S_OK)  
		{  
			printf("EnumAudioEndpoints Failed!\n");  
			goto releasepEnumerator;  
		}  

		hr = pCollection->GetCount(&deviceCount);  
		if (hr != S_OK)  
		{  
			printf("GetCount Failed!\n");  
			goto releasepCollection;  
		}  

		for (UINT dev=0;dev<deviceCount;dev++)  
		{  
			pDevice = NULL;  
			hr = pCollection->Item(dev,&pDevice);  
			if (hr == S_OK)  
			{  

				hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
				if (hr == S_OK)  
				{  
					PROPVARIANT varName;  
					PropVariantInit(&varName);  
					hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
					if( SUCCEEDED(hr) )
					{
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
						if (hr==S_OK)  
						{  
							float volumnLevel = (float)volumnIndex/100;
							hr = pVolumeAPI->SetMasterVolumeLevelScalar( volumnLevel , NULL );
							UINT iChannelSum=0;   
							hr = pVolumeAPI->GetChannelCount(&iChannelSum);   

							if (SUCCEEDED(hr) &&(iChannelSum>=2))   
							{   
								for (int i=0;i<iChannelSum;i++)   
								{   
									pVolumeAPI->SetChannelVolumeLevelScalar(i,volumnLevel,NULL);   
								}   
							}   
						} 
					}

					SAFE_RELEASE(pProperties);  

				}  

				SAFE_RELEASE(pDevice);  
			}  
		}  

releasepCollection:  

		SAFE_RELEASE(pCollection);  

releasepEnumerator:  

		SAFE_RELEASE(pEnumerator);  

		return  volumnIndex;
	}
    else
	{
		MMRESULT          rc;     // 多媒体函数返回结果变量
		HMIXER            hMixer; // 混合器设备句柄
		MIXERLINE         mxl;    // 音频线路标准状态信息结构体
		MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
		MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体

		//枚举所有声卡驱动，这个枚举很重要。很多代码调用mixOpen时只是用了第一个设备，即
		//mixerOpen(&hMixer, 0 , 0 ,0 )，但是我在这个设备下调用mixerGetLineInfo出问题了，
		//可能还是跟声卡驱动有关。
		for( int deviceID =0 ; true ; deviceID++ )
		{
			// 打开混合器设备
			rc = mixerOpen(&hMixer, // 返回的设备句柄
				deviceID,       // 单声卡的设备ID为零
				0,       // 不使用回调机制
				0,       // 回调函数参数
				MIXER_OBJECTF_MIXER);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID
			if( MMSYSERR_NOERROR!=rc )
			{
				//跳出循环
				break;
			}
			// 打开混合器设备无错的话，则

			// MIXERLINE 结构体变量清零
			ZeroMemory(&mxl, sizeof(MIXERLINE));

			mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
			// 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_WAVEIN;

			// 取得混合器设备的指定线路信息
			rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
				&mxl,
				// 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
				MIXER_OBJECTF_HMIXER|MIXER_GETLINEINFOF_COMPONENTTYPE);
			if( rc!=MMSYSERR_NOERROR )
			{
				//无法获取声音输入线路，尝试其他声卡
				continue;
			}

			// 取得混合器设备的指定线路信息成功的话，则

			// 将连接数保存 
			DWORD   dwConnections   =   mxl.cConnections; 
			// 准备获取麦克风设备的ID 
			DWORD   dwLineID = -1; 
			for( DWORD i=0 ; i<dwConnections; i++ )
			{ 
				// 枚举每一个设备，当Source的ID等于当前的迭代记数 
				mxl.dwSource = i;
				// 根据SourceID获得连接的信息 
				rc = mixerGetLineInfo(   (HMIXEROBJ)hMixer,   &mxl, 
					MIXER_OBJECTF_HMIXER   |   MIXER_GETLINEINFOF_SOURCE   ); 
				// 判断函数执行错误 
				if( MMSYSERR_NOERROR==rc )
				{ 
					// 如果当前设备类型是麦克风，则跳出循环。 
					if( mxl.dwComponentType==MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE )
					{ 
						dwLineID = mxl.dwLineID; 
						break; 
					}
				}
			}
			// 如果没有找到，返回失败。 
			if( dwLineID==-1 )
			{ 
				//尝试其他声卡
				continue;
			}

			// MIXERCONTROL 结构体变量清零
			ZeroMemory(&mxc, sizeof(MIXERCONTROL));

			mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本

			// MIXERLINECONTROLS 结构体变量清零
			ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));

			mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本

			mxlc.dwLineID = dwLineID; // 上面取得的声卡音频输入线路标识
			// 控制类型为控制音量
			mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
			mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
			mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
			mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小

			// 取得控制器信息
			rc = mixerGetLineControls((HMIXEROBJ)hMixer,
				&mxlc,
				MIXER_GETLINECONTROLSF_ONEBYTYPE);

			// 取得控制器信息成功的话，则
			if (MMSYSERR_NOERROR == rc)
			{
				// 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
				MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
				MIXERCONTROLDETAILS_SIGNED volStruct; // 音量结构体变量(就一个成员量)

				// MIXERCONTROLDETAILS 结构体变量清零
				ZeroMemory(&mxcd, sizeof(mxcd));

				mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
				mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
				mxcd.paDetails = &volStruct;        // 音量结构体变量指针
				mxcd.cbDetails = sizeof(volStruct); // 音量结构体变量字节大小
				mxcd.cChannels = 1;                 // 取得或设置全部通道

				//  获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum. 

				// 获得音量值
				rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
					&mxcd,
					MIXER_GETCONTROLDETAILSF_VALUE);

				if (MMSYSERR_NOERROR == rc)
				{
					long step = ( mxc.Bounds.lMaximum - mxc.Bounds.lMinimum ) /100;
					volStruct.lValue = volumnIndex*step;	 // 想要设置的值  
					rc = mixerSetControlDetails((HMIXEROBJ)hMixer,  
						&mxcd,  
						MIXER_SETCONTROLDETAILSF_VALUE);  

					if (MMSYSERR_NOERROR == rc)  
					{  
						mixerClose( hMixer );
						return 1;
					}  
				}
			}
		}

		if( hMixer )
			mixerClose( hMixer );
	}

	return -1;
}



void DeviceManager::GetDisplayDevices( DisplayDeviceList* pDeviceList )
{
	HDC hScrDC = NULL;
	DISPLAY_DEVICE device;
	device.cb = sizeof( DISPLAY_DEVICE );
	DWORD deviceNum = 0;

	pDeviceList->clear();
	while( EnumDisplayDevicesA( NULL ,deviceNum++, &device, NULL ) )
	{
		
		hScrDC = CreateDC( device.DeviceName , NULL , NULL , NULL );
		// 获得设备信息
		DisplayDevice displayDevice;
		displayDevice._deviceName = device.DeviceName;
		char friendlyName[36] = {0};
		_snprintf( friendlyName,36 , "显示器%d" , deviceNum );
		displayDevice._friendlyName = friendlyName;
		displayDevice._xResolution = GetDeviceCaps(hScrDC, HORZRES);
		displayDevice._yResolution = GetDeviceCaps(hScrDC, VERTRES);
		if( displayDevice._xResolution==0 || displayDevice._yResolution==0 )
			continue;

		//枚举显示器可用的配置参数
		DEVMODE deviceMode;
		ZeroMemory( &deviceMode , sizeof(DEVMODE) );
		deviceMode.dmSize = sizeof(DEVMODE);
		DWORD modeNum = 0;
		DWORD bitsPerPel = 0;

		//获取当前的显示器设置信息，主要是颜色位数
		if( EnumDisplaySettings(device.DeviceName, ENUM_CURRENT_SETTINGS,&deviceMode) )
		{
			bitsPerPel = deviceMode.dmBitsPerPel;
		}
		
		//开始枚举
		char resolution[36]={0};
		while( EnumDisplaySettings( device.DeviceName , modeNum++, &deviceMode ) )
		{
			DWORD height = deviceMode.dmPelsHeight;
			DWORD width = deviceMode.dmPelsWidth;
			
			if( width>800 && height>600&& bitsPerPel==deviceMode.dmBitsPerPel )
			{
				if( height==displayDevice._yResolution && width==displayDevice._xResolution )
					_snprintf( resolution , 36, "%d x %d （当前配置）" , width , height );
				else
					_snprintf( resolution , 36, "%d x %d" , width , height );
				vector<string>::iterator it = std::find( displayDevice._resolutionList.begin(), displayDevice._resolutionList.end(),
					resolution );
				if( it==displayDevice._resolutionList.end() )
				{
					displayDevice._resolutionList.push_back(  resolution );
				}
			}
		}
		//最后的一行是推荐配置
		int lastIndex = displayDevice._resolutionList.size()-1;
		displayDevice._resolutionList[lastIndex] = displayDevice._resolutionList[lastIndex] + "（推荐）";

		pDeviceList->push_back( displayDevice );

	}
	if( hScrDC )
	{
		DeleteDC(hScrDC);
	}
}

int DeviceManager::SetDisplayDevice( const DisplayDevice& newDevice, long& code )
{
	DEVMODE deviceMode;
	ZeroMemory( &deviceMode , sizeof(DEVMODE) );
	deviceMode.dmSize = sizeof(DEVMODE);
	if( EnumDisplaySettings( newDevice._deviceName.c_str() , ENUM_CURRENT_SETTINGS, &deviceMode ) )
	{
		deviceMode.dmPelsHeight = newDevice._yResolution;
		deviceMode.dmPelsWidth = newDevice._xResolution;

		code = ChangeDisplaySettings(&deviceMode,CDS_UPDATEREGISTRY);   
		if (code==DISP_CHANGE_SUCCESSFUL)   
		{    
			return 1;
		}   
		else  
		{   
			return -1;
		}  
	}

	return 0;
}

int DeviceManager::GetAudioOutputMute( const std::string &deviceName )
{
    OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
		//vista或win7，采取不同的方式获取静音
		IMMDeviceEnumerator* pEnumerator;
		IMMDeviceCollection* pCollection = NULL;
		IMMDevice *pDevice = NULL;
		IPropertyStore *pProperties=NULL;  
		IAudioEndpointVolume *pVolumeAPI=NULL;  
		UINT deviceCount = 0;

		BOOL bMute = FALSE;
        BOOL bFindDevice = FALSE;

		CoInitializeEx( NULL , COINIT_MULTITHREADED );

		HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
		if (hr != S_OK)  
		{  
			printf("CoCreateInstance Failed!\n");  
			return 0;  
		}  

		//hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
		hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
		if (hr != S_OK)  
		{  
			printf("EnumAudioEndpoints Failed!\n");  
			goto releasepEnumerator;  
		}  

		hr = pCollection->GetCount(&deviceCount);  
		if (hr != S_OK)  
		{  
			printf("GetCount Failed!\n");  
			goto releasepCollection;  
		}  

		for (UINT dev=0;dev<deviceCount;dev++)  
		{  
			pDevice = NULL;  
			hr = pCollection->Item(dev,&pDevice);  
			if (hr == S_OK)  
			{  

				hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
				if (hr == S_OK)  
				{  
					PROPVARIANT varName;  
					PropVariantInit(&varName);  
					hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
					if( SUCCEEDED(hr) )
					{
                        if (varName.vt != VT_LPWSTR || 
                            (wstr2str(varName.pwszVal).find(deviceName) != 0)) //传入的值可能不完整，需要前部分匹配
                        {
                          continue;
                        }
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
						if (hr==S_OK)  
						{  
							hr = pVolumeAPI->GetMute(&bMute);
                            bFindDevice = TRUE;
						} 
					}

					SAFE_RELEASE(pProperties);  
				}  

				SAFE_RELEASE(pDevice);  
			}
            if (bFindDevice) break;
		}  

releasepCollection:  

		SAFE_RELEASE(pCollection);  

releasepEnumerator:  

		SAFE_RELEASE(pEnumerator);  

		return  hr == S_OK ? bMute : -1;
	}
    else
	{
		MMRESULT          rc;     // 多媒体函数返回结果变量
		HMIXER            hMixer; // 混合器设备句柄
		MIXERLINE         mxl;    // 音频线路标准状态信息结构体
		MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
		MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体


		// 打开混合器设备
		rc = mixerOpen(&hMixer, // 返回的设备句柄
			0,       // 单声卡的设备ID为零
			0,       // 不使用回调机制
			0,       // 回调函数参数
			0);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID

		// 打开混合器设备无错的话，则
		if (MMSYSERR_NOERROR == rc)
		{
			// MIXERLINE 结构体变量清零
			ZeroMemory(&mxl, sizeof(MIXERLINE));

			mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
			// 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

			// 取得混合器设备的指定线路信息
			rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
				&mxl,
				// 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
				MIXER_GETLINEINFOF_COMPONENTTYPE);

			// 取得混合器设备的指定线路信息成功的话，则
			if (MMSYSERR_NOERROR == rc)
			{
				// MIXERCONTROL 结构体变量清零
				ZeroMemory(&mxc, sizeof(MIXERCONTROL));

				mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本

				// MIXERLINECONTROLS 结构体变量清零
				ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));

				mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本

				mxlc.dwLineID = mxl.dwLineID; // 上面取得的声卡音频输出线路标识
				// 控制类型为控制静音
				mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
				mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
				mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
				mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小

				// 取得控制器信息
				rc = mixerGetLineControls((HMIXEROBJ)hMixer,
					&mxlc,
					MIXER_GETLINECONTROLSF_ONEBYTYPE);

				// 取得控制器信息成功的话，则
				if (MMSYSERR_NOERROR == rc)
				{
					// 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
					MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
                    MIXERCONTROLDETAILS_BOOLEAN mcdMute;

					// MIXERCONTROLDETAILS 结构体变量清零
					ZeroMemory(&mxcd, sizeof(mxcd));

					mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
					mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
					mxcd.paDetails = &mcdMute;        // 静音结构体变量指针
					mxcd.cbDetails = sizeof(mcdMute); // 静音结构体变量字节大小
					mxcd.cChannels = 1;                 // 取得或设置全部通道

					// 获得静音值
					rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
						&mxcd,
						MIXER_GETCONTROLDETAILSF_VALUE);

					if (MMSYSERR_NOERROR == rc)
					{
                        BOOL bMute = mcdMute.fValue;
                        return bMute;
					}
				}
			}
		}
	}

	return -1;
}

int DeviceManager::SetAudioOutputMute( bool isMute )
{
    OSVERSIONINFO osVer;
	osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx( &osVer );
	if( osVer.dwMajorVersion==6 )
	{
		//vista或win7，采取不同的方式获取静音
		IMMDeviceEnumerator* pEnumerator;
		IMMDeviceCollection* pCollection = NULL;
		IMMDevice *pDevice = NULL;
		IPropertyStore *pProperties=NULL;  
		IAudioEndpointVolume *pVolumeAPI=NULL;  
		UINT deviceCount = 0;

		CoInitializeEx( NULL , COINIT_MULTITHREADED );

		HRESULT hr=CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),(void**)&pEnumerator);
		if (hr != S_OK)  
		{  
			printf("CoCreateInstance Failed!\n");  
			return 0;  
		}  

		//hr = pEnumerator->EnumAudioEndpoints(eCapture /*eRender*/, DEVICE_STATE_ACTIVE, &pCollection);
		hr = pEnumerator->EnumAudioEndpoints( eRender , DEVICE_STATE_ACTIVE , &pCollection );
		if (hr != S_OK)  
		{  
			printf("EnumAudioEndpoints Failed!\n");  
			goto releasepEnumerator;  
		}  

		hr = pCollection->GetCount(&deviceCount);  
		if (hr != S_OK)  
		{  
			printf("GetCount Failed!\n");  
			goto releasepCollection;  
		}  

		for (UINT dev=0;dev<deviceCount;dev++)  
		{  
			pDevice = NULL;  
			hr = pCollection->Item(dev,&pDevice);  
			if (hr == S_OK)  
			{  

				hr = pDevice->OpenPropertyStore(STGM_READ,&pProperties);  
				if (hr == S_OK)  
				{  
					PROPVARIANT varName;  
					PropVariantInit(&varName);  
					hr = pProperties->GetValue(PKEY_Device_FriendlyName, &varName); 
					if( SUCCEEDED(hr) )
					{
						hr=pDevice->Activate(__uuidof(IAudioEndpointVolume),CLSCTX_ALL,NULL,(void **)(&pVolumeAPI));  
						if (hr==S_OK)  
						{  
							hr = pVolumeAPI->SetMute(isMute ? TRUE : FALSE, NULL);
						} 
					}
					SAFE_RELEASE(pProperties);  

				}  

				SAFE_RELEASE(pDevice);  
			}  
		}  

releasepCollection:  

		SAFE_RELEASE(pCollection);  

releasepEnumerator:  

		SAFE_RELEASE(pEnumerator);  

		return  hr == S_OK ? 0 : -1;
	}
    else
	{
		MMRESULT          rc;     // 多媒体函数返回结果变量
		HMIXER            hMixer; // 混合器设备句柄
		MIXERLINE         mxl;    // 音频线路标准状态信息结构体
		MIXERLINECONTROLS mxlc;   // 音频线路控制器集合信息结构体
		MIXERCONTROL      mxc;    // 单个音频线路控制器信息结构体

		// 打开混合器设备
		rc = mixerOpen(&hMixer, // 返回的设备句柄
			0,       // 单声卡的设备ID为零
			0,       // 不使用回调机制
			0,       // 回调函数参数
			0);      // MIXER_OBJECTF_MIXER宏的值，表示任选有效设备ID

		// 打开混合器设备无错的话，则
		if (MMSYSERR_NOERROR == rc)
		{
			// MIXERLINE 结构体变量清零
			ZeroMemory(&mxl, sizeof(MIXERLINE));

			mxl.cbStruct = sizeof(MIXERLINE); // 微软用此办法判断版本
			// 指出需要获取的通道，声卡的音频输出用MIXERLINE_COMPONENTTYPE_DST_SPEAKERS
			mxl.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;

			// 取得混合器设备的指定线路信息
			rc = mixerGetLineInfo((HMIXEROBJ)hMixer,
				&mxl,
				// 取得MIXERLINE::dwComponentType指定类型的第一个音频线路信息
				MIXER_GETLINEINFOF_COMPONENTTYPE);

			// 取得混合器设备的指定线路信息成功的话，则
			if (MMSYSERR_NOERROR == rc)
			{
				// MIXERCONTROL 结构体变量清零
				ZeroMemory(&mxc, sizeof(MIXERCONTROL));

				mxc.cbStruct = sizeof(mxc); // 微软用此办法判断版本

				// MIXERLINECONTROLS 结构体变量清零
				ZeroMemory(&mxlc, sizeof(MIXERLINECONTROLS));

				mxlc.cbStruct = sizeof(mxlc); // 微软用此办法判断版本

				mxlc.dwLineID = mxl.dwLineID; // 上面取得的声卡音频输出线路标识
				// 控制类型为控制静音
				mxlc.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
				mxlc.cControls = 1;          // 使用 MIXERCONTROL 结构体变量个数
				mxlc.pamxctrl = &mxc;        // MIXERCONTROL 结构体变量指针
				mxlc.cbmxctrl = sizeof(mxc); // MIXERCONTROL 结构体变量字节大小

				// 取得控制器信息
				rc = mixerGetLineControls((HMIXEROBJ)hMixer,
					&mxlc,
					MIXER_GETLINECONTROLSF_ONEBYTYPE);

				// 取得控制器信息成功的话，则
				if (MMSYSERR_NOERROR == rc)
				{
					// 获取控制器中的值的音量范围：mxc.Bounds.lMinimum到mxc.Bounds.lMaximum.
					MIXERCONTROLDETAILS        mxcd;      // 控制器的状态信息
                    MIXERCONTROLDETAILS_BOOLEAN mcdMute;

					// MIXERCONTROLDETAILS 结构体变量清零
					ZeroMemory(&mxcd, sizeof(mxcd));

					mxcd.cbStruct = sizeof(mxcd);       // 微软用此办法判断版本
					mxcd.dwControlID = mxc.dwControlID; // 上面取得的控制器标识
					mxcd.paDetails = &mcdMute;        // 静音结构体变量指针
					mxcd.cbDetails = sizeof(mcdMute); // 静音结构体变量字节大小
					mxcd.cChannels = 1;                 // 取得或设置全部通道

					// 获得静音值
					rc = mixerGetControlDetails((HMIXEROBJ)hMixer,
						&mxcd,
						MIXER_GETCONTROLDETAILSF_VALUE);

					if (MMSYSERR_NOERROR == rc)
					{
						mcdMute.fValue = isMute ? TRUE : FALSE;	 // 想要设置的值  
						rc = mixerSetControlDetails((HMIXEROBJ)hMixer,  
							&mxcd,  
							MIXER_SETCONTROLDETAILSF_VALUE);  

						if (MMSYSERR_NOERROR == rc)  
						{  
							return 0;
						}  
					}
				}
			}
		}
	}
	
	return -1;
}

