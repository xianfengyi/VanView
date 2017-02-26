#include "stdafx.h"
#include <string>
#include "ExportUtil.h"
#include "IGraphController.h"
#include "util.h"
#include "LogUtil.h"

using namespace std;


wstring  AudioDeviceList[20];
int NumberOfAudioDevice = 0;

wstring AudioRenderDeviceList[50];
int NumberOfAudioRenderDevice = 0;

wstring  VideoDeviceList[20];
int NumberOfVideoDevice = 0;

//for ��Ƶ�ɼ���crossbar add by chenjunlin  2010-7-8
wstring Crossbar[20];
int NumberOfCrossbar = 0;


HRESULT ListDevicesByCategory(const CLSID & clsidDeviceClass, wstring * pWStringArray, int & NumOfDevices)
{
    //�豸ö����Interface
    ICreateDevEnum *pDevEnum = NULL;

    //����ö��Interface
    IEnumMoniker *pEnum = NULL;

    // Create the System Device Enumerator.
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL,
        CLSCTX_INPROC_SERVER, IID_ICreateDevEnum,
        reinterpret_cast<void**>(&pDevEnum)); //�����豸ö��COM����

    if (SUCCEEDED(hr))
    {
        // Create an enumerator for the video capture category.
        hr = pDevEnum->CreateClassEnumerator(
            clsidDeviceClass,	//CLSID_VideoInputDeviceCategory or CLSID_AudioInputDeviceCategory
            &pEnum, 0); //������Ƶ�ɼ��豸ö��COM����
    }
    else if (pDevEnum==NULL)
    {
        //����ʧ��
        return E_FAIL; //���û���豸������
    }

    NumOfDevices = 0;

    IMoniker *pMoniker = NULL;
    if(pEnum == NULL )
    {
        pDevEnum->Release();
        return E_FAIL; //���û���豸������
    }

    while (pEnum->Next(1, &pMoniker, NULL) == S_OK) //����ö�٣�ֱ��Ϊ��
    {
        IPropertyBag *pPropBag;
        hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
            (void**)(&pPropBag));
        if (FAILED(hr))
        {
            pMoniker->Release();
            continue; // Skip this one, maybe the next one will work.
        }
        // Find the description or friendly name.
        VARIANT varName;
        VariantInit(&varName);

        hr = pPropBag->Read(L"Description", &varName, 0);
        if (FAILED(hr))
        {
            hr = pPropBag->Read(L"FriendlyName", &varName, 0); //�豸�Ѻ�����
        }
        if (SUCCEEDED(hr))
        {
            // Add it to the application's list box.
            //	char displayName[1024];
            //	WideCharToMultiByte(CP_ACP,0,varName.bstrVal,-1,displayName,1024,"",NULL);

            pWStringArray[NumOfDevices++] = varName.bstrVal; //varName.bstrVal;
            VariantClear(&varName);

            WCHAR * wszDiaplayName = NULL;
            pMoniker->GetDisplayName(NULL, NULL, &wszDiaplayName);
            pWStringArray[NumOfDevices++] = wszDiaplayName;
            CoTaskMemFree(wszDiaplayName);
            //	SysFreeString(varName.bstrVal);
        }

        pPropBag->Release();
        pMoniker->Release();
    }

    pEnum->Release();
    pDevEnum->Release();
    return S_OK;

}

HRESULT  ListAudioCaptureDevice(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(CLSID_AudioInputDeviceCategory, pWStringArray, NumOfDevices);
}

HRESULT  ListAudioRenderDevice(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(CLSID_AudioRendererCategory, pWStringArray, NumOfDevices);
}

HRESULT   ListVideoCaptureDevice(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(CLSID_VideoInputDeviceCategory, pWStringArray, NumOfDevices);
}

HRESULT  ListVideoCrossbar(wstring * pWStringArray, int & NumOfDevices)
{
    return ListDevicesByCategory(AM_KSCATEGORY_CROSSBAR, pWStringArray, NumOfDevices);
}


//�⺯�������Ȼ�Ƿ��ͺͽ��չ��õģ�̫�����ˣ�Ҳֻ�ܽ�������
void MSDXAPI msdx_config_set_audio_device(
    const char* input_device , //���������豸
    const char* output_device) //��������豸����������Ҳ��
{
    /* if (input_device) send_graph_param.audio.input_device = input_device;
    if (output_device) send_graph_param.audio.output_device = output_device;
    if (output_device) recv_graph_param.audio.output_device = output_device;*/
}


int GetAudioSourceDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name)
{
    HRESULT hr = ListAudioCaptureDevice(AudioDeviceList, NumberOfAudioDevice);
    CHK_FAIL_MSG_RETURN(FAILED(hr), -1, "Enum audio capture devices failed.");

    //���ҵ��趨���豸��DisplayName
    std::wstring display_name_w = AudioDeviceList[1]; //�Ҳ���������Ĭ���豸
    for (int i = 0; i < NumberOfAudioDevice; i += 2) 
    {
        if (AudioDeviceList[i] == Util::str2wstr(friendly_name))
        {
            display_name_w = AudioDeviceList[i+1]; //ż��ΪFriendlyName����������Ϊ��ӦDisplayName
            break;
        }
    }
    display_name = Util::wstr2str(display_name_w);
    //�ǲ��Ǳ�������ֽ�ת����ת��ȥŪ���ˣ�����ң���ǰʦ���õĿ��ֽڣ������㡭��

    LOG_PRINTF("Audio source: %s \n\tDevice displayName: %s\n", 
        friendly_name.c_str(), display_name.c_str());

    return 0;
}

int GetAudioRendererDisplayNameFromFriendlyName(std::string friendly_name, std::string &display_name)
{
    CHK_FAIL_MSG_RETURN(friendly_name.empty(), -1, "Null audio renderer device friendlyName");

    HRESULT hr = ListAudioRenderDevice(AudioRenderDeviceList, NumberOfAudioRenderDevice);
    CHK_FAIL_MSG_RETURN(FAILED(hr), -1, "Get audio renderer devices failed: 0x%08x", hr);
    CHK_FAIL_MSG_RETURN(NumberOfAudioRenderDevice == 0, -1, "System has no audio renderer devices.");

    //ȡ�ò����豸��Ӧ��DirectSound�豸��������MSDN��ֻҪ����"DirectSound: "(ע��ð�ź��пո�)ǰ׺����
    const wchar_t *directsound_prefix = L"DirectSound: ";
    std::wstring audio_renderer_friendly_name = Util::str2wstr(friendly_name);
    std::wstring audio_renderer_display_name;
    bool fuzzy_match = false; //�Ƿ�ģ��ƥ��

    if (audio_renderer_friendly_name.find(directsound_prefix) != 0) {
        //����"DirectSound: "��ͷ�Ĳ���DirectSound�豸�����Թ�����Ӧ��DirectSound�豸
        //��������ƥ��ʱ�и����⣬�����豸�����ܳ���31�ֽڣ������ǲ������ģ����磺
        //  �����豸����Headphone (VIA HD Audio(Compati
        //  ��ӦDirectSound�豸����DirectSound: Headphone (VIA HD Audio(Compatible))
        //������湹�������ƿ��ܲ�������ֻ��ǰ�벿��ƥ�䣬��fuzzy_match��Ϊtrue
        audio_renderer_friendly_name = std::wstring(directsound_prefix) + audio_renderer_friendly_name;
        fuzzy_match = true;
    }

    for (int i = 0; i < NumberOfAudioRenderDevice; i += 2) 
    {
        if ((fuzzy_match && AudioRenderDeviceList[i].find(audio_renderer_friendly_name) == 0) ||
            AudioRenderDeviceList[i] == audio_renderer_friendly_name)
        {
            audio_renderer_display_name = AudioRenderDeviceList[i+1]; //��������ΪDisplayName��ż��ΪFriendlyName
            break;
        }
    }

    CHK_FAIL_MSG_RETURN(audio_renderer_display_name.empty(), -1, 
      L"Audio renderer device not exist: %s", audio_renderer_display_name.c_str());

    LOG_PRINTF(L"Audio renderer: %s \n\tCorresponding DirectSound device: %s \n\tDirectSound device displayName: %s\n", 
        Util::str2wstr(friendly_name).c_str(), 
        audio_renderer_friendly_name.c_str(), audio_renderer_display_name.c_str());

    display_name = Util::wstr2str(audio_renderer_display_name);
    return 0;
}

