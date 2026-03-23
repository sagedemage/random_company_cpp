#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <map>
#include <ctime>

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
    std::map<std::string, std::vector<std::string>> csv_data;
    
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

std::vector<std::string> split(std::string s, char delimiter) {
    // String split implementation
    std::vector<std::string> array;
    std::string tmp;

    for (int i = 1; i < s.length(); i++) {
        char left_side = '"';
        char right_side = '"';
        if (s[i] == delimiter && s[i-1] == left_side && s[i+1] == right_side) {
            tmp.erase(tmp.begin());
            tmp.erase(tmp.end()-1);
            array.push_back(tmp);
            tmp = "";

        } else {
            tmp += s[i];
        }

        if (i == s.length()-1) {
            tmp.erase(tmp.begin());
            tmp.erase(tmp.end()-1);
            array.push_back(tmp);
        }
    }

    return array;
}

std::string format_array_to_string(std::vector<std::string> array) {
    std::string s = "";

    for (auto i: array) {
        s += i + " ";
    }
    return s;
}

long get_file_size(std::string read_file_path) {
    std::ifstream read_file(read_file_path);
    read_file.seekg(0, std::ios::end);
    long file_size = read_file.tellg();

    read_file.seekg(0, std::ios::beg);
    return file_size;
}

std::map<std::string, std::vector<std::string>> read_csv_file() {
    std::string text;
    std::string read_file_path = "original_data/largest_companies_by_market_cap.csv";
    std::ifstream read_file(read_file_path);

    if (!read_file.is_open()) {
        std::exit(-1);
    }

    long file_size = get_file_size(read_file_path);

    char delimiter = ',';

    std::map<std::string, std::vector<std::string>> csv_data;
    std::vector<std::string> keys;

    int i = 0;
    while (std::getline(read_file, text)) {
        std::vector<std::string> array = split(text, delimiter);

        std::string s = format_array_to_string(array);

        if (i == 0) {
            keys = array;
            for (int j = 0; j < keys.size(); j++) {
                std::vector<std::string> tmp;
                std::string key = keys[j];
                csv_data[key] = tmp;
            }
        } else {
            for (int j = 0; j < keys.size(); j++) {
                // issue
                std::string key = keys[j];
                std::string item = array[j];
                csv_data[key].push_back(item);
            }
        }

        i += 1;
    }

    read_file.close();

    return csv_data;
}

int get_random_number(int end) {
    // Get a diffrent random number each time the program runs
    srand(time(0));

    // Generate a random number between 0 and end-1
    int random_num = rand() % end;

    return random_num;
}

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

    wxSize list_box_size(100, 150);
    list_box = new wxListBox(panel, LIST_BOX_Companies, wxDefaultPosition, list_box_size, 0);

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

    csv_data = read_csv_file();
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
    std::string name_1 = csv_data["Name"][0];
    std::string name_2 = csv_data["Name"][1];
    std::string name_3 = csv_data["Name"][2];
    std::string name_4 = csv_data["Name"][3];
    std::string name_5 = csv_data["Name"][4];
    std::string name_6 = csv_data["Name"][5];
    std::string name_7 = csv_data["Name"][6];
    std::string name_8 = csv_data["Name"][7];
    std::string name_9 = csv_data["Name"][8];
    std::string name_10 = csv_data["Name"][9];
    std::string name_11 = csv_data["Name"][10];
    std::string name_12 = csv_data["Name"][11];

    wxString item_1(name_1.c_str());
    wxString item_2(name_2.c_str());
    wxString item_3(name_3.c_str());
    wxString item_4(name_4.c_str());
    wxString item_5(name_5.c_str());
    wxString item_6(name_6.c_str());
    wxString item_7(name_7.c_str());
    wxString item_8(name_8.c_str());
    wxString item_9(name_9.c_str());
    wxString item_10(name_10.c_str());
    wxString item_11(name_11.c_str());
    wxString item_12(name_12.c_str());

    list_box->Clear();

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

    int end = csv_data["Name"].size();
    int random_num = get_random_number(end);

    wxLogMessage(csv_data["Name"][random_num]);
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