#pragma once

#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/IO/Log.h>

#include <functional>

using namespace Urho3D;

namespace UrhoBehaviorTree
{
    enum BTNodeState
    {
        NS_INACTIVE,
        NS_RUNNING,
        NS_SUCCESS,
        NS_FAILURE,
        NS_ERROR
    };
    
    typedef std::function<void(BTNodeState)> Observer;

    class BehaviorTree;
    class DecoratorNode;
    
    class BehaviorTreeNode : public Serializable
    {
        URHO3D_OBJECT(BehaviorTreeNode, Serializable);

    public:
        BehaviorTreeNode(Context* context, BehaviorTree& tree, const String& name = String::EMPTY);
        ~BehaviorTreeNode() {};

        BTNodeState OnStep();
        String GetName() const { return name_; }
        BTNodeState GetState() const { return state_; }        
        void SetState(BTNodeState state) { state_ = state; };
        void AddDecorator(SharedPtr<DecoratorNode> decorator) { decorator_ = decorator; }

        Observer observer_;
        
    protected:
        virtual void Initialize();
        virtual void OnInitialization() {};
        virtual BTNodeState HandleStep() = 0;
        virtual void Terminate(BTNodeState state);
        virtual void OnTermination(BTNodeState state) {};

        BehaviorTree& tree_;
        String name_;
        BTNodeState state_;
        SharedPtr<DecoratorNode> decorator_;
    };
    
    class CompositeNode : public BehaviorTreeNode
    {
        URHO3D_OBJECT(CompositeNode, BehaviorTreeNode);
    public:
        CompositeNode(Context* context, BehaviorTree& tree, bool random = false, const String& name = String::EMPTY);
        ~CompositeNode() {};
        virtual void AddChild(BehaviorTreeNode* node);
        void ShuffleChildren();
        unsigned int GetChildrenCount() const { return children_.Size(); }

    protected:
        virtual void OnInitialization() override;
        virtual BTNodeState HandleStep() override;
        virtual void OnChildCompleted(BTNodeState state) = 0;

        void Initialize() override;
        void ActivateChild(BehaviorTreeNode& node);
        bool CanBeActivated();
        void Terminate(BTNodeState state) override;

        Vector<SharedPtr<BehaviorTreeNode>> children_;
        Vector<SharedPtr<BehaviorTreeNode>>::Iterator currentChild_;
        bool isRandom_;
    };    

    class LeafNode : public BehaviorTreeNode
    {
        URHO3D_OBJECT(LeafNode, BehaviorTreeNode);

    public:
        LeafNode(Context* context, BehaviorTree& tree, const String& name = String::EMPTY);
        ~LeafNode() {};

    protected:
        virtual void OnInitialization() override {};
        virtual BTNodeState HandleStep() override { return state_; };
    };

    class DecoratorNode : public LeafNode
    {
        URHO3D_OBJECT(DecoratorNode, LeafNode);

    public:
        DecoratorNode(Context* context, BehaviorTree& tree, const String& name = String::EMPTY);
        ~DecoratorNode() {};

        virtual BTNodeState Decorate(BTNodeState state) = 0;
    };
}