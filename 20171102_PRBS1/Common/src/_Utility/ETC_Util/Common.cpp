#include "stdafx.h"
#include "Common.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




//ȣȯ�� ����
unsigned int g_uiLanguageSelect = eLANGUAGE_SELECTED;
CRITICAL_SECTION g_csError;
//______________________

