#include "Example1.h"

using namespace UrhoBehaviorTree;

Example1::Example1(Context * context_) : Example(context_),
useStep_(true)
{
}

void Example1::Init()
{
    GetSubsystem<Log>()->SetLevel(LOG_INFO);
    console_->SetFocusOnShow(false);
    console_->SetVisible(true);
    BuildTree();
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Example1, HandleInput));
}

void Example1::BuildTree()
{
    //Create tree
    bt_ = SharedPtr<BehaviorTree>(new BehaviorTree(context_));

    /*Tree is build of BehaviorTree Nodes. Nodes derive from BehaviorTreeNode. Its constructor takes as paramters:
    - Context* context - required by Object class;
    - BehaviorTree& tree - for adding/removing child nodes to/from activeNodes_ Vector (this is handled automatically)
                           also this allows to write/read on 'blackboard' (VariantMap) - just use 'tree_.blackboard';
    - String name - last parameter in BehaviorTreeNode constructor, empty by default. Used mostly for debugging.
    Additionaly composite nodes (sequence, selector, parallel) take bool random as third parametr (false by default). If true children will be randomly shuffled.
    Nodes SharedPtr is held only by its parent(s) (behavior tree in case of root node), so removed node will delete its children (removing root will delete whole tree structure)
    */
    //Create first node - this will be root, so it must be a composite node (sequence, selector or parallel).
    SharedPtr<SelectorNode> rootSelector_(new SelectorNode(context_, *bt_, false, "ROOT_SELECTOR"));

    //First Sequence - it will check if tree is running for the first time and return NS_SUCCESS if yes, this will end execution of the tree since root selector will not go to the next child
    SharedPtr<SequenceNode> firstSequence(new SequenceNode(context_, *bt_, false, "FIRST SEQUENCE"));
    //First node will be our custom action checking if it's first run and return NS_SUCCESS if yes(see its HandleStep function lower)
    SharedPtr<BlackboardCheckAction> checkAction(new BlackboardCheckAction(context_, *bt_, "First", "FIRST BRANCH CHECK"));
    //Second node will be another sequence - it'll be executed only if custom action succeed, so on second run it will stay inactive
    SharedPtr<SequenceNode> secondSequence(new SequenceNode(context_, *bt_, false, "SECOND SEQUENCE"));
    //Lets get some string that will be passed to the log action
    String log = l10n_->Get("E1_LOG1");
    //Create first LogAction. LogActions constructor takes String as third parameter - this will be written to the log - and int (LogLevel) as fourth (by default LOG_INFO)
    SharedPtr<LogAction> log1(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_1"));
    //second log action
    log = l10n_->Get("E1_LOG2");
    SharedPtr<LogAction> log2(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_2"));
    //third log action
    log = l10n_->Get("E1_LOG3");
    SharedPtr<LogAction> log3(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_3"));

    //Composite nodes must have at least one child
    //If composite node is not random, children will be executed in the same order as they were added to parent
    //Populate second sequence
    secondSequence->AddChild(log1);
    secondSequence->AddChild(log2);
    secondSequence->AddChild(log3);
    
    //Oh, one more thing... just to keep it clean. By default log actions will add their name to output message
    log1->AddName(false);
    log2->AddName(false);
    log3->AddName(false);

    //Populate first sequence
    firstSequence->AddChild(checkAction);
    firstSequence->AddChild(secondSequence);
    //Finally add first sequence to the root node
    rootSelector_->AddChild(firstSequence);

    /*OK. At this point, first sequence will succeed and therefore root selector and whole tree will succeed,
    but on the second run our blackboard check action will fail, so root selector will activate next child. 
    If none provided, it will also fail - we don't want that, so lets add some more nodes.
    */

    //First, lets check if it's our second run
    SharedPtr<SequenceNode> thirdSequence(new SequenceNode(context_, *bt_, false, "THIRD_SEQUENCE"));
    //Check blackbard 
    SharedPtr<BlackboardCheckAction> checkAction2(new BlackboardCheckAction(context_, *bt_, "second", "SECOND_BRANCH_CHECK"));
    //Parallel node will acitvate all children in the same time
    //note: step is send by behavior tree only to one node, so even if all are activeted at the same time, they will handle it one after another
    SharedPtr<ParallelNode> para1(new ParallelNode(context_, *bt_, false, "PARALLEL_1"));
    //Now some actions
    log = l10n_->Get("E1_LOG4");
    SharedPtr<LogAction> log4(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_4"));
    log = l10n_->Get("E1_LOG5");
    SharedPtr<LogAction> log5(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_5"));
    log = l10n_->Get("E1_LOG6");
    SharedPtr<LogAction> log6(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_6"));
    //We want to go to the next child of root selector, so we better don't succeed
    //InvertDecorator will change NS_SUCCESS to NS_FAILURE or NS_FAILURE to NS_SUCCESS
    log6->AddDecorator(SharedPtr<InvertDecorator>(new InvertDecorator(context_, *bt_, "INVERTER_1")));
    //Each node can have only one decorator, if You want to use more then that, You should add decorator to decorator. In this case only first of them will be called automatically, to activate next one You must call its Decorate function before returning from first.

    para1->AddChild(log4);
    para1->AddChild(log5);
    para1->AddChild(log6);

    log4->AddName(false);
    log5->AddName(false);
    log6->AddName(false);

    //Populate sequence
    thirdSequence->AddChild(checkAction2);
    thirdSequence->AddChild(para1);
    //and add it to the root
    rootSelector_->AddChild(thirdSequence);

    //Parallel node failed (thanks to decorator) and we'll move to the next child of the root selector - this will be another branch check
    SharedPtr<SequenceNode> seq4(new SequenceNode(context_, *bt_, false, "FOURTH_SEQUENCE"));
    //Check blackboard
    SharedPtr<BlackboardCheckAction>checkAction3(new BlackboardCheckAction(context_, *bt_, "third", "THIRD_BRANCH_CHECK"));
    // if it's first visit in third branch, we'll write some stuff to the log and after that, we'll enable debug logging (disabled in constructor of this example)
    SharedPtr<SequenceNode>seq5(new SequenceNode(context_, *bt_, false, "FIFTH_SEQUENCE"));
    log = l10n_->Get("E1_LOG7");
    SharedPtr<LogAction>log7(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_7"));
    log = l10n_->Get("E1_LOG8");
    SharedPtr<LogAction>log8(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_8"));
    //action that will change log level to debug, it will also return NS_SUCCESS, so the whole tree will succeed. On the next run blackboard check of this branch will fail and we'll be able to move to the next branch
    SharedPtr<ChangeLogLevelAction> logToDebug(new ChangeLogLevelAction(context_, *bt_, LOG_DEBUG, "CHANGE_LOG_TO_DEBUG_ACTION"));
    //same as before...
    seq5->AddChild(log7);
    seq5->AddChild(log8);
    seq5->AddChild(logToDebug);

    log7->AddName(false);
    log8->AddName(false);

    seq4->AddChild(checkAction3);
    seq4->AddChild(seq5);

    rootSelector_->AddChild(seq4);

    //4th branch
    //Branch check - there will be one more branch after this
    SharedPtr<SequenceNode>seq6(new SequenceNode(context_, *bt_, false, "SIXTH_SEQUENCE"));
    SharedPtr<BlackboardCheckAction>checkAction4(new BlackboardCheckAction(context_, *bt_, "fourth", "FOURTH_BRANCH_CHECK"));
    SharedPtr<SequenceNode>seq7(new SequenceNode(context_, *bt_, false, "SEVENTH_SEQUENCE"));
    //Lets disable debug logging for a moment
    SharedPtr<ChangeLogLevelAction>logToInfo(new ChangeLogLevelAction(context_, *bt_, LOG_INFO, "CHANGE_LOG_TO_INFO"));
    log = l10n_->Get("E1_LOG9");
    SharedPtr<LogAction>log9(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_9"));
    log = l10n_->Get("E1_LOG10");
    SharedPtr<LogAction>log10(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_10"));
    //Now lets prepere our ToggleStepAction
    SharedPtr<ToggleStepAction> toggle1(new ToggleStepAction(context_, *bt_, false, "TOGGLE_STEP_1"));
    toggle1->toggleFunction_ = std::bind(&Example1::SetUseStep, this, std::placeholders::_1);

    seq7->AddChild(checkAction4);
    seq7->AddChild(logToInfo);
    seq7->AddChild(log9);
    seq7->AddChild(log10);
    //we can reuse this node because we know that it will be inactive at this moment
    seq7->AddChild(logToDebug);
    seq7->AddChild(toggle1);

    log9->AddName(false);
    log10->AddName(false);

    seq6->AddChild(checkAction4);
    seq6->AddChild(seq7);

    rootSelector_->AddChild(seq7);

    //And final branch - no need to use branch check this time
    SharedPtr<SequenceNode>seq8(new SequenceNode(context_, *bt_, false, "LAST_SEQUENCE"));
    //some actions
    SharedPtr<LogAction>log11(new LogAction(context_, *bt_, "3", LOG_RAW, "LOG_11"));
    SharedPtr<LogAction>log12(new LogAction(context_, *bt_, "2", LOG_RAW, "LOG_12"));
    SharedPtr<LogAction>log13(new LogAction(context_, *bt_, "1", LOG_RAW, "LOG_13"));
    SharedPtr<LogAction>log14(new LogAction(context_, *bt_, "0", LOG_RAW, "LOG_14"));
    log = l10n_->Get("E1_LOG15");
    SharedPtr<LogAction>log15(new LogAction(context_, *bt_, log, LOG_RAW, "LOG_15"));

    seq8->AddChild(log11);
    seq8->AddChild(log12);
    seq8->AddChild(log13);
    seq8->AddChild(log14);
    //lets disable debug logging, once again we can reuse inactive node
    seq8->AddChild(logToInfo);
    seq8->AddChild(log15);

    log11->AddName(false);
    log12->AddName(false);
    log13->AddName(false);
    log14->AddName(false);
    log15->AddName(false);

    rootSelector_->AddChild(seq8);
    //Finally lets add root node to our tree. Root node will be automatically activated
    bt_->SetRoot(rootSelector_);

    /*Behavior tree for first example is ready. It was completly built from 'outside' of the BehaviorTree class. 
    This way may be little incovenient for trees that will be instantiated several times. In this case, it's better to create a
    new class derived from BehaviorTree and place whole construction code in its Build function 
    (it's called from BT constructor, so You can just override constructor and place code there).
    Just remember to activate root node by using SetRoot() or to use root_ = yourRootNode and calling Reset() after that.
    You can also call Reset(false) if You want to keep tree inactive and later call Reset()/Reset(true) to activate it.
    Note that Reset should be called only after tree has finished its execution/before first run. 
    To pause execution use SetActive function.
    Creating new class derived from BT will be shown in second example... at some point in the future.
    */

    //Send some single steps so that the first message is shown straight away
    for (int i = 0; i <= 4; i++)
    {
        bt_->SingleStep();
    }  
}

void Example1::HandleInput(StringHash eventType, VariantMap & eventData)
{
    using namespace KeyDown;
    if (input_->GetKeyPress(KEY_SPACE) && bt_)
    {
        if (useStep_)
        {
            //This will send step only to first active node
            bt_->SingleStep();
        }
        else
        {
            //This will send step to all active nodes, if none of them returns NS_RUNNING tree will finish, otherwise running node(s) will be queued for next process/step
            bt_->Process();
        }
    }
}

//Custom Leaf node
BlackboardCheckAction::BlackboardCheckAction(Context * context, BehaviorTree & tree, const String& run, const String & name) :
    LeafNode(context, tree, name),
    branch_(run)
{
}

BTNodeState BlackboardCheckAction::HandleStep()
{
    if (tree_.blackboard_.Contains(branch_))
    {
        URHO3D_LOGDEBUG(name_ + " FAILED");
        return NS_FAILURE;
    }
    else
    {
        URHO3D_LOGDEBUG(name_ + " SUCCEED");
        tree_.blackboard_[branch_] = true;
        return NS_SUCCESS;
    }
}

ChangeLogLevelAction::ChangeLogLevelAction(Context * context, BehaviorTree & tree, int level, const String & name) :
    LeafNode(context, tree, name),
    level_(level)
{
}

void ChangeLogLevelAction::OnInitialization()
{
    log_ = GetSubsystem<Log>();
}

BTNodeState ChangeLogLevelAction::HandleStep()
{
    //just change log level and return NS_SUCCESS
    log_->SetLevel(level_);
    return NS_SUCCESS;
}

ToggleStepAction::ToggleStepAction(Context * context, BehaviorTree & tree, bool useStep, const String & name) :
    LeafNode(context, tree, name),
    useStep_(useStep)
{
}

BTNodeState ToggleStepAction::HandleStep()
{
    if (toggleFunction_)
    {
        URHO3D_LOGDEBUG(name_ + " CHANGING STEP HENDLING BY THIS EXAMPLE");
        toggleFunction_(useStep_);
        return NS_SUCCESS;
    }
    else
    {
        URHO3D_LOGERROR("Toggle function not set!");
        //NS_ERROR will stop tree execution, this can be changed by setting BehaviorTree::alwaysReset_ to true
        return NS_ERROR;
    }
}
