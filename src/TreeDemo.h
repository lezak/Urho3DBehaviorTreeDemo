#pragma once

#include <Urho3D/Urho3DAll.h>

class TreeDemo : public Application
{
    URHO3D_OBJECT(TreeDemo, Application);

public:
    TreeDemo(Context* context);
    ~TreeDemo() {};

    virtual void Setup();
    virtual void Start();
};

URHO3D_DEFINE_APPLICATION_MAIN(TreeDemo)