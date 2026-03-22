#include <iostream>
#include <fstream>
#include <stdio.h>
#include <wx/wx.h>
#include <wx/clipbrd.h>

class MyApp : public wxApp
{
public: 
    bool OnInit() override;
};

class MyFrame : public wxFrame
{
public: 
    MyFrame();
    wxButton *generate_button;
    wxButton *copy_clipboard_button;
    wxPanel *panel;
    wxBoxSizer *box_sizer;
    wxStaticText *label;
    wxListBox *list_box;
    
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void Generate(wxCommandEvent& event);
    void CopyToClipboard(wxCommandEvent& event);
};

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

enum {
    ID_Hello = 1,
    BUTTON_Generate = wxID_HIGHEST + 1,
    BUTTON_Copy_Clipboard = wxID_HIGHEST + 2,
    STATIC_TEXT_Label = 2,
    LIST_BOX_Companies = 3
};

int width = 400;
int height = 300;
wxSize fixed_size(width, height);

long frame_style = (wxSYSTEM_MENU | \
    wxMINIMIZE_BOX | \
    wxMAXIMIZE_BOX | \
    wxCLOSE_BOX | \
    wxCAPTION | \
    wxCLIP_CHILDREN);

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Hello World", wxDefaultPosition, fixed_size, frame_style){
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H", "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    panel = new wxPanel(this, wxID_ANY);

    box_sizer = new wxBoxSizer(wxVERTICAL);

    label = new wxStaticText(panel, STATIC_TEXT_Label, _T("Random Companies:"), wxDefaultPosition, wxDefaultSize, 0);

    list_box = new wxListBox(panel, LIST_BOX_Companies, wxDefaultPosition, wxDefaultSize, 0);

    wxString item_1("Word1");
    wxString item_2("Word2");
    wxString item_3("Word3");
    wxString item_4("Word4");
    wxString item_5("Word5");
    wxString item_6("Word6");
    wxString item_7("Word7");
    wxString item_8("Word8");
    wxString item_9("Word9");
    wxString item_10("Word10");
    wxString item_11("Word11");
    wxString item_12("Word12");

    list_box->Append(item_1);
    list_box->Append(item_2);
    list_box->Append(item_3);
    list_box->Append(item_4);
    list_box->Append(item_5);
    list_box->Append(item_6);
    list_box->Append(item_7);
    list_box->Append(item_8);
    list_box->Append(item_9);
    list_box->Append(item_10);
    list_box->Append(item_11);
    list_box->Append(item_12);

    generate_button = new wxButton(panel, BUTTON_Generate, _T("Generate"), wxDefaultPosition, wxDefaultSize, 0);

    copy_clipboard_button = new wxButton(panel, BUTTON_Copy_Clipboard, _T("Copy to Clipboard"), wxDefaultPosition, wxDefaultSize, 0);

    box_sizer->Add(label, 0, wxALIGN_CENTER_HORIZONTAL, 10);
    box_sizer->Add(list_box, 0, wxALIGN_CENTER_HORIZONTAL, 10);
    box_sizer->Add(generate_button, 0, wxALIGN_CENTER_HORIZONTAL, 10);
    box_sizer->Add(copy_clipboard_button, 0, wxALIGN_CENTER_HORIZONTAL, 10);

    panel->SetSizer(box_sizer);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::Generate, this, BUTTON_Generate);
    Bind(wxEVT_BUTTON, &MyFrame::CopyToClipboard, this, BUTTON_Copy_Clipboard);
}

void MyFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event) {
    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::Generate(wxCommandEvent& event) {
    wxLogMessage("Generate");
}

void MyFrame::CopyToClipboard(wxCommandEvent& event) {
    int index = list_box->GetSelection();
    if (index != wxNOT_FOUND) {
        if (wxTheClipboard->Open()) {
            wxString item_label = list_box->GetString(index);
            wxTextDataObject *text = new wxTextDataObject(item_label);
            wxTheClipboard->SetData(text);
            wxTheClipboard->Close();
        }
    }
}

wxIMPLEMENT_APP(MyApp);