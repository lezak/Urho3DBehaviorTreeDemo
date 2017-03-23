#include "AppManager.h"
#include "Example1/Example1.h"

AppManager::AppManager(Context * context) : Object(context),
exampleToRun_(0)
{
    cache_ = GetSubsystem<ResourceCache>();
    input_ = GetSubsystem<Input>();

    ui_ = GetSubsystem<UI>();
    uiRoot_ = ui_->GetRoot();
    defStyle_ = cache_->GetResource<XMLFile>("UI/DefaultStyle.xml");
    uiRoot_->SetDefaultStyle(defStyle_);

    CreateConsoleAndDebugHud();

    l10n_ = GetSubsystem<Localization>();
    l10n_->LoadJSONFile("BTDemoStrings.json");
    l10n_->LoadJSONFile("Example1Strings.json");

    cursor_ = new Cursor(context_);
    cursor_->SetStyleAuto(defStyle_);
    ui_->SetCursor(cursor_);

    SetInterface();
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(AppManager, HandleUpdate));
}

void AppManager::CreateConsoleAndDebugHud()
{
    console_ = GetSubsystem<Engine>()->CreateConsole();
    console_->SetDefaultStyle(defStyle_);
    DebugHud* dbh = GetSubsystem<Engine>()->CreateDebugHud();
    dbh->SetDefaultStyle(defStyle_);

    console_->SetNumRows(GetSubsystem<Graphics>()->GetHeight() / 19);
    console_->SetNumBufferedRows(2 * console_->GetNumRows());
}

void AppManager::SetInterface()
{
    XMLFile* file = cache_->GetResource<XMLFile>(mainInterfacePath_);
    mainInterface_ = ui_->LoadLayout(file);
    uiRoot_->AddChild(mainInterface_);

    instrBtn_ = mainInterface_->GetChildStaticCast<Button>("InstrBtn", true);
    exaple1Btn_ = mainInterface_->GetChildStaticCast<Button>("Exmpl1BTn", true);

    textPlaceholder_ = mainInterface_->GetChildStaticCast<Window>("TextPlaceholder", true);
    contentText_ = mainInterface_->GetChildStaticCast<Text>("ContentText", true);
    runBtn_ = mainInterface_->GetChildStaticCast<Button>("RunBtn", true);
    closeBtn_ = mainInterface_->GetChildStaticCast<Button>("CloseBtn", true);

    view_ = mainInterface_->GetChildStaticCast<View3D>("Viewport", true);
    SetMenuText("_INSTRUCTIONS");

    SubscribeToEvent(instrBtn_, E_RELEASED, URHO3D_HANDLER(AppManager, HandleInstructionsBtn));
    SubscribeToEvent(exaple1Btn_, E_RELEASED, URHO3D_HANDLER(AppManager, HandleExample1Btn));
    SubscribeToEvent(runBtn_, E_RELEASED, URHO3D_HANDLER(AppManager, HandleRunBtn));
    SubscribeToEvent(closeBtn_, E_RELEASED, URHO3D_HANDLER(AppManager, HandleCloseBtn));
}

void AppManager::SetMenuText(const String & txt)
{
    textPlaceholder_->ResetDeepEnabled();
    textPlaceholder_->SetVisible(true);
    contentText_->SetText(txt);
}

void AppManager::CloseTextMenu()
{
    textPlaceholder_->SetDeepEnabled(true);
    textPlaceholder_->SetVisible(false);
}

void AppManager::HandleUpdate(StringHash evetType, VariantMap & eventData)
{
    using namespace KeyDown;
    if (input_->GetKeyPress(KEY_ESCAPE))
    {
        GetSubsystem<Engine>()->Exit();
    }
    if (input_->GetKeyPress(KEY_F1))
    {
        console_->Toggle();
    }
}

void AppManager::HandleInstructionsBtn(StringHash eventType, VariantMap & eventData)
{
    exampleToRun_ = 0;
    SetMenuText("_INSTRUCTIONS");
}

void AppManager::HandleExample1Btn(StringHash eventType, VariantMap & eventData)
{
    exampleToRun_ = 1;
    SetMenuText("_EXMPL1_DESCR");
}

void AppManager::HandleRunBtn(StringHash eventType, VariantMap & eventData)
{
    switch (exampleToRun_)
    {
    case 0:
        example_ = nullptr;
        return;
    case 1:
        example_ = SharedPtr<Example1>(new Example1(context_));
        break;
    default:
        break;
    }
    CloseTextMenu();
    example_->Init();
}

void AppManager::HandleCloseBtn(StringHash eventType, VariantMap & eventData)
{
    CloseTextMenu();
}
