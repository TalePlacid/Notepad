#ifndef _PREVIEWSCALER_H //guard
#define _PREVIEWSCALER_H
#include <afxwin.h>

typedef signed long int Long;

class PreviewScaler {
public:
	PreviewScaler(CWnd *parent);
	~PreviewScaler();

	void ConvertToPreviewSize();

	CFont* GetFont() const;
	Long GetRowHeight() const;
private:
	CWnd* parent;
	CFont* font;
	Long rowHeight;
};

inline CFont* PreviewScaler::GetFont() const {
	return this->font;
}

inline Long PreviewScaler::GetRowHeight() const {
	return this->rowHeight;
}

#endif // !_PREVIEWSCALER_H
