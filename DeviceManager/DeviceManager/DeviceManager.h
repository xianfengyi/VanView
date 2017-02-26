#pragma  once

#ifdef   DEVICEMANAGER_EXPORTS
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
	int deviceNo;//每一个设备有个设备号，从1开始递增
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

///存储显示器信息的类
class DisplayDevice
{
public:
	//An array of characters identifying the device name. 
	//This is either the adapter device or the monitor device.
	std::string	_deviceName;

	//An array of characters containing the device context string. 
	//This is either a description of the display adapter or of the display monitor
	std::string  _friendlyName;

	//当前的屏幕分辨率
	int		_xResolution;
	int		_yResolution;

	//可用分辨率列表
	std::vector<std::string>	_resolutionList;
};
typedef std::vector<DisplayDevice>	DisplayDeviceList;

class DEVICE_MANAGER_API DeviceManager 
{
 public:
    static DeviceManager* GetInstatnce() 
    {
        static DeviceManager manager;
        return &manager;
    }

    //重载视频设备get函数 by Patrick
    void GetVideoCaptureDevices(VCapDeviceList* deviceList);
    /**
    * @brief 获取视频设备
    */
    void GetVideoCaptureDevices(std::vector<std::string>* deviceList);

    ///@brief 获取音频输入设备
    void GetAudioCaptureDevices(std::vector<std::string>* deviceList);

    ///@brief 获取音频输出设备
    void GetAudioOutputDevices(std::vector<std::string>* deviceList);

    /**
    * @brief 用于获取Crossbar设备列表。通常高清才需要用到
    * @param captureDevice 视频设备
    */
    void GetCrossbarDeviceList( const std::string captureDevice, std::vector<std::string> *deviceList );
    /**
    * @brief 用于获取指定crossbar的输入类型
    * @return crossbar支持的媒体类型
    */
    void GetCrossbarInputType( const std::string &crossbarDisplayName , std::vector<std::string> *typeList  );
    //*************************************

    /** 
    * @brief 获取当前设备支持的媒体类型
    */
    void GetVideoDeviceMediaType( const std::string &deviceName , std::vector<std::string>* typeList );

    /**
    * @brief 获取当前声音输出设备的音量，
    * @return 0-99的数值，赋值表示失败
    */
    int GetAudioOutputVolumn( const std::string &deviceName = "" );

    /**
    * @brief 设置声音输出设备的音量
    * @return 非负表示成功
    */
    int SetAudioOutputVolumn( int volumnIndex );

    /**
    * @brief 获取当前声音输出设备的静音，
    * @return 1表示静音，0表示没有静音，-1表示失败
    */
    int GetAudioOutputMute(const std::string &deviceName = "");

    /**
    * @brief 设置声音输出设备的音量
    * @return 0表示成功， -1表示失败
    */
    int SetAudioOutputMute(bool isMute);

    /**
    * @brief 获取特定mic的输入音量
    * @return 0-99的数值，负值表示失败
    */
    int GetAudioInputVolumn( const std::string &deviceName );

    /**
    * @brief 设置特定mic的输出音量
    * @param deviceName mic设备名称
    * @param volumnIndex 音量值 ， 0-99
    */
    int SetAudioInputVolumn( const std::string &deviceName , int volumnIndex );

    /**
    * @brief 获取显示设备的信息
    * @param deviceList 输出参数，将输出显示设备列表
    */
    void GetDisplayDevices( DisplayDeviceList* deviceList );

    /**
    * @brief 设置屏幕分辨率
    * @param newDevice 新的显示参数
    * @param code 输出参数，返回执行结果
    * @return 成功返回1，失败返回-1，如果没有找到设备则返回0
    */
    int SetDisplayDevice( const DisplayDevice& newDevice, long& code );

 private:
    DeviceManager();
    DeviceManager(const DeviceManager&);
    ~DeviceManager();
    DeviceManager& operator=(const DeviceManager&);
};