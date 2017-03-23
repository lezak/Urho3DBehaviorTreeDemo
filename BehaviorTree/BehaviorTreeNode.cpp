#include "BehaviorTreeNode.h"
#include "BehaviorTree.h"

#include <algorithm>

namespace UrhoBehaviorTree
{
   BehaviorTreeNode::BehaviorTreeNode(Context * context, BehaviorTree & tree, const String& name) : Serializable(context),
        tree_(tree),
        name_(name),
        state_(NS_INACTIVE)
    {
    }

    BTNodeState BehaviorTreeNode::OnStep()
    {
        if (state_ == NS_INACTIVE)
        {
            Initialize();
        }
        if (state_ == NS_ERROR)
        {
            URHO3D_LOGWARNING(name_ + "is in error state");
            return NS_ERROR;
        }
        state_ = HandleStep();
        if (state_ != NS_RUNNING)
        {
            if (decorator_)
            {
                state_ = decorator_->Decorate(state_);
                if (state_ == NS_INACTIVE)
                {
                    Initialize();
                }
            }
            //don't terminate if decorator changed state to running
            if (state_!= NS_RUNNING)
            {
                Terminate(state_);
            }
        }
        return state_;
    }

    void BehaviorTreeNode::Initialize()
    {
        OnInitialization();
        state_ = NS_RUNNING;
    }

    void BehaviorTreeNode::Terminate(BTNodeState state)
    {
        OnTermination(state);
    }

    //Composite
    CompositeNode::CompositeNode(Context * context, BehaviorTree & tree, bool random, const String& name) : 
        BehaviorTreeNode(context, tree, name),
        isRandom_(random)
    {
    }

    void CompositeNode::AddChild(BehaviorTreeNode* node)
    {
        children_.Push(SharedPtr<BehaviorTreeNode>(node));
    }

    void CompositeNode::ShuffleChildren()
    {
        SetRandomSeed(Rand());
        for (unsigned i = 1; i < children_.Size(); ++i)
        {
            std::swap(children_[i], children_[(Random(0, children_.Size()))]);
        }
    }

    void CompositeNode::OnInitialization()
    {
        URHO3D_LOGDEBUG(name_ + " Initialization called");
        currentChild_ = children_.Begin();
        ActivateChild(**currentChild_);
    }

    BTNodeState CompositeNode::HandleStep()
    {
        URHO3D_LOGDEBUG(name_ + " Handling step");
        if (state_ == NS_ERROR)
        {
            URHO3D_LOGWARNING(name_ + " Handling step in error state");
            return NS_ERROR;
        }
        return NS_RUNNING;
    }

    void CompositeNode::Initialize()
    {
        if (CanBeActivated())
        {
            if (isRandom_)
            {
                ShuffleChildren();
            }
            OnInitialization();
        }
    }

    void CompositeNode::ActivateChild(BehaviorTreeNode & node)
    {
        Observer observer = std::bind(&CompositeNode::OnChildCompleted, this, std::placeholders::_1);
        tree_.ActivateNode(node, &observer);
        URHO3D_LOGDEBUG(name_ + " activated child " + node.GetName());
    }

    bool CompositeNode::CanBeActivated()
    {
        if (children_.Size())
        {
            return true;
        }
        URHO3D_LOGWARNING(name_ + " Composite node has no children!");
        state_ = NS_ERROR;
        return false;
    }

    void CompositeNode::Terminate(BTNodeState state)
    {
        BehaviorTreeNode::Terminate(state);
        URHO3D_LOGDEBUG(name_ + " Termination called");
        tree_.DeactivateNode(*this, state);
        for (auto n : children_)
        {
            tree_.RemoveFromActiveList(*n, state);
            n->SetState(NS_INACTIVE);
        }
    }

    //Leaf node
    LeafNode::LeafNode(Context * context, BehaviorTree & tree, const String & name) :
        BehaviorTreeNode(context, tree, name)
    {
    }

    DecoratorNode::DecoratorNode(Context * context, BehaviorTree & tree, const String & name) :
        LeafNode(context, tree, name)
    {
    }
}
