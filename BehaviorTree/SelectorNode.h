#pragma once

#include "BehaviorTreeNode.h"

namespace UrhoBehaviorTree
{
    class SelectorNode : public CompositeNode
    {
        URHO3D_OBJECT(SelectorNode, CompositeNode);

    public:
        SelectorNode(Context* context, BehaviorTree& tree, bool random = false, const String& name = String::EMPTY);
        ~SelectorNode() {};

    protected:
        void OnChildCompleted(BTNodeState state) override;
    };
}
