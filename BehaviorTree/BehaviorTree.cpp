#include "BehaviorTree.h"

namespace UrhoBehaviorTree
{
    BehaviorTree::BehaviorTree(Context * context) : Object(context),
        isActive_(false),
        resetOnSuccess_(true),
        resetOnFailure_(true),
        alwaysReset_(false)
    {
        Build();
    }

    void BehaviorTree::Build()
    {
    }

    void BehaviorTree::ActivateNode(BehaviorTreeNode & node, Observer * observer)
    {
        if (node.GetState() == NS_ERROR)
        {
            URHO3D_LOGERROR(node.GetName() + " nodes in error state can't be activated!");
            return;
        }
        if (observer)
        {
            node.observer_ = *observer;
        }
        activeNodes_.Insert(0, WeakPtr<BehaviorTreeNode>(&node));
    }

    void BehaviorTree::DeactivateNode(BehaviorTreeNode & node, BTNodeState state)
    {
        if (state == NS_RUNNING)
        {
            return;
        }
        node.SetState(state);
        if (node.observer_)
        {
            node.observer_(state);
        }
        RemoveFromActiveList(node, state);
    }

    void BehaviorTree::RemoveFromActiveList(BehaviorTreeNode & node, BTNodeState state)
    {
        if (activeNodes_.Contains(WeakPtr<BehaviorTreeNode>(&node)))
        {
            node.SetState(state);
            activeNodes_.Erase(activeNodes_.Find(WeakPtr<BehaviorTreeNode>(&node)));
        }
    }

    void BehaviorTree::Process(float timeStep)
    {
        if (isActive_)
        {
            activeNodes_.Push(nullptr);
            while (Step())
            {
                continue;
            }
        }
    }

    void BehaviorTree::SingleStep(float timeStep)
    {
        if (isActive_)
        {
            if (activeNodes_.Size())
            {
                Step();
            }
            else
            {
                EndTree(NS_SUCCESS);
            }
        }
    }

    void BehaviorTree::Reset(bool activate)
    {
        if (root_)
        {
            activeNodes_.Clear();
            root_->SetState(NS_INACTIVE);
            ActivateNode(*root_);
            isActive_ = activate;
        }
        else
        {
            URHO3D_LOGERROR("Behavior tree has no root");
            isActive_ = false;
        }
    }

    void BehaviorTree::SetRoot(SharedPtr<CompositeNode> node)
    {
        root_ = node;
        Reset();
    }

    bool BehaviorTree::Step()
    {
        BehaviorTreeNode* current = activeNodes_.Front();
        activeNodes_.Erase(0);
        if (!current)
        {
            if (activeNodes_.Size() == 0)
            {
                EndTree(NS_SUCCESS);
            }
            return false;
        }
        OnTreeStep();
        BTNodeState currState = current->OnStep();
        if (currState == NS_ERROR)
        {
            URHO3D_LOGDEBUG("Step called on node in error state");
            EndTree(NS_ERROR);
            return false;
        }
        if (currState != NS_RUNNING)
        {
            if (current->observer_)
            {
                current->observer_(currState);
            }
        }
        else
        {
            activeNodes_.Push(SharedPtr<BehaviorTreeNode>(current));
        }
        return true;
    }

    void BehaviorTree::EndTree(BTNodeState state)
    {
        URHO3D_LOGDEBUG("Tree execution finished");
        isActive_ = false;
        if (alwaysReset_)
        {
            Reset();
        }
        if (state == NS_SUCCESS && resetOnSuccess_)
        {
            Reset();
        }
        else if (state == NS_FAILURE && resetOnFailure_)
        {
            Reset();
        }
        OnTreeFinished(state);
    }
}
