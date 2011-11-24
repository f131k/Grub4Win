#include "StdAfx.h"
#include "demoredir.h"

CDemoRedirector::CDemoRedirector(void)
{
	m_pWnd = NULL;
}

CDemoRedirector::~CDemoRedirector(void)
{
}

void CDemoRedirector::WriteStdOut(LPCSTR pszOutput)
{
	if (m_pWnd != NULL)
	{
		int nSize = m_pWnd->GetWindowTextLength();
		m_pWnd->SetSel(nSize, nSize);
#if defined(UNICODE) || defined(_UNICODE)
#pragma warning(push)
#pragma warning(disable:4996)
                // Convert to unicode string
                size_t len = mbstowcs(0, pszOutput, 0);
                typedef wchar_t wchar;
                wchar* wszOutput = new wchar[len + 1];
                mbstowcs(wszOutput, pszOutput, len + 1);
#pragma warning(pop)
	        m_pWnd->ReplaceSel(wszOutput);
                // Free
                delete[] wszOutput;
#else        /* UNICODE */
                m_pWnd->ReplaceSel(pszOutput);		// add the message to the end of Edit control
#endif
        }
}

void CDemoRedirector::WriteStdError(LPCSTR pszError)
{
	WriteStdOut("\r\nError message: ");
	WriteStdOut(pszError);
}
