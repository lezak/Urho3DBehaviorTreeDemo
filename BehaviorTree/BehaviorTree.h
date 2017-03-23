#pragma once

#include "BehaviorTreeNode.h"

namespace UrhoBehaviorTree
{
    class BehaviorTree : public Object
    {
        URHO3D_OBJECT(BehaviorTree, Object);

    public:
        BehaviorTree(Context* context);
        ~BehaviorTree() {};

        virtual void Build();

        void ActivateNode(BehaviorTreeNode& node, Observer* observer = nullptr);
        void DeactivateNode(BehaviorTreeNode& node, BTNodeState state);
        void RemoveFromActiveList(BehaviorTreeNode& node, BTNodeState state);
        void Process(float timeStep = 0);
        void SingleStep(float timeStep = 0);
        void Reset(bool activate = true);
        
        bool IsActive() { return isActive_; }
        void SetActive(bool active) { isActive_ = active; }
        void SetRoot(SharedPtr<CompositeNode> node);
        CompositeNode* GetRoot() const { return root_; }
        unsigned GetActiveNodesSize() const { return activeNodes_.Size(); }

        VariantMap blackboard_;

    protected:
        virtual void OnTreeStep() {};
        virtual void OnTreeFinished(BTNodeState state) {};

        bool Step();
        virtual void EndTree(BTNodeState state);

        bool isActive_;
        bool resetOnSuccess_;
        bool resetOnFailure_;
        bool alwaysReset_;

        SharedPtr<CompositeNode> root_;
        Vector<WeakPtr<BehaviorTreeNode>> activeNodes_;
    };
}
