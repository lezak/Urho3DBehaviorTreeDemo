#include "ParallelNode.h"
#include "BehaviorTree.h"

namespace UrhoBehaviorTree
{
    ParallelNode::ParallelNode(Context * context, BehaviorTree & tree, bool random, const String & name) :
        CompositeNode(context, tree, random, name),
        requiredSuccesses_(0),
        successes_(0),
        allowedFails_(0),
        fails_(0)
    {
    }

    void ParallelNode::AddChild(BehaviorTreeNode * node)
    {
        CompositeNode::AddChild(node);
        SetRequiredSuccesses(requiredSuccesses_ + 1);
    }

    void ParallelNode::SetRequiredSuccesses(int value)
    {
        requiredSuccesses_ = value;
        allowedFails_ = children_.Size() - requiredSuccesses_;
    }

    void ParallelNode::OnInitialization()
    {
        //Reset counters
        successes_ = 0;
        fails_ = 0;
        //Activate children from last to first since they are pushed to the front of the active nodes list
        for (auto i = (children_.End() - children_.Begin()) - 1; i >= 0; --i)
        {
            ActivateChild(*children_[i]);
        }
    }

    void ParallelNode::OnChildCompleted(BTNodeState state)
    {
        switch (state)
        {
        case NS_INACTIVE:
            URHO3D_LOGERROR(name_ + " Child completed in inactive state!");
            Terminate(NS_ERROR);
            break;
        case NS_RUNNING:
            break;
        case UrhoBehaviorTree::NS_SUCCESS:
            successes_++;
            if (successes_ >= requiredSuccesses_)
            {
                Terminate(NS_SUCCESS);
            }
            break;
        case UrhoBehaviorTree::NS_FAILURE:
            fails_++;
            if (fails_ > allowedFails_)
            {
                Terminate(NS_FAILURE);
            }
            break;
        case UrhoBehaviorTree::NS_ERROR:
            Terminate(NS_ERROR);
            break;
        default:
            break;
        }
    }
}