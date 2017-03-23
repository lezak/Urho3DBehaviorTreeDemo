#pragma once

#include <Urho3D/Urho3DAll.h>
#include "BehaviorTree.h"
#include "SelectorNode.h"
#include "SequenceNode.h"
#include "ParallelNode.h"
#include "BasicActions.h"
#include "BasicDecorators.h"

class AppManager;

class Example : public Object
{
    URHO3D_OBJECT(Example, Object);

public:
    Example(Context* context);
    ~Example() {};

    virtual void Init() {};

protected:
    Console* console_;
    Input* input_;
    SharedPtr<UrhoBehaviorTree::BehaviorTree> bt_;
    AppManager* manager_;

    Localization* l10n_;
};
