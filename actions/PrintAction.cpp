#include <afxwin.h>
#include <afxdlgs.h>
#include "PrintAction.h"
#include "../prints/PageSetting.h"
#include "../prints/PrinterResource.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"
#include "../prints/PreviewPaginator.h"
#include "../prints/PrintRenderer.h"
#include "../SuspendAutoWrap.h"
#include "../PageLoader.h"

#pragma warning(disable:4996)

PrintAction::PrintAction(CWnd* parent)
	:Action(parent) {
}

PrintAction::~PrintAction() {
}

void PrintAction::Perform() {
	//1. 자동개행을 멈추고, 처음으로 이동한다.
	SuspendAutoWrap suspendAutoWrap(this->parent);
	PageLoader::LoadFirst(this->parent);

	//2. 페이징 한다.
	PreviewPaginator previewPaginator(this->parent);
	previewPaginator.Paginate();

	//3. 인쇄 대화상자를 연다.
	CPrintDialog printDialog(FALSE);
	printDialog.m_pd.Flags &= ~PD_NOPAGENUMS; //페이지 범위 설정 제거 기능 플래그 OFF
	printDialog.m_pd.Flags |= PD_NOSELECTION; //선택 영역 프린트 기능 OFF
	printDialog.m_pd.Flags |= PD_USEDEVMODECOPIESANDCOLLATE; // 인쇄 매수 설정 기능 ON
	printDialog.m_pd.nMinPage = 1; //최소 페이지
	printDialog.m_pd.nMaxPage = previewPaginator.GetPageCount(); //최대 페이지
	printDialog.m_pd.nFromPage = 1; //기본 최소 페이지
	printDialog.m_pd.nToPage = previewPaginator.GetPageCount(); //기본 최대 페이지
	printDialog.m_pd.nCopies = 1; // 기본 인쇄 매수
	INT_PTR ret = printDialog.DoModal();

	//4. 인쇄 대화상자에서 확인을 눌렀으면,
	if (ret == IDOK)
	{
		//4.1. 프린터 리소스를 읽는다.
		PageSetting pageSetting = ((NotepadForm*)(this->parent))->pageSetting;
		PrinterResource printerResource(this->parent, &printDialog);
		printerResource.LoadMetrics();

		Long fromPage = 1;
		Long toPage = previewPaginator.GetPageCount();
		if (!printDialog.PrintAll())
		{
			fromPage = printDialog.GetFromPage();
			toPage = printDialog.GetToPage();
		}
		Long copies = printDialog.GetCopies();

		//4.2. 프린터 DC를 연결한다.
		CDC dc;
		dc.Attach(printDialog.CreatePrinterDC());
		dc.m_bPrinting = TRUE;

		//4.3 문서 정보 설정한다.
		DOCINFO di;
		memset(&di, 0, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		CString fileName;
		this->parent->GetWindowText(fileName);
		if (fileName == "메모장 ~제목없음")
		{
			fileName = "제목없음";
		}
		di.lpszDocName = (LPCSTR)fileName;

		//3.4. 인쇄를 시작에 성공했으면,
		int isStarted = dc.StartDoc(&di);
		if (isStarted != SP_ERROR)
		{
			//3.4.1. 한 부씩 인쇄이면,
			CFont* oldFont = dc.SelectObject(printerResource.GetFont());

			PrintRenderer printRenderer(this->parent, &previewPaginator, &printerResource);
			Long i;
			Long previous;
			Long current = previewPaginator.GetCurrent();
			if (printDialog.PrintCollate())
			{
				i = 0;
				while (i < copies)
				{
					while (current < fromPage)
					{
						current = previewPaginator.Next();
					}

					previous = 0;
					while (current <= toPage && current != previous)
					{
						dc.StartPage();
						printRenderer.Render(&dc);
						dc.EndPage();
						previous = current;
						current = previewPaginator.Next();
					}

					current = previewPaginator.First();
					i++;
				}
			}
			else //3.4.2. 한 부씩 인쇄가 아니면,
			{
				while (current < fromPage)
				{
					current = previewPaginator.Next();
				}

				previous = 0;
				while (current <= toPage && current != previous)
				{
					i = 0;
					while (i < copies)
					{
						dc.StartPage();
						printRenderer.Render(&dc);
						dc.EndPage();
						i++;
					}
					previous = current;
					current = previewPaginator.Next();
				}
			}

			dc.SelectObject(oldFont);
			dc.EndDoc();
			dc.Detach();
		}
	}
}