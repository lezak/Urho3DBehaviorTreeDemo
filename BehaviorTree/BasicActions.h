#pragma once

#include "BehaviorTreeNode.h"

namespace UrhoBehaviorTree
{
    class LogAction : public LeafNode
    {
        URHO3D_OBJECT(LogAction, LeafNode);
    public:
        LogAction(Context* context, BehaviorTree& tree, const String& message, const int& logType = LOG_INFO, const String& name = String::EMPTY);
        ~LogAction() {};

        void AddName(bool value) { addName_ = value; }

    protected:
        virtual BTNodeState HandleStep() override;

        String message_;
        int logType_;
        bool addName_;
    };

    class WaitStepsAction : public LeafNode
    {
        URHO3D_OBJECT(WaitStepsAction, LeafNode)
    public:
        WaitStepsAction(Context* context, BehaviorTree& tree, unsigned waitTicks, const String& name = String::EMPTY);
        ~WaitStepsAction() {};

    protected:
        virtual void OnInitialization() override;
        virtual BTNodeState HandleStep() override;

        unsigned waitTicks_;
        unsigned remainingTicks_;
    };
}
