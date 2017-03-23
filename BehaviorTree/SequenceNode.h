#pragma once

#include "BehaviorTreeNode.h"

namespace UrhoBehaviorTree
{
    class SequenceNode : public CompositeNode
    {
        URHO3D_OBJECT(SequenceNode, CompositeNode);

    public:
        SequenceNode(Context* context, BehaviorTree& tree, bool random = false, const String& name = String::EMPTY);
        ~SequenceNode() {};

    protected:
        void OnChildCompleted(BTNodeState state) override;
    };
}
