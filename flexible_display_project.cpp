#include <wx/wx.h>
#include <wx/aui/aui.h>
#include "flexible_display_project.h"
 
#include <gl/GL.h>
#include <gl/GLU.h>


wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MyFrame::OnMenuFileOpen)
    EVT_MENU(wxID_EXIT, MyFrame::OnMenuFileExit)
    EVT_MENU(wxID_HELP, MyFrame::OnMenuHelpAbout)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(wxWindow* parent) : wxFrame(parent, -1, _("wxAUI Test"),
	wxDefaultPosition, wxSize(800, 600),
	wxDEFAULT_FRAME_STYLE),
	m_canvas(NULL)
{
	wxMenu *fileMenu = new wxMenu;
	fileMenu->Append(wxID_OPEN, wxT("&Open..."));
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, wxT("E&xit...\tALT-X"));

	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(wxID_HELP, wxT("&About"));

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(menuBar);

	// notify wxAUI which frame to use
	m_mgr.SetManagedWindow(this);

	// create several text controls
	wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	/*
	wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
	wxDefaultPosition, wxSize(200,150),
	wxNO_BORDER | wxTE_MULTILINE);
	*/
	int *gl_attrib = NULL;
	m_canvas = new PreviewGLCanvas(this, wxID_ANY, gl_attrib);
	m_canvas->InitGL();

	// add the panes to the manager
	m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
	m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
	m_mgr.AddPane(m_canvas, wxCENTER);

	// tell the manager to "commit" all the changes just made
	m_mgr.Update();
}
void MyFrame::OnMenuFileOpen( wxCommandEvent& WXUNUSED(event) )
{
    wxString filename = wxFileSelector(wxT("Choose DXF Model"), wxT(""), wxT(""), wxT(""),
#if wxUSE_ZLIB
        wxT("DXF Drawing (*.dxf;*.dxf.gz)|*.dxf;*.dxf.gz|All files (*.*)|*.*"),
#else
        wxT("DXF Drawing (*.dxf)|*.dxf)|All files (*.*)|*.*"),
#endif
        wxFD_OPEN);
}
void MyFrame::OnMenuFileExit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}
void MyFrame::OnMenuHelpAbout( wxCommandEvent& WXUNUSED(event) )
{
    wxMessageBox(wxT("Mogencelab"));
}

 
// our normal wxApp-derived class, as usual
class MyApp : public wxApp
{
public:
 
    bool OnInit()
    {
        wxFrame* frame = new MyFrame(NULL);
        SetTopWindow(frame);
        frame->Show();
        return true;                    
    }
};
 
DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);

//---------------------------------------------------------------------------
// PreviewGLCanvas
//---------------------------------------------------------------------------
wxBEGIN_EVENT_TABLE(PreviewGLCanvas, wxGLCanvas)
//    EVT_SIZE(PreviewGLCanvas::OnSize)
    EVT_PAINT(PreviewGLCanvas::OnPaint)
    EVT_MOUSE_EVENTS(PreviewGLCanvas::OnMouseEvent)
wxEND_EVENT_TABLE()

PreviewGLCanvas::PreviewGLCanvas(wxWindow *parent,
wxWindowID id,
int* gl_attrib)
: wxGLCanvas(parent, id, gl_attrib)
{
	m_xrot = 0;
	m_yrot = 0;
	//    m_numverts = 0;

	// Explicitly create a new rendering context instance for this canvas.
	m_glRC = new wxGLContext(this);
}
PreviewGLCanvas::~PreviewGLCanvas()
{
    delete m_glRC;
}

void PreviewGLCanvas::OnPaint( wxPaintEvent& WXUNUSED(event) )
{
    // This is a dummy, to avoid an endless succession of paint messages.
    // OnPaint handlers must always create a wxPaintDC.
    wxPaintDC dc(this);

    // This is normally only necessary if there is more than one wxGLCanvas
    // or more than one wxGLContext in the application.
    SetCurrent(*m_glRC);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();
//    glRotatef( 45, 1.0f, 0.0f, 0.0f );
//    glTranslatef(2.0f,0,0.0f);
    glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );
	glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );
	GLUquadricObj *obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_LINE); // GLU_FILL GLU_LINE GLU_SILHOUETTE GLU_POINT
//	gluQuadricNormals(obj, GLU_NONE); //GLU_NONE GLU_FLAT GLU_SMOOTH
//	gluQuadricOrientation(obj, GLU_OUTSIDE);
//	gluQuadricTexture(obj, GL_TRUE);
	gluSphere(obj, 0.0f, 30, 30);//(obj, 1.0, 1, 2, 30, 30);
    glTranslatef(0.0f,0,-2.0f);

	gluQuadricDrawStyle(obj, GLU_LINE); // GLU_FILL GLU_LINE GLU_SILHOUETTE GLU_POINT
    gluCylinder(obj, 1, 1, 4, 30, 30);

	glPopMatrix();
	/*
    glPushMatrix();
    glRotatef( m_yrot, 0.0f, 1.0f, 0.0f );
    glRotatef( m_xrot, 1.0f, 0.0f, 0.0f );

    // draw the surface
    if (g_use_vertex_arrays)
    {
        glDrawArrays( GL_TRIANGLE_STRIP, 0, m_numverts );
    }
    else
    {
        glBegin( GL_TRIANGLE_STRIP );

        for (int i=0;i<m_numverts;i++)
        {
            glNormal3fv( m_norms[i] );
            glVertex3fv( m_verts[i] );
        }

        glEnd();
    }

    glPopMatrix();
	*/
    glFlush(); // Not really necessary: buffer swapping below implies glFlush()

    SwapBuffers();
}
void PreviewGLCanvas::OnMouseEvent(wxMouseEvent& event)
{
    static int dragging = 0;
    static float last_x, last_y;

    // Allow default processing to happen, or else the canvas cannot gain focus
    // (for key events).
    event.Skip();

    if (event.LeftIsDown())
    {
        if (!dragging)
        {
            dragging = 1;
        }
        else
        {
            m_yrot += (event.GetX() - last_x)*1.0;
            m_xrot += (event.GetY() - last_y)*1.0;
            Refresh(false);
        }
        last_x = event.GetX();
        last_y = event.GetY();
    }
    else
    {
        dragging = 0;
    }
}
void PreviewGLCanvas::InitGL()
{
    // Make the new context current (activate it for use) with this canvas.
    SetCurrent(*m_glRC);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

//    InitMaterials();
	glViewport(0, 0, 600, 400);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 25.0 );
	gluPerspective(45 /*view angle*/, 600/400.0, 0.1 /*clip close*/, 200 /*clip far*/);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0.0, 0.0, -10.0 );

	/*
    if (g_use_vertex_arrays)
    {
        glVertexPointer( 3, GL_FLOAT, 0, m_verts );
        glNormalPointer( GL_FLOAT, 0, m_norms );
        glEnable( GL_VERTEX_ARRAY );
        glEnable( GL_NORMAL_ARRAY );
    }
	*/

//    InitMaterials();
//    LoadSurface("isosurf.dat.gz");
}
