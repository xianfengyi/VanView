/************************************************************************/
/* 本文件定义graph的控制类和控制类工厂类的对外导出接口                  */
/* 使用接口的目的是，隐藏具体控制类的实现，以减少暴漏不必要的成员。     */
/************************************************************************/

#ifndef I_GRAPH_CONTROLLER_H
#define I_GRAPH_CONTROLLER_H

#include "DxDefinitions.h"
//本来这导出文件不该引入别的文件，但是真的要用到，没办法

#include <string>
#include <vector>
#include "msdx_global.h"


namespace msdx //注意所有定义都在msdx命名空间中
{

//设备测试Graph控制器接口
typedef struct
{
    //video/audio只能二选一
    struct Video
    {
        std::string crossbar; //采集卡设备DisplayName
        long crossbar_physical_type; //采集卡输入索引
        std::string device; //摄像头display_name
        unsigned int frame_width;
        unsigned int frame_height;
        VideoWindow preview;
    } video;

    struct Audio
    {
        //音频测试仅支持默认采样率和声道
        std::string input_device; //设备display_name,优先
        std::string input_file; //文件名。同时指定了输入设备时，此项无效
        std::string output_device;
    } audio;
} TestGraphParam;

class MSDXAPI ITestGraphController {
public:
    virtual ~ITestGraphController() {}

    virtual dxStatus Build(TestGraphParam &) = 0;
    virtual dxStatus Run() = 0;
    virtual dxStatus Stop() = 0;
};



//用于创建graph对象的工厂类
class MSDXAPI GraphControllerFactory
{
private:
    GraphControllerFactory(void);
    ~GraphControllerFactory(void);

public:
    static GraphControllerFactory &GetInstance();

    ITestGraphController *CreateTestGraphController();
};

} // end namespace msdx

#endif
