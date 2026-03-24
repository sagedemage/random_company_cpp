#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <wx/wx.h>
#include <wx/clipbrd.h>
#include <map>
#include <algorithm>
#include <random>

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

int window_width = 345;
int window_height = 385;
wxSize window_size(window_width, window_height);

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

    for (int i = 0; i < s.length(); i++) {
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
        s += i + ",";
    }
    return s;
}

std::map<std::string, std::vector<std::string>> read_csv_file() {
    std::string text;
    std::string read_file_path = "original_data/largest_companies_by_market_cap.csv";
    std::ifstream read_file(read_file_path);

    if (!read_file.is_open()) {
        std::exit(-1);
    }

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

int get_random_number(int start, int end) {
    // Truly random seed
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<uint32_t> distribution(start, end);
    int random_num = distribution(rng);

    return random_num;
}

bool item_in_array(int item, std::vector<int> array) {
    auto vec_it = std::find(array.begin(), array.end(), item);

    if (vec_it == array.end()) {
        return false;
    }

    return true;
}

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Random Company", wxDefaultPosition, window_size, frame_style){
    wxFont font = wxFont();
    font.SetFamily(wxFONTFAMILY_SWISS);
    double point_size = 12.0;
    font.SetFractionalPointSize(point_size);
    font.SetStyle(wxFONTSTYLE_NORMAL);

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

    panel->SetFont(font);

    box_sizer = new wxBoxSizer(wxVERTICAL);

    label = new wxStaticText(panel, STATIC_TEXT_Label, _T("Random Companies:"), wxDefaultPosition, wxDefaultSize, 0);

    int list_box_width = 300;
    int list_box_height = 225;
    wxSize list_box_size(list_box_width, list_box_height);
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
    list_box->Clear();

    int end = csv_data["Name"].size();

    std::vector<int> random_nums = {};

    int i = 0;
    while (true) {
        if (i == 12) {
            break;
        }
        int random_num = get_random_number(0, end);

        bool result = item_in_array(random_num, random_nums);

        if (result == false) {
            random_nums.push_back(random_num);
        } else {
            continue;
        }

        i = i + 1;
    }

    for (int random_num : random_nums) {
        std::string name = csv_data["Name"][random_num];
        wxString item(name.c_str());
        list_box->Append(item);
    }
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