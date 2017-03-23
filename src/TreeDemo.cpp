#include "TreeDemo.h"
#include "AppManager.h"

TreeDemo::TreeDemo(Context * context) : Application(context)
{
}

void TreeDemo::Setup()
{
#if _DEBUG
    engineParameters_["Fullscreen"] = false;
#endif
    engine_->SetMaxFps(60);
}

void TreeDemo::Start()
{
    context_->RegisterSubsystem(new AppManager(context_));
}

