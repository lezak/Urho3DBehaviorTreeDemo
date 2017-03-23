#include "BasicActions.h"

namespace UrhoBehaviorTree
{
    //Log action
    LogAction::LogAction(Context * context, BehaviorTree & tree, const String & message, const int & logType, const String & name) :
        LeafNode(context, tree, name),
        message_(message),
        logType_(logType),
        addName_(true)
    {
    }
    BTNodeState LogAction::HandleStep()
    {
        String message = message_;
        if (addName_)
        {
            message.Insert(0, String(name_+ " "));
        }
        switch (logType_)
        {
        case(LOG_DEBUG):
            URHO3D_LOGDEBUG(message);
            break;
        case(LOG_WARNING):
            URHO3D_LOGWARNING(message);
            break;
        case(LOG_ERROR):
            URHO3D_LOGERROR(message);
        case(LOG_RAW):
            URHO3D_LOGRAW(message);
            break;
        case(LOG_INFO):
            URHO3D_LOGINFO(message);
            break;
        default:
            break;
        }
        return NS_SUCCESS;
    }

    //Wait for ticks action
    WaitStepsAction::WaitStepsAction(Context * context, BehaviorTree & tree, unsigned waitTicks, const String & name) :
        LeafNode(context, tree, name),
        waitTicks_(waitTicks),
        remainingTicks_(waitTicks)
    {
    }

    void WaitStepsAction::OnInitialization()
    {
        remainingTicks_ = waitTicks_;
    }

    BTNodeState WaitStepsAction::HandleStep()
    {
        if (remainingTicks_ > 0)
        {
            remainingTicks_--;
            return NS_RUNNING;
        }
        else
        {
            return NS_SUCCESS;
        }
    }
}
