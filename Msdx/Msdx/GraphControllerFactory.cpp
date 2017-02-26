#include "IGraphController.h"
#include "TestGraphController.h"


using namespace msdx;

GraphControllerFactory &GraphControllerFactory::GetInstance()
{
    //ʹ��C++11��̬��ʼ�����Ա�֤������ʼ�����̵��̰߳�ȫ
    static GraphControllerFactory instance;
    return instance;
}

GraphControllerFactory::GraphControllerFactory(void)
{
}


GraphControllerFactory::~GraphControllerFactory(void)
{
}

ITestGraphController * GraphControllerFactory::CreateTestGraphController()
{
    TestGraphController * controller = new TestGraphController;
    return static_cast<ITestGraphController*>(controller);
}


