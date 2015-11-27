//#include <GL/gl.h>
//#include <GL/glu.h>

#include "wx/glcanvas.h"

class PreviewGLCanvas;

class MyFrame : public wxFrame
{
public:
    MyFrame(wxWindow *parent);

    void OnMenuFileOpen(wxCommandEvent& event);
    void OnMenuFileExit(wxCommandEvent& event);
    void OnMenuHelpAbout(wxCommandEvent& event);

//    void SetCanvas(PreviewGLCanvas *canvas) { m_canvas = canvas; }
//    PreviewGLCanvas *GetCanvas() { return m_canvas; }

	~MyFrame()
	{
        // deinitialize the frame manager
		m_mgr.UnInit();
	};
private:
    PreviewGLCanvas *m_canvas;
    wxAuiManager m_mgr;

    wxDECLARE_EVENT_TABLE();
};

class PreviewGLCanvas : public wxGLCanvas
{
public:
    PreviewGLCanvas(wxWindow *parent,
                 wxWindowID id = wxID_ANY,
                 int *gl_attrib = NULL);

    virtual ~PreviewGLCanvas();

    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnChar(wxKeyEvent& event);
    void OnMouseEvent(wxMouseEvent& event);

    void LoadSurface(const wxString& filename);
//    void InitMaterials();
    void InitGL();

private:
    wxGLContext* m_glRC;

	/*
    GLfloat m_verts[MAXVERTS][3];
    GLfloat m_norms[MAXVERTS][3];
    GLint m_numverts;
	*/

    GLfloat m_xrot;
    GLfloat m_yrot;

    wxDECLARE_NO_COPY_CLASS(PreviewGLCanvas);
    wxDECLARE_EVENT_TABLE();
};