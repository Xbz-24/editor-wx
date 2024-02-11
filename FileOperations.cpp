/**
 * @file app.cpp
 * @brief This class is responsible for initializing the application.
 * @version 0.1
 * @date 2024-01-30
 * @Author Renato German Chavez Chicoma
 */
#include "FileOperations.hpp"

 FileOperations::FileOperations(wxStyledTextCtrl* editor, wxFrame* frame)
    : m_editor(editor), m_frame(frame)
{
    if (!m_editor || !m_frame)
    {
        throw std::runtime_error("Editor or frame is uninitialized in FileOperations");
    }
}
void FileOperations::BindButtonEvents()
{
    m_toolbarComponent->GetButtons()[0]->Bind(wxEVT_BUTTON, &FileOperations::OnSave, this);
    m_toolbarComponent->GetButtons()[1]->Bind(wxEVT_BUTTON, &FileOperations::OnOpen, this);
    m_toolbarComponent->GetButtons()[2]->Bind(wxEVT_BUTTON, &FileOperations::OnNewFile, this);
}
void FileOperations::SaveLastFilePath(const wxString& path)
{
}
wxString FileOperations::LoadLastFilePath()
{
    return "path";
}
void FileOperations::OnSave(wxCommandEvent& event)
{
    if (!m_editor) {
        throw std::runtime_error("Editor is uninitialized");
    }
    wxFileDialog saveFileDialog
    (
    m_frame, _("Save File"), "", "",
    "Text Files (*.txt)|*.txt|All files (*.*)|*.*",
    wxFD_SAVE | wxFD_OVERWRITE_PROMPT
    );
    if(saveFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    if(!m_editor->SaveFile(saveFileDialog.GetPath()))
    {
        wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
    }
    wxFileName fileName(saveFileDialog.GetPath());
    m_frame->SetStatusText(fileName.GetFullName(), 0);
    SaveLastFilePath(saveFileDialog.GetPath());
}
void FileOperations::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog
    (
        m_frame, _("Open file"), "", "",
        "Text Files (*.txt)|*.txt|All files (*.*)|*.*",
        wxFD_OPEN|wxFD_FILE_MUST_EXIST
    );
    if(openFileDialog.ShowModal() == wxID_CANCEL)
    {
        return;
    }
    wxFileInputStream input_stream(openFileDialog.GetPath());
    if(!input_stream.IsOk())
    {
        wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
        return;
    }
    m_editor->LoadFile(openFileDialog.GetPath());
    wxFileName fileName(openFileDialog.GetPath());
    m_frame->SetStatusText(fileName.GetFullName(), 0);
    SaveLastFilePath(openFileDialog.GetPath());
}
void FileOperations::OnNewFile(wxCommandEvent& event)
{
    if (m_editor->GetModify())
    {
        wxMessageDialog confirmDialog
        (
            m_frame, _("Do you want to save changes to the current document?")
            ,_("Confirm"), wxYES_NO | wxCANCEL | wxICON_QUESTION
        );
        int response = confirmDialog.ShowModal();

        if (response == wxID_YES) {
            this->OnSave(event);
        } else if (response == wxID_CANCEL) {
            return;
        }
    }
    m_editor->ClearAll();
    m_editor->EmptyUndoBuffer();
    m_frame->SetTitle("wx-editor - New File");
}