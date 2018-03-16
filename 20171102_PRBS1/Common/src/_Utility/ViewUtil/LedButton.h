#pragma once
// LedButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLedButton window

#define DEFAULT_MARGIN	20

class CRGBImageList
{
  public:
    CRGBImageList()
    {
      m_dcMem.CreateCompatibleDC(NULL);
      m_old = NULL;
    }
    virtual ~CRGBImageList()
    {
      if( m_hImage )
      {
        m_dcMem.SelectObject(m_old);
        m_old = NULL;
        m_bmp.Detach();
        ::DeleteObject(m_hImage);
      }
      m_dcMem.DeleteDC();
    }
    
    BOOL Create( UINT idBitmap, UINT nWidth )
    {
      m_hImage = (HBITMAP)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(idBitmap), IMAGE_BITMAP, 0, 0, LR_LOADTRANSPARENT|LR_LOADMAP3DCOLORS );
      if( !m_hImage ) return FALSE;
      m_bmp.Attach(m_hImage);
      m_width = nWidth;
      
      //CSize size = m_bmp.GetBitmapDimension(); //Must be set by SetBitmapDimension: unusable here.
      m_bmp.GetObject(sizeof(BITMAP),&m_bmpInfo);
      m_height = m_bmpInfo.bmHeight;

      m_old = m_dcMem.SelectObject(&m_bmp);
      return TRUE;
    }

	  BOOL Draw(CDC* pDC, int nImage, POINT pt)
    {
      return  pDC->BitBlt( pt.x, pt.y, m_width, m_height, &m_dcMem, nImage*m_width, 0, SRCCOPY );
    }

    CSize GetImageDimension() { CSize s(m_width,m_bmpInfo.bmHeight); return s; }

  protected:
	  CDC  m_dcMem;
	  HBITMAP m_hImage;
	  CBitmap m_bmp;
	  UINT m_width, m_height;
	  CGdiObject *m_old;
	  BITMAP m_bmpInfo;
};


class CLedButton : public CButton
{
  // Construction
  public:
    //ReadOnly does not work: it is ALWAYS read-only.
	  CLedButton( BOOL bReadOnly=TRUE, BOOL bDepressed=FALSE, BOOL bCenterAlign=TRUE ) { Init(bReadOnly,bDepressed,bCenterAlign); }
    virtual ~CLedButton() {}
    void Init( BOOL bReadOnly=TRUE, BOOL bDepressed=FALSE, BOOL bCenterAlign=TRUE );

    //Call "SetImage" once before displaying the control.
    void SetImage(UINT idBmp, UINT nWidthOfOneImage);
    //Does not work// void SetReadOnly(BOOL bReadOnly=TRUE) { m_bReadOnly = bReadOnly; }

	  BOOL Depress( BOOL Down ); //Set button state (TRUE=pushed, FALSE=depressed)
	  BOOL IsDepressed();        //Get button state

    // Overrides
	  // ClassWizard generated virtual function overrides
	  //{{AFX_VIRTUAL(CLedButton)
	  public:
	  virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	  protected:
	  virtual void PreSubclassWindow();
	  //}}AFX_VIRTUAL

	  // Generated message map functions
  protected:
	  int m_nMargin;
	  int m_nRadius;
	  CPoint m_ptCenter;
	  CRect  m_textRect;
	  BOOL   m_bDepressed, m_bCenterAlign, m_bReadOnly; //m_bReadOnly does not work
    
	  CRGBImageList m_imgList;
	  CSize m_rcImage;

	  //{{AFX_MSG(CLedButton)
	//}}AFX_MSG
	//afx_msg BOOL OnClicked();

	  DECLARE_MESSAGE_MAP()
};

