/*
    encFSGui - encfsgui_settings.cpp
    source file contains code to change app settings

    written by Peter Van Eeckhoutte

*/

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/config.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filefn.h> // wxRemoveFile
#include <wx/stdpaths.h>

#include "encfsgui.h"

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

enum
{
    ID_BTN_CHOOSE_ENCFS,
    ID_BTN_CHOOSE_MOUNT,
    ID_BTN_CHOOSE_UMOUNT,
    ID_CHECK_STARTATLOGIN,
    ID_CHECK_STARTASICON,
    ID_CHECK_UNMOUNT_ON_QUIT,
    ID_CHECK_UPDATES
};

// ----------------------------------------------------------------------------
// Classes
// ----------------------------------------------------------------------------

class frmSettingsDialog : public wxDialog
{
public:
    //ctor
    frmSettingsDialog(wxWindow *parent, 
                      wxString& title, 
                      const wxPoint& pos, 
                      const wxSize& size, 
                      long style);
    void Create();
    void SelectEncFSBinPath(wxCommandEvent &event);
    void SetCurrentEncFSBinPath(wxString &encfsbinpath);
    void SelectMountBinPath(wxCommandEvent &event);
    void SetCurrentMountBinPath(wxString &mountbinpath);
    void SelectUMountBinPath(wxCommandEvent &event);
    void SetCurrentUMountBinPath(wxString &umountbinpath);
    void SaveSettings(wxCommandEvent &event);

private:
    wxString currentEncFSBinPath;
    wxString currentMountBinPath;
    wxString currentUMountBinPath;
    wxDECLARE_EVENT_TABLE();
    wxTextCtrl * m_encfsbin_field;
    wxTextCtrl * m_mountbin_field;
    wxTextCtrl * m_umountbin_field;
    wxCheckBox * m_chkbx_startatlogin;
    wxCheckBox * m_chkbx_startasicon;
    wxCheckBox * m_chkbx_unmount_on_quit;
    wxCheckBox * m_chkbx_prompt_on_quit;
    wxCheckBox * m_chkbx_prompt_on_unmount;
    wxCheckBox * m_chkbx_check_updates;
};


// Events

wxBEGIN_EVENT_TABLE(frmSettingsDialog, wxDialog)
    EVT_BUTTON(ID_BTN_CHOOSE_ENCFS,  frmSettingsDialog::SelectEncFSBinPath)
    EVT_BUTTON(ID_BTN_CHOOSE_MOUNT,  frmSettingsDialog::SelectMountBinPath)
    EVT_BUTTON(ID_BTN_CHOOSE_UMOUNT,  frmSettingsDialog::SelectUMountBinPath)
    EVT_BUTTON(wxID_APPLY, frmSettingsDialog::SaveSettings)
wxEND_EVENT_TABLE()

// dialog constructionr
frmSettingsDialog::frmSettingsDialog(wxWindow *parent, 
                                     wxString& title, 
                                     const wxPoint& pos, 
                                     const wxSize& size, 
                                     long style) : wxDialog(parent, wxID_ANY, title, pos, size, style)
{
    currentEncFSBinPath = getEncFSBinPath();
}

void frmSettingsDialog::SetCurrentEncFSBinPath(wxString &encfsbinpath)
{
    currentEncFSBinPath = encfsbinpath;
    m_encfsbin_field->ChangeValue(currentEncFSBinPath);
}

void frmSettingsDialog::SetCurrentMountBinPath(wxString &mountbinpath)
{
    currentMountBinPath = mountbinpath;
    m_mountbin_field->ChangeValue(currentMountBinPath);
}

void frmSettingsDialog::SetCurrentUMountBinPath(wxString &umountbinpath)
{
    currentUMountBinPath = umountbinpath;
    m_umountbin_field->ChangeValue(currentUMountBinPath);
}


void frmSettingsDialog::SelectEncFSBinPath(wxCommandEvent& WXUNUSED(event))
{

    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetPath(wxT("/Config"));
    wxString currentbin;
    currentbin = getEncFSBinPath();

    wxFileDialog openFileDialog(this, _("Select full path to 'encfs' executable"), currentbin, "",
                       "All files (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString fn = openFileDialog.GetPath();
        
        if ( wxFileName::FileExists(fn) )
        {
            SetCurrentEncFSBinPath(fn);
        }
    }
    openFileDialog.Destroy();
}

void frmSettingsDialog::SelectMountBinPath(wxCommandEvent& WXUNUSED(event))
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetPath(wxT("/Config"));
    wxString currentbin;
    currentbin = pConfig->Read(wxT("mountbinpath"), "/sbin/mount");
    wxFileDialog openFileDialog(this, _("Select full path to 'mount' executable"), currentbin, "",
                       "All files (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString fn = openFileDialog.GetPath();
        
        if ( wxFileName::FileExists(fn) )
        {
            SetCurrentMountBinPath(fn);
        }
    }
    openFileDialog.Destroy();
}


void frmSettingsDialog::SelectUMountBinPath(wxCommandEvent& WXUNUSED(event))
{
    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetPath(wxT("/Config"));
    wxString currentbin;
    currentbin = pConfig->Read(wxT("umountbinpath"), "/sbin/umount");
    wxFileDialog openFileDialog(this, _("Select full path to 'umount' executable"), currentbin, "",
                       "All files (*.*)|*.*", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if (openFileDialog.ShowModal() == wxID_OK)
    {
        wxString fn = openFileDialog.GetPath();
        
        if ( wxFileName::FileExists(fn) )
        {
            SetCurrentUMountBinPath(fn);
        }
    }
    openFileDialog.Destroy();
}


void frmSettingsDialog::SaveSettings(wxCommandEvent& WXUNUSED(event))
{

    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetPath(wxT("/Config"));
    pConfig->Write(wxT("encfsbinpath"), m_encfsbin_field->GetValue());
    pConfig->Write(wxT("mountbinpath"), m_mountbin_field->GetValue());
    pConfig->Write(wxT("umountbinpath"), m_umountbin_field->GetValue());
    pConfig->Write(wxT("startatlogin"), m_chkbx_startatlogin->GetValue());
    pConfig->Write(wxT("startasicon"), m_chkbx_startasicon->GetValue());
    pConfig->Write(wxT("autounmount"), m_chkbx_unmount_on_quit->GetValue());
    pConfig->Write(wxT("nopromptonquit"), m_chkbx_prompt_on_quit->GetValue());
    pConfig->Write(wxT("nopromptonunmount"), m_chkbx_prompt_on_unmount->GetValue());
    pConfig->Write(wxT("checkupdates"), m_chkbx_check_updates->GetValue());
    // to do: remove timer to check for updates, if option was deselected

    pConfig->Flush();

    // set app to run at login if needed
    bool autolaunch = m_chkbx_startatlogin->GetValue();
    // destination for file: ~/Library/LaunchAgents
    wxStandardPathsBase& stdp = wxStandardPaths::Get();
    wxString tdir = stdp.GetUserConfigDir();
    tdir.Replace("Preferences","LaunchAgents");
    wxString launchfile;
    launchfile.Printf(wxT("%s/org.corelan.encfsgui.LaunchAtLogin.plist"), tdir);

    if (autolaunch)
    {
        wxString launchscript = getLaunchAgentContents();
        // check if we are inside /Applications first
        //   /Applications/EncFSGui.app/
        wxString appfolder = "/Applications/EncFSGui.app";
        wxDir * dir = new wxDir(appfolder);

        if (!dir->Exists(appfolder))
        {
            wxString title = "Oops";
            wxString errormsg = "It looks like you have not placed EncFSGui.app\n into /Applications yet. Try again later";
            wxMessageDialog * dlg = new wxMessageDialog(this, errormsg, title, wxOK|wxCENTRE|wxICON_ERROR);
            dlg->ShowModal();
            dlg->Destroy();
            pConfig->Write(wxT("startatlogin"), false);
            pConfig->Flush();
        }
        else
        {
            wxFile * pFile = new wxFile(launchfile, wxFile::write);
            if (pFile->Access(launchfile, wxFile::write))
            {
                pFile->Write(launchscript);
                pFile->Close();
            }
            else
            {
                wxString title = "Oops";
                wxString errormsg;
                errormsg.Printf(wxT("Unable to create LaunchAgent '%s'"), launchfile);
                wxMessageDialog * dlg = new wxMessageDialog(this, errormsg, title, wxOK|wxCENTRE|wxICON_ERROR);
                dlg->ShowModal();
                dlg->Destroy();
                pConfig->Write(wxT("startatlogin"), false);
                pConfig->Flush();
            }
        }
    }
    else
    {
        // try removing the LaunchAgants file, if it exists
        wxRemoveFile(launchfile);
    }

    Close(true);
}

void frmSettingsDialog::Create()
{

    wxConfigBase *pConfig = wxConfigBase::Get();
    pConfig->SetPath(wxT("/Config"));

    // add controls to the settings page
    // put everything into a master Sizer
    wxSizer * const sizerTop = new wxBoxSizer(wxVERTICAL);
    
    // section Global options
    wxSizer * const sizerGlobal = new wxStaticBoxSizer(wxVERTICAL, this, "Application dependencies / binaries");

    // encfspath
    sizerGlobal->Add(new wxStaticText(this, wxID_ANY, "&Full path to 'encfs' executable:"));
    m_encfsbin_field = new wxTextCtrl(this, wxID_ANY, getEncFSBinPath());
    sizerGlobal->Add(m_encfsbin_field, wxSizerFlags().Border(wxLEFT|wxBOTTOM|wxRIGHT, 10).Expand());
    sizerGlobal->Add(new wxButton( this , ID_BTN_CHOOSE_ENCFS, wxT("Choose 'encfs' executable")));

    sizerGlobal->AddSpacer(15);

    // mount binary
    sizerGlobal->Add(new wxStaticText(this, wxID_ANY, "&Full path to 'mount' executable:"));    
    m_mountbin_field = new wxTextCtrl(this, wxID_ANY, pConfig->Read(wxT("mountbinpath"), "/sbin/mount"));
    sizerGlobal->Add(m_mountbin_field, wxSizerFlags().Border(wxLEFT|wxBOTTOM|wxRIGHT, 10).Expand());
    sizerGlobal->Add(new wxButton( this , ID_BTN_CHOOSE_MOUNT, wxT("Choose 'mount' executable")));

    sizerGlobal->AddSpacer(15);

    // umount binary
    sizerGlobal->Add(new wxStaticText(this, wxID_ANY, "&Full path to 'umount' executable:"));   
    m_umountbin_field = new wxTextCtrl(this, wxID_ANY, pConfig->Read(wxT("umountbinpath"), "/sbin/umount"));
    sizerGlobal->Add(m_umountbin_field, wxSizerFlags().Border(wxLEFT|wxBOTTOM|wxRIGHT, 10).Expand());
    sizerGlobal->Add(new wxButton( this , ID_BTN_CHOOSE_UMOUNT, wxT("Choose 'umount' executable")));


    // start at login
    wxSizer * const sizerStartup = new wxStaticBoxSizer(wxVERTICAL, this, "Startup && exit options");
    m_chkbx_startatlogin  = new wxCheckBox(this, ID_CHECK_STARTATLOGIN, "Start encfsgui at login");
    // 0l = disabled by default
    // 1l = enabled by default
    m_chkbx_startatlogin->SetValue(pConfig->Read(wxT("startatlogin"), 0l) != 0);
    sizerStartup->Add(m_chkbx_startatlogin);

    m_chkbx_startasicon  = new wxCheckBox(this, ID_CHECK_STARTASICON, "Start encfsgui as icon in taskbar");
    // 0l = disabled by default
    // 1l = enabled by default
    m_chkbx_startasicon->SetValue(pConfig->Read(wxT("startasicon"), 0l) != 0);
    sizerStartup->Add(m_chkbx_startasicon);


    // unmount when exit
    m_chkbx_unmount_on_quit  = new wxCheckBox(this, ID_CHECK_UNMOUNT_ON_QUIT, "Auto unmount volumes when closing app");
    // 0l = disabled by default
    // 1l = enabled by default
    m_chkbx_unmount_on_quit->SetValue(pConfig->Read(wxT("autounmount"), 0l) != 0);
    sizerStartup->Add(m_chkbx_unmount_on_quit);


    m_chkbx_prompt_on_unmount = new wxCheckBox(this, ID_CHECK_UNMOUNT_ON_QUIT, "Do not prompt for confirmation on unmount");
    m_chkbx_prompt_on_unmount->SetValue(pConfig->Read(wxT("nopromptonunmount"), 0l) != 0);
    sizerStartup->Add(m_chkbx_prompt_on_unmount);

    m_chkbx_prompt_on_quit = new wxCheckBox(this, ID_CHECK_UNMOUNT_ON_QUIT, "Do not ask for confirmation on exit");
    m_chkbx_prompt_on_quit->SetValue(pConfig->Read(wxT("nopromptonquit"), 0l) != 0);
    sizerStartup->Add(m_chkbx_prompt_on_quit);

    m_chkbx_check_updates = new wxCheckBox(this, ID_CHECK_UPDATES, "Automatically check for updates at startup");
    m_chkbx_check_updates->SetValue(pConfig->Read(wxT("checkupdates"), 0l) != 0);
    sizerStartup->Add(m_chkbx_check_updates);


    // glue together
    sizerTop->Add(sizerGlobal, wxSizerFlags(1).Expand().Border());
    sizerTop->Add(sizerStartup, wxSizerFlags(1).Expand().Border());

    // Add "Apply" and "Cancel"
    sizerTop->Add(CreateStdDialogButtonSizer(wxAPPLY | wxCANCEL),
                  wxSizerFlags().Right().Border());

    CentreOnScreen();

    //SetSizerAndFit(sizerTop);
    SetSizer(sizerTop);

}

///
//
//

void openSettings(wxWindow *parent)
{   
    wxSize dlgSettingsSize;
    // make height larger when adding more options
    dlgSettingsSize.Set(400,510);

    long style = wxDEFAULT_DIALOG_STYLE;// | wxRESIZE_BORDER;

    wxString strTitle;
    strTitle.Printf( "EncFSGui Settings");  

    frmSettingsDialog* dlg = new frmSettingsDialog(parent, 
                                                   strTitle, 
                                                   wxDefaultPosition, 
                                                   dlgSettingsSize, 
                                                   style);
    dlg->Create();
    dlg->ShowModal();
}
