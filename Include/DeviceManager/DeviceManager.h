#pragma  once

#ifdef DEVICEMANAGER_EXPORTS
#define  DEVICE_MANAGER_API		__declspec(dllexport)
#else
#define	 DEVICE_MANAGER_API		__declspec(dllimport)
#endif

#include <string>
#include <vector>

struct VCapDevice
{
	std::string friendlyName;//human-readable name for the device
	std::string devicePath;	 //unique for each video capture device
	std::string crossBarType;
	int deviceNo;//ÿһ���豸�и��豸�ţ���1��ʼ����
	int width;
	int height;
	int fps;

	VCapDevice() :
		deviceNo(0),
		width(0),
		height(0),
		fps(0)
	{}
};
typedef std::vector<VCapDevice>		VCapDeviceList;

///������״̬��Ϣ
enum InterfaceStatus
{

	//WAN adapter that is not connected.
	IF_STATUS_UNREACHABLE,  

	//WAN adapter that is in the process of connecting.
	IF_STATUS_CONNECTING , 

	//WAN adapter that is connected to a remote peer.
	IF_STATUS_CONNECTED  ,

	//LAN adapter has been disabled, for example because of an address conflict.
	IF_STATUS_NON_OPERATIONAL , 

	//For LAN adapters: network cable disconnected. For WAN adapters: no carrier.
	IF_STATUS_DISCONNECTED , 

	//Default status for LAN adapters
	IF_STATUS_OPERATIONAL  

};


///�洢��ʾ����Ϣ����
class DisplayDevice
{
public:
	//An array of characters identifying the device name. 
	//This is either the adapter device or the monitor device.
	std::string	_deviceName;

	//An array of characters containing the device context string. 
	//This is either a description of the display adapter or of the display monitor
	std::string  _friendlyName;

	//��ǰ����Ļ�ֱ���
	int		_xResolution;
	int		_yResolution;

	//���÷ֱ����б�
	std::vector<std::string>	_resolutionList;
};
typedef std::vector<DisplayDevice>	DisplayDeviceList;

class DEVICE_MANAGER_API DeviceManager {
 public:
  static DeviceManager* GetInstatnce() {
	static DeviceManager manager;
	return &manager;
  }

  //������Ƶ�豸get���� by Patrick
  void GetVideoCaptureDevices(VCapDeviceList* deviceList);
  /**
	* @brief ��ȡ��Ƶ�豸
	*/
  void GetVideoCaptureDevices(std::vector<std::string>* deviceList);

  ///@brief ��ȡ��Ƶ�����豸
  void GetAudioCaptureDevices(std::vector<std::string>* deviceList);

  ///@brief ��ȡ��Ƶ����豸
  void GetAudioOutputDevices(std::vector<std::string>* deviceList);

  /**
	* @brief ���ڻ�ȡCrossbar�豸�б�ͨ���������Ҫ�õ�
	* @param captureDevice ��Ƶ�豸
	*/
  void GetCrossbarDeviceList( const std::string captureDevice, std::vector<std::string> *deviceList );
  /**
	* @brief ���ڻ�ȡָ��crossbar����������
	* @return crossbar֧�ֵ�ý������
	*/
  void GetCrossbarInputType( const std::string &crossbarDisplayName , std::vector<std::string> *typeList  );
  //*************************************

  /** 
	* zhenHua.sun 2010-10-20
	* @brief ��ȡ��ǰ�豸֧�ֵ�ý������
	*/
  void GetVideoDeviceMediaType( const std::string &deviceName , std::vector<std::string>* typeList );

  /**
	* zhenHua.sun 2010-12-23
	* @brief ��ȡ��ǰ��������豸��������
	* @return 0-99����ֵ����ֵ��ʾʧ��
	*/
  int GetAudioOutputVolumn( const std::string &deviceName = "" );

  /**
	* zhenHua.sun 2010-12-23
	* @brief ������������豸������
	* @return �Ǹ���ʾ�ɹ�
	*/
  int SetAudioOutputVolumn( int volumnIndex );

  /**
	* @brief ��ȡ��ǰ��������豸�ľ�����
	* @return 1��ʾ������0��ʾû�о�����-1��ʾʧ��
	*/
  int GetAudioOutputMute(const std::string &deviceName = "");

  /**
	* @brief ������������豸������
	* @return 0��ʾ�ɹ��� -1��ʾʧ��
	*/
  int SetAudioOutputMute(bool isMute);

  /**
	* @brief ��ȡ�ض�mic����������
	* @return 0-99����ֵ����ֵ��ʾʧ��
	*/
  int GetAudioInputVolumn( const std::string &deviceName );

  /**
	* @brief �����ض�mic���������
	* @param deviceName mic�豸����
	* @param volumnIndex ����ֵ �� 0-99
	*/
  int SetAudioInputVolumn( const std::string &deviceName , int volumnIndex );

  /**
	* @brief ��ȡ�����б�
	* @param adaptorList ������������������������豸����
	*/
  void GetNetworkAdaptorList( std::vector<std::string> *adaptorList );

  /**
	* @brief ��ȡָ�������ľ������
	* @param ip		ip��ַ
	* @param submask	��������
	* @param defaultGateway Ĭ������
	* @param dnsServer dns������
	*/
  void GetNetworkAdaptorInfo( const std::string& adaptorName ,std::string* ip , std::string* submask , 
	  std::string* defaultGateway, std::string* dnsServer, bool* enableDHCP );

  /**
	* @brief ����ָ�������ľ������
	* @param ip		ip��ַ
	* @param submask	��������
	* @param defaultGateway Ĭ������
	* @param dnsServer dns������
	*/
  void SetNetworkAdaptorInfo( const std::string& adaptorName ,std::string& ip , std::string& submask , 
	  std::string& defaultGateway, std::string& dnsServer, bool enableDHCP );

  /**
	* @brief ��ȡ����������״̬
	* @param mac		������Ӧ��mac��ַ
	* @param state		������״̬���ο�GetIfEntry�ķ���ֵ
	* @return �ɹ�����0��ʧ�ܷ���-1
	*/
  int GetNetworkAdaptorState( const std::string& mac , InterfaceStatus& status );


  /**
	* @brief ��ȡ��ʾ�豸����Ϣ
	* @param deviceList ����������������ʾ�豸�б�
	*/
  void GetDisplayDevices( DisplayDeviceList* deviceList );

  /**
	* @brief ������Ļ�ֱ���
	* @param newDevice �µ���ʾ����
	* @param code �������������ִ�н��
	* @return �ɹ�����1��ʧ�ܷ���-1�����û���ҵ��豸�򷵻�0
	*/
  int SetDisplayDevice( const DisplayDevice& newDevice, long& code );

 private:
  DeviceManager();
  DeviceManager(const DeviceManager&);
  ~DeviceManager();
  DeviceManager& operator=(const DeviceManager&);
};