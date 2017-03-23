#pragma once

#include "Example.h"
#include "BehaviorTreeNode.h"

#include <functional>

class Example1 : public Example
{
    URHO3D_OBJECT(Example1, Example);

public:
    Example1(Context* context_);
    ~Example1() {};

    void Init() override;
    void SetUseStep(bool value) { useStep_ = value; }

private:
    //Check source file for explenation how to build a tree
    void BuildTree();
    void HandleInput(StringHash eventType, VariantMap& eventData);

    bool useStep_;
};

//Example of custom actions
using namespace UrhoBehaviorTree;
//Action for checking blackboard
class BlackboardCheckAction : public LeafNode
{
    URHO3D_OBJECT(BlackboardCheckAction, LeafNode);

public:
    BlackboardCheckAction(Context* context, BehaviorTree& tree,const String& run, const String& name = String::EMPTY);
    ~BlackboardCheckAction() {};

protected:
    //Optionally You can use this function to reset variables used by action. It is called when node is initialized (before first handled step)
    //virtual void OnInitialization() override;

    //Most important function in action nodes, You shoud override it with Your custom logic
    BTNodeState HandleStep() override;

    String branch_;
};
//Action for changing log level
class ChangeLogLevelAction : public LeafNode
{
    URHO3D_OBJECT(ChangeLogLevelAction, LeafNode);

public:
    ChangeLogLevelAction(Context* context, BehaviorTree& tree, int level = LOG_DEBUG, const String& name = String::EMPTY);
    ~ChangeLogLevelAction() {};

    //We're going to use initialization to get log subsystem
    void OnInitialization() override;
    
    BTNodeState HandleStep() override;

protected:
    int level_;
    Log* log_;
};
//Action for changing tree step handling
class ToggleStepAction : public LeafNode
{
    URHO3D_OBJECT(ToggleStepAction, LeafNode);
public:
    ToggleStepAction(Context* context, BehaviorTree& tree, bool useStep, const String& name = String::EMPTY);
    ~ToggleStepAction() {};

    BTNodeState HandleStep() override;

    std::function<void(bool)> toggleFunction_;
private:
    bool useStep_;
};


