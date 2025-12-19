#ifndef _SCROLLBARANALYZER_H //guard
#define _SCROLLBARANALYZER_H

typedef signed long int Long;

class CWnd;

class ScrollBarAnalyzer {
public:
	ScrollBarAnalyzer(CWnd* parent);
	~ScrollBarAnalyzer();

	void AnalyzeWithoutWrap();
	void AnalyzeWithWrap();

	bool GetVScrollNeeded() const;
	bool GetHScrollNeeded() const;
	Long GetClientAreaHeight() const;
	Long GetClientAreaWidth() const;
	Long GetContentsHeight() const;
	Long GetContentsWidth() const;
	Long GetScrollBarThickness() const;
private:
	CWnd* parent;
	bool vScrollNeeded;
	bool hScrollNeeded;
	Long clientAreaHeight;
	Long clientAreaWidth;
	Long contentsHeight;
	Long contentsWidth;
	Long scrollBarThickness;
};

inline bool ScrollBarAnalyzer::GetVScrollNeeded() const {
	return this->vScrollNeeded;
}

inline bool ScrollBarAnalyzer::GetHScrollNeeded() const {
	return this->hScrollNeeded;
}

inline Long ScrollBarAnalyzer::GetClientAreaHeight() const {
	return this->clientAreaHeight;
}

inline Long ScrollBarAnalyzer::GetClientAreaWidth() const {
	return this->clientAreaWidth;
}

inline Long ScrollBarAnalyzer::GetContentsHeight() const {
	return this->contentsHeight;
}

inline Long ScrollBarAnalyzer::GetContentsWidth() const {
	return this->contentsWidth;
}

inline Long ScrollBarAnalyzer::GetScrollBarThickness() const {
	return this->scrollBarThickness;
}

#endif // !_SCROLLBARANALYZER_H
