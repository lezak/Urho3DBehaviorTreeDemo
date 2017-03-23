#include "BasicDecorators.h"
#include "BehaviorTree.h"

namespace UrhoBehaviorTree
{
    InvertDecorator::InvertDecorator(Context * context, BehaviorTree & tree, const String & name) : 
        DecoratorNode(context, tree, name)
    {
    }
    BTNodeState InvertDecorator::Decorate(BTNodeState state)
    {
        if (state == NS_SUCCESS)
        {
            URHO3D_LOGDEBUG(name_ + " Inverted Success->Failure");
            return NS_FAILURE;
        }
        else if(state == NS_FAILURE)
        {
            URHO3D_LOGDEBUG(name_ + " Inverted Failure->Success");
            return NS_SUCCESS;
        }
        return state;
    }

    LoopDecorator::LoopDecorator(Context * context, BehaviorTree & tree, unsigned desiredLoops, const String & name) :
        DecoratorNode(context, tree, name),
        desiredLoops_(desiredLoops),
        finishedRuns_(0)
    {
    }

    BTNodeState LoopDecorator::Decorate(BTNodeState state)
    {
        if (state == NS_SUCCESS || state == NS_FAILURE)
        {
            if (desiredLoops_ != M_MAX_INT)
            {
                finishedRuns_++;
            }
            if (finishedRuns_ < desiredLoops_)
            {
                URHO3D_LOGDEBUG(name_ + "Loops left: " + String(desiredLoops_ - finishedRuns_));
                return NS_INACTIVE;
            }
            else
            {
                URHO3D_LOGDEBUG(name_ + " End of loops");
                finishedRuns_ = 0;
                return NS_SUCCESS;
            }
        }
        return NS_RUNNING;
    }

    SuccedDecorator::SuccedDecorator(Context * context, BehaviorTree & tree, const String & name) :
        DecoratorNode(context, tree, name)
    {
    }

    BTNodeState SuccedDecorator::Decorate(BTNodeState state)
    {
        return NS_SUCCESS;
    }

    FailerDecorator::FailerDecorator(Context * context, BehaviorTree & tree, const String & name) : 
        DecoratorNode(context, tree, name)
    {
    }

    BTNodeState FailerDecorator::Decorate(BTNodeState state)
    {
        return NS_FAILURE;
    }
}