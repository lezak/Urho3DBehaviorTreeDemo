#include "SelectorNode.h"

namespace UrhoBehaviorTree
{
    SelectorNode::SelectorNode(Context * context, BehaviorTree & tree, bool random, const String& name) :
        CompositeNode(context, tree, random, name)
    {
    }

    void SelectorNode::OnChildCompleted(BTNodeState state)
    {
        BehaviorTreeNode& child = **currentChild_;
        if (child.GetState() != state || state == NS_ERROR)
        {
            Terminate(NS_ERROR);
            return;
        }
        if (state == NS_SUCCESS)
        {
            Terminate(NS_SUCCESS);
            URHO3D_LOGDEBUG(name_ + " SUCCED");
            return;
        }
        if (state == NS_FAILURE)
        {
            if (++currentChild_ == children_.End())
            {
                Terminate(NS_FAILURE);
                URHO3D_LOGDEBUG(name_ + " FAILED");
            }
            else
            {
                ActivateChild(**currentChild_);
            }
        }
    }
}