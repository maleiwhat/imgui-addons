#include <imgui.h>

GLuint myImageTextureId = 0;
GLuint myImageTextureId2 = 0;

#ifndef NO_IMGUITABWINDOW
void TabContentProvider(ImGui::TabWindow::TabLabel* tab,ImGui::TabWindow& parent,void* userPtr) {
    // Users will use tab->userPtr here most of the time
    ImGui::Spacing();ImGui::Separator();
    if (tab) ImGui::Text("Here is the content of tab label: \"%s\"\n",tab->getLabel());
    else {ImGui::Text("EMPTY TAB LABEL DOCKING SPACE.");ImGui::Text("PLEASE DRAG AND DROP TAB LABELS HERE!");}
    ImGui::Separator();ImGui::Spacing();
}
void TabLabelPopupMenuProvider(ImGui::TabWindow::TabLabel* tab,ImGui::TabWindow& parent,void* userPtr) {
    if (ImGui::BeginPopup(ImGui::TabWindow::GetTabLabelPopupMenuName()))   {
        ImGui::PushID(tab->getLabel());
        ImGui::Text("\"%s\"",tab->getLabel());
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::MenuItem("Entry 1");
        ImGui::MenuItem("Entry 2");
        ImGui::MenuItem("Entry 3");
        ImGui::MenuItem("Entry 4");
        ImGui::MenuItem("Entry 5");
        ImGui::PopID();
        ImGui::EndPopup();
    }

}
#endif //NO_IMGUITABWINDOW

void InitGL()	// Mandatory
{
/*
if (!ImGui::LoadStyle("./myimgui.style",ImGui::GetStyle()))   {
    fprintf(stderr,"Warning: \"./myimgui.style\" not present.\n");
}
*/

if (!myImageTextureId) myImageTextureId = ImImpl_LoadTexture("./Tile8x8.png");
if (!myImageTextureId2) myImageTextureId2 = ImImpl_LoadTexture("./myNumbersTexture.png");
}

static ImGui::PanelManager mgr;
static ImVec2 gMainMenuBarSize(0,0);
static bool gShowMainMenuBar = true;
static void ShowExampleMenuFile()
{
    ImGui::MenuItem("(dummy menu)", NULL, false, false);
    if (ImGui::MenuItem("New")) {}
    if (ImGui::MenuItem("Open", "Ctrl+O")) {}
    if (ImGui::BeginMenu("Open Recent"))
    {
        ImGui::MenuItem("fish_hat.c");
        ImGui::MenuItem("fish_hat.inl");
        ImGui::MenuItem("fish_hat.h");
        if (ImGui::BeginMenu("More.."))
        {
            ImGui::MenuItem("Hello");
            ImGui::MenuItem("Sailor");
            if (ImGui::BeginMenu("Recurse.."))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
    if (ImGui::MenuItem("Save As..")) {}
    ImGui::Separator();
    if (ImGui::BeginMenu("Options"))
    {
        static bool enabled = true;
        ImGui::MenuItem("Enabled", "", &enabled);
        ImGui::BeginChild("child", ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text("Scrolling Text %d", i);
        ImGui::EndChild();
        static float f = 0.5f;
        ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
        ImGui::InputFloat("Input", &f, 0.1f);
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Colors"))
    {
        for (int i = 0; i < ImGuiCol_COUNT; i++)
            ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Disabled", false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem("Checked", NULL, true)) {}
    if (ImGui::MenuItem("Quit", "Alt+F4")) {}
}
static void ShowExampleMenuBar(bool isMainMenu=false)
{
    //static bool ids[2];
    //ImGui::PushID((const void*) isMainMenu ? &ids[0] : &ids[1]);
    const bool open = isMainMenu ? ImGui::BeginMainMenuBar() : ImGui::BeginMenuBar();
    if (open)
    {
        if (ImGui::BeginMenu("File"))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X")) {}
            if (ImGui::MenuItem("Copy", "CTRL+C")) {}
            if (ImGui::MenuItem("Paste", "CTRL+V")) {}
            ImGui::Separator();
            static bool booleanProp=true;
            ImGui::MenuItem("Boolean Test", NULL, &booleanProp);
            ImGui::EndMenu();
        }
        if (isMainMenu) {
            gMainMenuBarSize = ImGui::GetWindowSize();
            ImGui::EndMainMenuBar();
        }
        else ImGui::EndMenuBar();
    }
    //ImGui::PopID();
}

void ResizeGL(int w,int h)	// Mandatory
{
    static ImVec2 initialSize(w,h);
    //fprintf(stderr,"ResizeGL(%d,%d); ImGui::DisplaySize(%d,%d);\n",w,h,(int)ImGui::GetIO().DisplaySize.x,(int)ImGui::GetIO().DisplaySize.y);

    mgr.setToolbarsScaling((float)w/initialSize.x,(float)h/initialSize.y);
    // This line is only necessary if we have a global menu bar:
    if (gShowMainMenuBar) mgr.setDisplayPortion(ImVec4(0,gMainMenuBarSize.y,w,h-gMainMenuBarSize.y));

}

static const char* DockedWindowNames[] = {"Solution Explorer","Toolbox","Property Window","Find Window","Output Window","Application Output","Preferences"};
static const char* ToggleWindowNames[] = {"Toggle Window 1","Toggle Window 2","Toggle Window 3","Toggle Window 4"};

static void DrawDockedWindows(ImGui::PanelManagerWindowData& wd)    {
if (!wd.isToggleWindow || ImGui::Begin(wd.name,&wd.closed,wd.pos,-1.f,ImGuiWindowFlags_NoSavedSettings))    {
    // Here we simply draw all the windows (in our case both DockedWindowNames and ToggleWindowNames) without using ImGui::Begin()/ImGui::End():
    if (strcmp(wd.name,DockedWindowNames[0])==0)  {
        // Draw Solution Explorer
        ImGui::Text("%s\n",wd.name);
        static float f;
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        //show_test_window ^= ImGui::Button("Test Window");
        //show_another_window ^= ImGui::Button("Another Window");

        // Calculate and show framerate
        static float ms_per_frame[120] = { 0 };
        static int ms_per_frame_idx = 0;
        static float ms_per_frame_accum = 0.0f;
        ms_per_frame_accum -= ms_per_frame[ms_per_frame_idx];
        ms_per_frame[ms_per_frame_idx] = ImGui::GetIO().DeltaTime * 1000.0f;
        ms_per_frame_accum += ms_per_frame[ms_per_frame_idx];
        ms_per_frame_idx = (ms_per_frame_idx + 1) % 120;
        const float ms_per_frame_avg = ms_per_frame_accum / 120;
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", ms_per_frame_avg, 1000.0f / ms_per_frame_avg);
    }
    else if (strcmp(wd.name,DockedWindowNames[1])==0)    {
        // Draw Toolbox
        ImGui::Text("%s\n",wd.name);
        if (!wd.isToggleWindow) {
            ImGui::PushItemWidth(150);
            ImGui::SliderFloat("Window Size",&wd.length,16,wd.dockPos<ImGui::PanelManager::TOP ? ImGui::GetIO().DisplaySize.y-wd.pos.y : ImGui::GetIO().DisplaySize.x-wd.pos.x);
            ImGui::PopItemWidth();
            ImGui::Separator();
        }
        ImGui::Text("Hello world from window \"%s\"",wd.name);                
    }
    else if (strcmp(wd.name,DockedWindowNames[2])==0)    {
        // Draw Property Window
        ImGui::Text("%s\n",wd.name);
        //ImGui::ShowTestWindow();
        /*
        if (!wd.isToggleWindow) {
            ImGui::PushItemWidth(150);
            ImGui::SliderFloat("Window Size",&wd.length,16,wd.dockPos<ImGui::PanelManager::TOP ? ImGui::GetIO().DisplaySize.y-wd.pos.y : ImGui::GetIO().DisplaySize.x-wd.pos.x);
            ImGui::PopItemWidth();
            ImGui::Separator();
        }
        ImGui::Text("Hello world from window \"%s\"",wd.name);
        */
    }
    else if (strcmp(wd.name,DockedWindowNames[3])==0) {
        // Draw Find Window
        ImGui::Text("%s\n",wd.name);
        if (ImGui::TreeNode("Popup"))
        {
            static bool popup_open = false;
            static int selected_fish = -1;
            const char* fishes[] = { "Bream", "Haddock", "Mackerel", "Pollock", "Tilefish" };
            if (ImGui::Button("Select.."))  {
                popup_open = true;
                //wd.persistFocus=true;   // No way
                //ImGui::SetNextWindowFocus();  // No way
            }
            ImGui::SameLine();
            ImGui::Text("%s",selected_fish == -1 ? "<None>" : fishes[selected_fish]);
            if (popup_open)
            {

                //ImGui::BeginPopup(&popup_open);
                ImGui::OpenPopup("MyAquariumPopup");
                if (ImGui::BeginPopup("MyAquariumPopup"))   {
                    ImGui::Text("Aquarium");
                    ImGui::Separator();
                    for (size_t i = 0; i < sizeof(fishes)/sizeof(fishes[0]); i++)
                    {
                        if (ImGui::Selectable(fishes[i], false))
                        {
                            selected_fish = i;
                            popup_open = false;
                        }
                    }
                    if (popup_open)   // NEW: close menu when mouse goes away
                    {
                        ImVec2 pos = ImGui::GetWindowPos();pos.x-=5;pos.y-=5;
                        ImVec2 size = ImGui::GetWindowSize();size.x+=10;size.y+=10;
                        const ImVec2& mousePos = ImGui::GetIO().MousePos;
                        if (mousePos.x<pos.x || mousePos.y<pos.y || mousePos.x>pos.x+size.x || mousePos.y>pos.y+size.y) popup_open = false;
                    }
                    ImGui::EndPopup();
                }


            }
            ImGui::TreePop();
        }
        if (ImGui::Button("Launch Browser"))    {
            //const char* url = "/home/flix/Downloads2/Column separator_border sizing issue · Issue #170 · ocornut_imgui.html.maff";
            const char* url = "/media/flix/HDMaxi/Video";   // opens a folder
            //const char* url = "/home/flix/Pictures/Selection_003.png";  //opens an image
            ImGui::OpenWithDefaultApplication(url);
        }
        for (int i=0;i<20;i++) ImGui::TextWrapped("This window is being created by the ShowTestWindow() function. Please refer to the code for programming reference.\n\nUser Guide:");
    }
    else if (strcmp(wd.name,DockedWindowNames[4])==0)    {
        // Draw Output Window
        ImGui::Text("%s\n",wd.name);
    }
    else if (strcmp(wd.name,DockedWindowNames[5])==0)    {
        // Draw Application Window
        ImGui::Text("%s\n",wd.name);
    }
    else if (strcmp(wd.name,ToggleWindowNames[0])==0)   {
        // Draw Toggle Window 1
        ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x*0.15f,ImGui::GetIO().DisplaySize.y*0.24f),ImGuiSetCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x*0.25f,ImGui::GetIO().DisplaySize.y*0.24f),ImGuiSetCond_FirstUseEver);

        ImGui::Text("Hello world from toggle window \"%s\"",wd.name);
    }
    else if (wd.isToggleWindow)  //()
    {
        // Draw Toggle Window
        ImGui::SetWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x*0.25f,ImGui::GetIO().DisplaySize.y*0.34f),ImGuiSetCond_FirstUseEver);
        ImGui::SetWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x*0.5f,ImGui::GetIO().DisplaySize.y*0.34f),ImGuiSetCond_FirstUseEver);
        ImGui::Text("Hello world from toggle window \"%s\"",wd.name);
    }
}
if (wd.isToggleWindow) ImGui::End();

}


void DrawGL()	// Mandatory
{

        glClearColor(0.8f, 0.6f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (gShowMainMenuBar) ShowExampleMenuBar(true);

        if (mgr.isEmpty()) {
            // Hp) All the associated windows MUST have an unique name WITHOUT using the '##' chars that ImGui supports
            void* myImageTextureVoid = reinterpret_cast<void*>(myImageTextureId);         // 8x8 tiles
            void* myImageTextureVoid2 = reinterpret_cast<void*>(myImageTextureId2);       // 3x3 tiles
            ImVec2 uv0(0,0),uv1(0,0);int tileNumber=0;

            // LEFT PANE
            {
                ImGui::PanelManager::Pane* pane = mgr.addPane(ImGui::PanelManager::LEFT,"myFirstToolbarLeft##foo");
                if (pane)   {
                    // Here we add the "proper" docked buttons and windows:
                    const ImVec2 buttonSize(24,32);
                    for (int i=0;i<3;i++)   {
                        // Add to left pane the first 3 windows DrawDockedWindows[i], with Toolbuttons with the first 3 images of myImageTextureVoid (8x8 tiles):
                        tileNumber=i;uv0=ImVec2((float)(tileNumber%8)/8.f,(float)(tileNumber/8)/8.f);uv1=ImVec2(uv0.x+1.f/8.f,uv0.y+1.f/8.f);
                        pane->addButtonAndWindow(ImGui::Toolbutton(DockedWindowNames[i],myImageTextureVoid,uv0,uv1,buttonSize),         // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                 ImGui::PanelManagerPaneAssociatedWindow(DockedWindowNames[i],-1,&DrawDockedWindows));  //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                    }
                    pane->addSeparator(48); // Note that a separator "eats" one toolbutton index as if it was a real button

                    // Here we add two "automatic" toggle buttons (i.e. toolbuttons + associated windows): only the last args of Toolbutton change.
                    const ImVec2 toggleButtonSize(24,24);
                    tileNumber=0;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonAndWindow(ImGui::Toolbutton(ToggleWindowNames[0],myImageTextureVoid2,uv0,uv1,toggleButtonSize,true,false),        // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                 ImGui::PanelManagerPaneAssociatedWindow(ToggleWindowNames[0],-1,&DrawDockedWindows));              //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                    tileNumber=1;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonAndWindow(ImGui::Toolbutton(ToggleWindowNames[1],myImageTextureVoid2,uv0,uv1,toggleButtonSize,true,false),        // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                 ImGui::PanelManagerPaneAssociatedWindow(ToggleWindowNames[1],-1,&DrawDockedWindows));              //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                    pane->addSeparator(48); // Note that a separator "eats" one toolbutton index as if it was a real button


                    // Here we add two "manual" toggle buttons (i.e. toolbuttons only):
                    const ImVec2 extraButtonSize(24,24);
                    tileNumber=0;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 1",myImageTextureVoid2,uv0,uv1,extraButtonSize,true,false));
                    tileNumber=1;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 2",myImageTextureVoid2,uv0,uv1,extraButtonSize,true,false));

                    // Optional line that affects the look of the Toolbutton in this pane: NOPE: we'll override them later
                    //pane->setDisplayProperties(ImVec2(0.25f,0.9f),ImVec4(0.85,0.85,0.85,1));

                }
            }
            // RIGHT PANE
            {
                ImGui::PanelManager::Pane* pane = mgr.addPane(ImGui::PanelManager::RIGHT,"myFirstToolbarRight##foo");
                if (pane)   {
                    // Here we use (a part of) the left pane to clone windows (handy since we don't support drag and drop):
                    if (mgr.getPaneLeft()) pane->addClonedPane(*mgr.getPaneLeft(),false,0,2); // note that only the "docked" part of buttons/windows are clonable ("manual" buttons are simply ignored): TO FIX: for now please avoid leaving -1 as the last argument, as this seems to mess up button indices: just explicitely copy NonTogglable-DockButtons yourself.
                    // To clone single buttons (and not the whole pane) please use: pane->addClonedButtonAndWindow(...);
                    // IMPORTANT: Toggle Toolbuttons (and associated windows) can't be cloned and are just skipped if present

                    // here we could add new docked windows as well in the usual way now... but we don't
                    pane->addSeparator(48);   // Note that a separator "eats" one toolbutton index as if it was a real button

                    // Here we add two other "manual" toggle buttons:
                    tileNumber=2;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 3",myImageTextureVoid2,uv0,uv1,ImVec2(24,32),true,false));
                    tileNumber=3;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 4",myImageTextureVoid2,uv0,uv1,ImVec2(24,32),true,false));

                    // Here we add two "manual" normal buttons (actually "normal" buttons are always "manual"):
                    pane->addSeparator(48);   // Note that a separator "eats" one toolbutton index as if it was a real button
                    tileNumber=4;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual normal button 1",myImageTextureVoid2,uv0,uv1,ImVec2(24,32),false,false));
                    tileNumber=5;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 2",myImageTextureVoid2,uv0,uv1,ImVec2(24,32),false,false));

                }
            }
            // BOTTOM PANE
            {
                ImGui::PanelManager::Pane* pane = mgr.addPane(ImGui::PanelManager::BOTTOM,"myFirstToolbarBottom##foo");
                if (pane)   {
                    // Here we add the "proper" docked buttons and windows:
                    const ImVec2 buttonSize(32,32);
                    for (int i=3;i<6;i++)   {
                        // Add to left pane the windows DrawDockedWindows[i] from 3 to 6, with Toolbuttons with the images from 3 to 6 of myImageTextureVoid (8x8 tiles):
                        tileNumber=i;uv0=ImVec2((float)(tileNumber%8)/8.f,(float)(tileNumber/8)/8.f);uv1=ImVec2(uv0.x+1.f/8.f,uv0.y+1.f/8.f);
                        pane->addButtonAndWindow(ImGui::Toolbutton(DockedWindowNames[i],myImageTextureVoid,uv0,uv1,buttonSize),         // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                 ImGui::PanelManagerPaneAssociatedWindow(DockedWindowNames[i],-1,&DrawDockedWindows));  //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                    }
                    pane->addSeparator(64); // Note that a separator "eats" one toolbutton index as if it was a real button

                    // Here we add two "automatic" toggle buttons (i.e. toolbuttons + associated windows): only the last args of Toolbutton change.
                    const ImVec2 toggleButtonSize(32,32);
                    tileNumber=2;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonAndWindow(ImGui::Toolbutton(ToggleWindowNames[2],myImageTextureVoid2,uv0,uv1,toggleButtonSize,true,false),        // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                 ImGui::PanelManagerPaneAssociatedWindow(ToggleWindowNames[2],-1,&DrawDockedWindows));              //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                    tileNumber=3;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonAndWindow(ImGui::Toolbutton(ToggleWindowNames[3],myImageTextureVoid2,uv0,uv1,toggleButtonSize,true,false),        // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                 ImGui::PanelManagerPaneAssociatedWindow(ToggleWindowNames[3],-1,&DrawDockedWindows));              //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                    pane->addSeparator(64); // Note that a separator "eats" one toolbutton index as if it was a real button

                    // Here we add two "manual" toggle buttons:
                    const ImVec2 extraButtonSize(32,32);
                    tileNumber=4;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 4",myImageTextureVoid2,uv0,uv1,extraButtonSize,true,false));
                    tileNumber=5;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                    pane->addButtonOnly(ImGui::Toolbutton("Manual toggle button 5",myImageTextureVoid2,uv0,uv1,extraButtonSize,true,false));

                }
            }
            // TOP PANE
            {
                // Here we create a top pane by cloning the bottom one so that the same windows can be shown in two opposite sides.
                const ImGui::PanelManager::Pane* bottomPane = mgr.getPane(ImGui::PanelManager::BOTTOM);
                if (bottomPane) {
                    ImGui::PanelManager::Pane* pane = mgr.addPane(ImGui::PanelManager::TOP,"myFirstToolbarTop##foo");
                    if (pane)   {
                        // Here we add the "proper" docked buttons and windows:
                        const ImVec2 buttonSize(32,32);
                        for (int i=6;i<7;i++)   {
                            // Add to left pane the windows DrawDockedWindows[i] from 3 to 6, with Toolbuttons with the images from 3 to 6 of myImageTextureVoid (8x8 tiles):
                            tileNumber=i;uv0=ImVec2((float)(tileNumber%8)/8.f,(float)(tileNumber/8)/8.f);uv1=ImVec2(uv0.x+1.f/8.f,uv0.y+1.f/8.f);
                            pane->addButtonAndWindow(ImGui::Toolbutton(DockedWindowNames[i],myImageTextureVoid,uv0,uv1,buttonSize),         // the 1st arg of Toolbutton is only used as a text for the tooltip.
                                                     ImGui::PanelManagerPaneAssociatedWindow(DockedWindowNames[i],-1,&DrawDockedWindows));  //  the 1st arg of PanelManagerPaneAssociatedWindow is the name of the window
                        }
                        pane->addSeparator(64); // Note that a separator "eats" one toolbutton index as if it was a real button

                        const ImVec2 extraButtonSize(32,32);
                        pane->addButtonOnly(ImGui::Toolbutton("Normal Manual Button 1",myImageTextureVoid2,ImVec2(0,0),ImVec2(1.f/3.f,1.f/3.f),extraButtonSize));//,false,false,ImVec4(0,1,0,1)));  // Here we add a free button
                        tileNumber=1;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                        pane->addButtonOnly(ImGui::Toolbutton("Normal Manual Button 2",myImageTextureVoid2,uv0,uv1,extraButtonSize));  // Here we add a free button
                        tileNumber=2;uv0=ImVec2((float)(tileNumber%3)/3.f,(float)(tileNumber/3)/3.f);uv1=ImVec2(uv0.x+1.f/3.f,uv0.y+1.f/3.f);
                        pane->addButtonOnly(ImGui::Toolbutton("Normal Manual Button 3",myImageTextureVoid2,uv0,uv1,extraButtonSize));  // Here we add a free button
                        pane->addSeparator(32);  // Note that a separator "eats" one toolbutton index as if it was a real button

                        // Here we add two manual toggle buttons, but we'll use them later to show/hide menu and show/hide a central window
                        const ImVec2 toggleButtonSize(32,32);
                        tileNumber=51;uv0=ImVec2((float)(tileNumber%8)/8.f,(float)(tileNumber/8)/8.f);uv1=ImVec2(uv0.x+1.f/8.f,uv0.y+1.f/8.f);
                        pane->addButtonOnly(ImGui::Toolbutton("Show/Hide Main Menu Bar",myImageTextureVoid,uv0,uv1,toggleButtonSize,true,gShowMainMenuBar));  // Here we add a manual toggle button and simply bind it to "gShowMainMenuBar"
                        tileNumber=5;uv0=ImVec2((float)(tileNumber%8)/8.f,(float)(tileNumber/8)/8.f);uv1=ImVec2(uv0.x+1.f/8.f,uv0.y+1.f/8.f);
                        pane->addButtonOnly(ImGui::Toolbutton("Show/Hide central window",myImageTextureVoid,uv0,uv1,toggleButtonSize,true,false));  // Here we add a manual toggle button that we'll process later

                    }
                }
            }

            // Optional line that affects the look of all the Toolbuttons in the Panes inserted so far:
            mgr.overrideAllExistingPanesDisplayProperties(ImVec2(0.25f,0.9f),ImVec4(0.85,0.85,0.85,1));


            // These two lines are only necessary to accomodate space for the global menu bar we're using:
            if (gShowMainMenuBar)   {
                const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
                const ImVec4 displayPortion = ImVec4(0,gMainMenuBarSize.y,displaySize.x,displaySize.y-gMainMenuBarSize.y);
                mgr.setDisplayPortion(displayPortion);
            }


        }

        ImGui::PanelManagerPane* pressedPane=NULL;  // Optional
        int pressedPaneButtonIndex = -1;            // Optional
        if (mgr.render(&pressedPane,&pressedPaneButtonIndex))   {
            //const ImVec2& iqp = mgr.getCentralQuadPosition();
            //const ImVec2& iqs = mgr.getCentralQuadSize();
            //fprintf(stderr,"Inner Quad Size changed to {%1.f,%1.f,%1.f,%1.f}\n",iqp.x,iqp.y,iqs.x,iqs.y);
        }

        // (Optional) Some manual feedback to the user:
        if (pressedPane && pressedPaneButtonIndex!=-1)
        {
            static const char* paneNames[]={"LEFT","RIGHT","TOP","BOTTOM"};
            if (!pressedPane->getWindowName(pressedPaneButtonIndex)) {
                ImGui::Toolbutton* pButton = NULL;
                pressedPane->getButtonAndWindow(pressedPaneButtonIndex,&pButton);
                if (pButton->isToggleButton) {
                    printf("Pressed manual toggle button (number: %d on pane: %s)\n",pressedPaneButtonIndex,paneNames[pressedPane->pos]);
                    if (pressedPane->pos==ImGui::PanelManager::TOP && pressedPaneButtonIndex==(int)pressedPane->getSize()-2) {
                        gShowMainMenuBar=!gShowMainMenuBar;
                        const ImVec2 displaySize = ImGui::GetIO().DisplaySize;
                        const ImVec4 displayPortion = ImVec4(0,gMainMenuBarSize.y,displaySize.x,displaySize.y-gMainMenuBarSize.y);
                        mgr.setDisplayPortion(gShowMainMenuBar ? displayPortion : ImVec4(0,0,-1,-1));
                    }
                }
                else printf("Pressed manual button (number: %d on pane: %s)\n",pressedPaneButtonIndex,paneNames[pressedPane->pos]);
                fflush(stdout);
            }
            else {
                ImGui::Toolbutton* pButton = NULL;
                pressedPane->getButtonAndWindow(pressedPaneButtonIndex,&pButton);
                if (pButton->isToggleButton) printf("Pressed toggle button (number: %d on pane: %s)\n",pressedPaneButtonIndex,paneNames[pressedPane->pos]);
                else printf("Pressed dock button (number: %d on pane: %s)\n",pressedPaneButtonIndex,paneNames[pressedPane->pos]);
                fflush(stdout);
            }

        }

        //if (pMustShowCentralWidget && *pMustShowCentralWidget)
        if (mgr.getPaneTop()->isButtonPressed(mgr.getPaneTop()->getSize()-1))   // = last button of the top pane
        {
            const ImVec2& iqs = mgr.getCentralQuadSize();
            if (iqs.x>ImGui::GetStyle().WindowMinSize.x && iqs.y>ImGui::GetStyle().WindowMinSize.y) {
                ImGui::SetNextWindowPos(mgr.getCentralQuadPosition());
                ImGui::SetNextWindowSize(mgr.getCentralQuadSize());
                if (ImGui::Begin("Central Window"))    {
#                   ifndef NO_IMGUITABWINDOW
                    static ImGui::TabWindow tabWindow;
                    if (!tabWindow.isInited()) {
                        ImGui::TabWindow::SetWindowContentDrawerCallback(&TabContentProvider,NULL); // Mandatory
                        ImGui::TabWindow::SetTabLabelPopupMenuDrawerCallback(&TabLabelPopupMenuProvider,NULL);  // Optional (if tou need context-menu)

                        static const char* tabNames[] = {"Render","Layers","Scene","World","Object","Constraints","Modifiers","Data","Material","Texture","Particle","Physics"};
                        static const int numTabs = sizeof(tabNames)/sizeof(tabNames[0]);
                        static const char* tabTooltips[numTabs] = {"Render Tab Tooltip","Layers Tab Tooltip","Scene Tab Tooltip","","Object Tab Tooltip","","","","","Tired to add tooltips...",""};
                        for (int i=0;i<numTabs;i++) {
                            tabWindow.addTabLabel(tabNames[i],NULL,tabTooltips[i]); // see additional args to prevent a tab from (MMB) closing
                        }
                    }
                    tabWindow.render(); // Must be called inside "its" window (and sets isInited() to false)
#                   else // NO_IMGUITABWINDOW
                    ImGui::Text("Example central window");
#                   endif // NO_IMGUITABWINDOW
                }
                ImGui::End();
            }
        }



}
void DestroyGL()    // Mandatory
{
    if (myImageTextureId) {glDeleteTextures(1,&myImageTextureId);myImageTextureId=0;}
    if (myImageTextureId2) {glDeleteTextures(1,&myImageTextureId2);myImageTextureId2=0;}

}



#   define USE_ADVANCED_SETUP


// Application code
#ifndef IMIMPL_USE_WINAPI
int main(int argc, char** argv)
{
#   ifndef USE_ADVANCED_SETUP
    // Basic
    ImImpl_Main(NULL,argc,argv);

#   else //USE_ADVANCED_SETUP
    // Advanced
        static const ImWchar ranges[] =
        {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x20AC, 0x20AC,	// €
            0x2122, 0x2122,	// ™
            0x2196, 0x2196, // ↖
            0x21D6, 0x21D6, // ⇖
            0x2B01, 0x2B01, // ⬁
            0x2B09, 0x2B09, // ⬉
            0x2921, 0x2922, // ⤡ ⤢
            0x263A, 0x263A, // ☺
            0x266A, 0x266A, // ♪
            0, // € ™ ↖ ⇖ ⬁ ⬉ ⤡ ⤢ ☺ ♪
        };
    const float fontSizeInPixels = 18.f;


    // These lines load an embedded font. [However these files are way too big... inside <imgui.cpp> they used a better format storing bytes at groups of 4, so the files are more concise (1/4?) than mine]
    const unsigned char ttfMemory[] =
//#   include "./fonts/DejaVuSerifCondensed-Bold.ttf.stbz.inl"  // stbz.inl files needs a special definition to be defined in "ImImpl_RenderDrawLists.h"
//#   include "./fonts/DroidSerif-Bold.ttf.stbz.inl"
#   include "./fonts/DejaVuSerifCondensed-Bold.ttf.inl"
//#   include "./fonts/DroidSerif-Bold.ttf.inl"

;

    ImImpl_InitParams gImGuiInitParams(
    -1,-1,NULL,                                                         // optional window width, height, title

    NULL,
    //"./fonts/DejaVuSerifCondensed-Bold.ttf",                          // optional custom font from file (main custom font)
    //"./fonts/DroidSerif-Bold.ttf",                                       // optional custom font from file (main custom font)                                                              // optional white spot in font texture (returned by the console if not set)

    //NULL,0,
    &ttfMemory[0],sizeof(ttfMemory)/sizeof(ttfMemory[0]),               // optional custom font from memory (secondary custom font) WARNING (licensing problem): e.g. embedding a GPL font in your code can make your code GPL as well.

    fontSizeInPixels,
    &ranges[0]
    );
    //gImGuiInitParams.gFpsClampInsideImGui = 30.0f;                                 // Optional Max allowed FPS (default -1 => unclamped). Useful for editors and to save GPU and CPU power.
    //gImGuiInitParams.gFpsClampOutsideImGui = 15.0f;                                 // Optional Max allowed FPS (default -1 => unclamped). Useful for editors and to save GPU and CPU power.


    ImImpl_Main(&gImGuiInitParams,argc,argv);
#   endif //USE_ADVANCED_SETUP

	return 0;
}
#else //IMIMPL_USE_WINAPI
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int iCmdShow)   // This branch has made my code less concise (I will consider stripping it)
{
#   ifndef USE_ADVANCED_SETUP
    // Basic
    ImImpl_WinMain(NULL,hInstance,hPrevInstance,lpCmdLine,iCmdShow);
#   else //USE_ADVANCED_SETUP
    // Advanced
    // These lines load an embedded font. [However these files are way too big... inside <imgui.cpp> they used a better format storing bytes at groups of 4, so the files are more concise (1/4?) than mine]
    const unsigned char fntMemory[] =
#   include "./fonts/DejaVuSansCondensedBoldOutlineRGBAbinary18.fnt.inl"
    const unsigned char imgMemory[] =
#   include "./fonts/DejaVuSansCondensedBoldOutlineRGBAbinary18_0.png.inl"

    ImImpl_InitParams gImGuiInitParams(
    -1,-1,NULL,                                                         // optional window width, height, title
    NULL,//"./fonts/DejaVuSansCondensedBoldOutlineRGBAbinary18.fnt",    // optional custom font from file (main custom font)
    NULL,//"./fonts/DejaVuSansCondensedBoldOutlineRGBAbinary18_0.png",  // optional custom font from file (main custom font)
    -1,-1,                                                              // optional white spot in font texture (returned by the console if not set)
    &fntMemory[0],sizeof(fntMemory)/sizeof(fntMemory[0]),               // optional custom font from memory (secondary custom font) WARNING (licensing problem): e.g. embedding a GPL font in your code can make your code GPL as well.
    &imgMemory[0],sizeof(imgMemory)/sizeof(imgMemory[0])                // optional custom font from memory (secondary custom font) WARNING (licensing problem): e.g. embedding a GPL font in your code can make your code GPL as well.
    );
    gImGuiInitParams.gFpsClamp = 10.0f;                                 // Optional Max allowed FPS (default -1 => unclamped). Useful for editors and to save GPU and CPU power.
    ImImpl_WinMain(&gImGuiInitParams,hInstance,hPrevInstance,lpCmdLine,iCmdShow);
#   endif //#   USE_ADVANCED_SETUP

    return 0;
}
#endif //IMIMPL_USE_WINAPI



