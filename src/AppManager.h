#pragma once
#include "Example.h"

#include<Urho3D/Urho3DAll.h>

class AppManager : public Object
{
    URHO3D_OBJECT(AppManager, Object);

public:
    AppManager(Context* context);
    ~AppManager() {};

private:
    void CreateConsoleAndDebugHud();
    void SetInterface();
    void SetMenuText(const String& txt);
    void CloseTextMenu();

    void HandleUpdate(StringHash evetType, VariantMap& eventData);

    void HandleInstructionsBtn(StringHash eventType, VariantMap& eventData);
    void HandleExample1Btn(StringHash eventType, VariantMap& eventData);
    void HandleRunBtn(StringHash eventType, VariantMap& eventData);
    void HandleCloseBtn(StringHash eventType, VariantMap& eventData);

    ResourceCache* cache_;
    Localization* l10n_;
    Input* input_;
    UI* ui_;
    UIElement* uiRoot_;
    Console* console_;

    SharedPtr<XMLFile> defStyle_;
    SharedPtr<Cursor> cursor_;

    SharedPtr<UIElement> mainInterface_;
    SharedPtr<Button> instrBtn_;
    SharedPtr<Button> exaple1Btn_;

    SharedPtr<Window> textPlaceholder_;
    SharedPtr<Text> contentText_;
    SharedPtr<Button> runBtn_;
    SharedPtr<Button> closeBtn_;

    SharedPtr<View3D> view_;

    SharedPtr<Example> example_;

    unsigned exampleToRun_;

    const String mainInterfacePath_ = "UI/MainInterface.xml";
};
