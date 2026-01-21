#ifndef _PREVIEWSCALER_H //guard
#define _PREVIEWSCALER_H
#include <afxwin.h>

typedef signed long int Long;

class PreviewScaler {
public:
	PreviewScaler(CWnd *parent);
	~PreviewScaler();

	void ConvertToPreviewSize();

	CFont* GetLargeFont() const;
	CFont* GetRegularFont() const;
	Long GetLargeRowHeight() const;
	Long GetRegularRowHeight() const;
	Long GetLargeCount() const;
private:
	CWnd* parent;
	CFont* largeFont;
	CFont* regularFont;
	Long regularRowHeight;
	Long largeCount;
};

inline CFont* PreviewScaler::GetLargeFont() const {
	return this->largeFont;
}

inline CFont* PreviewScaler::GetRegularFont() const {
	return this->regularFont;
}

inline Long PreviewScaler::GetLargeRowHeight() const {
	return this->regularRowHeight + 1;
}

inline Long PreviewScaler::GetRegularRowHeight() const {
	return this->regularRowHeight;
}

inline Long PreviewScaler::GetLargeCount() const {
	return this->largeCount;
}

#endif // !_PREVIEWSCALER_H
