#pragma once

#include "BehaviorTreeNode.h"

namespace UrhoBehaviorTree
{
    
    class InvertDecorator : public DecoratorNode
    {
        URHO3D_OBJECT(InvertDecorator, DecoratorNode);

    public:
        InvertDecorator(Context* context, BehaviorTree& tree, const String& name = String::EMPTY);
        ~InvertDecorator() {};

        BTNodeState Decorate(BTNodeState state) override;
    };

    class LoopDecorator : public DecoratorNode
    {
        URHO3D_OBJECT(LoopDecorator, DecoratorNode);

    public:
        LoopDecorator(Context* context, BehaviorTree& tree, unsigned desiredLoops = 0, const String& name = String::EMPTY);
        ~LoopDecorator() {};

        BTNodeState Decorate(BTNodeState state) override;

    protected:
        unsigned desiredLoops_ = 3;
        unsigned finishedRuns_;
    };

    class SuccedDecorator : public DecoratorNode
    {
        URHO3D_OBJECT(SuccedDecorator, DecoratorNode);

    public:
        SuccedDecorator(Context* context, BehaviorTree& tree, const String& name = String::EMPTY);
        ~SuccedDecorator() {};

        BTNodeState Decorate(BTNodeState state) override;
    };

    class FailerDecorator : public DecoratorNode
    {
        URHO3D_OBJECT(FailerDecorator, DecoratorNode);

    public:
        FailerDecorator(Context* context, BehaviorTree& tree, const String& name = String::EMPTY);
        ~FailerDecorator() {}

        BTNodeState Decorate(BTNodeState state) override;
    };
}
