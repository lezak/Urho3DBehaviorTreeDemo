#pragma once

#include "BehaviorTreeNode.h"

namespace UrhoBehaviorTree
{
    class ParallelNode : public CompositeNode
    {
        URHO3D_OBJECT(ParallelNode, CompositeNode);

    public:
        ParallelNode(Context* context, BehaviorTree& tree, bool random = false, const String& name = String::EMPTY);
        ~ParallelNode() {};

        void AddChild(BehaviorTreeNode* node);
        void SetRequiredSuccesses(int value);

    protected:
        void OnInitialization() override;
        void OnChildCompleted(BTNodeState state) override;

        unsigned requiredSuccesses_;
        unsigned successes_;
        unsigned allowedFails_;
        unsigned fails_;
    };
}