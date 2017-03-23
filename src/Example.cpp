#include "Example.h"
#include "AppManager.h"

Example::Example(Context * context) : Object(context)
{
    manager_ = GetSubsystem<AppManager>();
    console_ = GetSubsystem<Console>();
    l10n_ = GetSubsystem<Localization>();
    input_ = GetSubsystem<Input>();
    Init();
}
