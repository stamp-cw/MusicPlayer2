#include "CloudMusic.h"

CloudMusic *CloudMusic::c_player = nullptr;

//加载与更新
CloudMusic* CloudMusic::Create(){
    if(c_player == nullptr){
        MemoryCount::AllocPlayer();
        c_player = new CloudMusic;
        c_player->Init();
    }
    c_player->AddInstantce();
    return c_player;
}
CloudMusic::~CloudMusic(){
    c_player = nullptr;
}
void CloudMusic::Init(){
    m_window = FindWindowEx(0, 0, L"OrpheusBrowserHost", 0);
}
bool CloudMusic::Update(DWORD time){
    // DWORD time = GetTickCount();
    if(time < m_last_time+500){
        return false;
    }
    m_last_time = time;
    
    bool abnormal = false;
    std::wstring LastTrack = m_track;

    //检查窗口
    if (!IsWindow(m_window)){
        m_window = NULL;
        Init();
    }
    if (!m_window){
        abnormal = true;
        m_track.clear();
    }

    //获取信息
    if (!abnormal){
        WCHAR Buffer[102] = {};
        GetWindowText(m_window, Buffer, 100);
        //L"%T - %A   "

        std::wstring data = Buffer;
        // data.resize(data.length() - 3);

        std::wstring::size_type pos;
        pos = data.find(L" - ");

        if (pos == std::wstring::npos){
            abnormal = true;
            m_track = Buffer;
        } else if (LastTrack.compare(data)){
            m_track.assign(data);
            m_title.assign(data, 0, pos);
            m_artist.assign(data.erase(0, pos + 3));
            abnormal = false;
        }
    }

    if (abnormal){
        m_title.clear();
        m_artist.clear();
    }
    return m_track.compare(LastTrack) != 0;
}

//窗口操作
void CloudMusic::OpenPlayer(){
    if (!IsWindow(m_window)){
        ShellExecute(nullptr, L"open", L"CloudMusic.exe", nullptr, nullptr, SW_SHOW);
    } else {
        ShowWindow(m_window, SW_SHOWNORMAL);
        BringWindowToTop(m_window);
    }
}
void CloudMusic::ClosePlayer(){
    if (IsWindow(m_window)){
        PostMessage(m_window, WM_CLOSE, 0, 0);
    }
}