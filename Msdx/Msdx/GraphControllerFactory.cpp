#include "IGraphController.h"
#include "TestGraphController.h"


using namespace msdx;

GraphControllerFactory &GraphControllerFactory::GetInstance()
{
    //使用C++11静态初始化特性保证单例初始化过程的线程安全
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


