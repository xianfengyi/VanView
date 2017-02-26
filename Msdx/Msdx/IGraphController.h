/************************************************************************/
/* ���ļ�����graph�Ŀ�����Ϳ����๤����Ķ��⵼���ӿ�                  */
/* ʹ�ýӿڵ�Ŀ���ǣ����ؾ���������ʵ�֣��Լ��ٱ�©����Ҫ�ĳ�Ա��     */
/************************************************************************/

#ifndef I_GRAPH_CONTROLLER_H
#define I_GRAPH_CONTROLLER_H

#include "DxDefinitions.h"
//�����⵼���ļ������������ļ����������Ҫ�õ���û�취

#include <string>
#include <vector>
#include "msdx_global.h"


namespace msdx //ע�����ж��嶼��msdx�����ռ���
{

//�豸����Graph�������ӿ�
typedef struct
{
    //video/audioֻ�ܶ�ѡһ
    struct Video
    {
        std::string crossbar; //�ɼ����豸DisplayName
        long crossbar_physical_type; //�ɼ�����������
        std::string device; //����ͷdisplay_name
        unsigned int frame_width;
        unsigned int frame_height;
        VideoWindow preview;
    } video;

    struct Audio
    {
        //��Ƶ���Խ�֧��Ĭ�ϲ����ʺ�����
        std::string input_device; //�豸display_name,����
        std::string input_file; //�ļ�����ͬʱָ���������豸ʱ��������Ч
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



//���ڴ���graph����Ĺ�����
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
