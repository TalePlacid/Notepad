#include "PreviewScaler.h"
#include "Paper.h"
#include "PreviewForm.h"
#include "PreviewLayout.h"
#include "PreviewPaginator.h"
#include "NotepadForm.h"
#include "Font.h"
#include "resource.h"

#pragma warning(disable:4996)

PreviewScaler::PreviewScaler(CWnd* parent) {
	this->parent = parent;
    this->font = NULL;
    this->rowHeight = 0;
}

PreviewScaler::~PreviewScaler() {
    if (this->font != NULL)
    {
        delete this->font;
    }
}

void PreviewScaler::ConvertToPreviewSize() {
    //1. 쓰기 영역 픽셀 높이를 읽는다.
    PreviewLayout* previewLayout = ((PreviewForm*)(this->parent))->previewLayout;
    RECT writingArea = previewLayout->GetWritingArea();
    Long writingAreaHeight = writingArea.bottom - writingArea.top;

    //2. 줄 높이를 구한다.
    PreviewPaginator* previewPaginator = ((PreviewForm*)(this->parent))->previewPaginator;
    Long rowCountPerPage = previewPaginator->GetRowCountPerPage();
    this->rowHeight = writingAreaHeight / rowCountPerPage;

    //3. 폰트를 구한다.    
    CFont* originalFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
    if (((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->font != NULL)
    {
        originalFont = ((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->font->GetCFont();
    }

    LOGFONT logFont = {};
    originalFont->GetLogFont(&logFont);
    logFont.lfHeight = -this->rowHeight;
    if (this->font != NULL)
    {
        delete this->font;
        this->font = NULL;
    }

    this->font = new CFont;
    this->font->CreateFontIndirectA(&logFont);

#if 0
    //1. 페이지 설정을 읽는다.
    PageSetting pageSetting = ((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->pageSetting;
    PaperSize paperSize = Paper::GetPaperSize(pageSetting.paperName);
    if (pageSetting.isVertical)
    {
        Long temp = paperSize.width;
        paperSize.width = paperSize.height;
        paperSize.height = paperSize.width;
    }

    //2. 배율을 구한다.
    PreviewLayout* previewLayout = ((PreviewForm*)(this->parent))->previewLayout;
    RECT paperArea = previewLayout->GetPaperArea();
    Long paperAreaHeight = paperArea.bottom - paperArea.top;
    double scale = paperAreaHeight / (paperSize.height * 1.0);

    //3. 폰트를 읽는다.
    CFont* originalFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
    if (((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->font != NULL)
    {
        originalFont = ((NotepadForm*)(((PreviewForm*)(this->parent))->parent))->font->GetCFont();
    }

    LOGFONT logFont = {};
    originalFont->GetLogFont(&logFont);

    Long fontSize = logFont.lfHeight;
    if (logFont.lfHeight < 0)
    {
        fontSize *= -1;
    }

    //4. 폰트를 mm단위계로 변환한다.
    Long dpi = GetDpiForWindow(this->parent->GetSafeHwnd());
    double mmFontSize = fontSize * 25.4 / dpi;

    //5. 미리보기 스케일로 변환다.
    Long previewFontSize = mmFontSize * scale + 0.5;

    //6. 폰트를 생성한다.
    if (this->font != NULL)
    {
        delete this->font;
        this->font = NULL;
    }

    this->font = new CFont;
    logFont.lfHeight = -previewFontSize;
    this->font->CreateFontIndirectA(&logFont);

    //7. 줄 높이를 측정한다.
    CDC* dc = this->parent->GetDC();
    CFont* oldFont = dc->SelectObject(this->font);

    TEXTMETRIC tm = { 0, };
    dc->GetTextMetrics(&tm);
    this->rowHeight = tm.tmHeight;

    dc->SelectObject(oldFont);
    this->parent->ReleaseDC(dc);
#endif
}
