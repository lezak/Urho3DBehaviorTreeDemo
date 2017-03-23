#include "SequenceNode.h"

namespace UrhoBehaviorTree
{
    SequenceNode::SequenceNode(Context * context, BehaviorTree & tree, bool random, const String& name) : 
        CompositeNode(context, tree, random, name)
    {
    }

    void SequenceNode::OnChildCompleted(BTNodeState state)
    {
        BehaviorTreeNode& child = **currentChild_;
        if (child.GetState() != state || state == NS_ERROR)
        {
            Terminate(NS_ERROR);
            return;
        }
        if (state == NS_FAILURE)
        {
            Terminate(NS_FAILURE);
            URHO3D_LOGDEBUG(name_ + " FAILED");
            return;
        }
        if (state == NS_SUCCESS)
        {
            if (++currentChild_ == children_.End())
            {
                Terminate(NS_SUCCESS);
                URHO3D_LOGDEBUG(name_ + " SUCCED");
            }
            else
            {
                ActivateChild(**currentChild_);
            }
        }
    }
}
