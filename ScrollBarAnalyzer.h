#ifndef _SCROLLBARANALYZER_H //guard
#define _SCROLLBARANALYZER_H

typedef signed long int Long;

class CWnd;

class ScrollBarAnalyzer {
public:
	ScrollBarAnalyzer(CWnd* parent);
	~ScrollBarAnalyzer();

	void Analyze();

	bool GetVScrollNeeded() const;
	bool GetHScrollNeeded() const;
	Long GetContentsHeight() const;
	Long GetCOntentsWidth() const;
private:
	CWnd* parent;
	bool vScrollNeeded;
	bool hScrollNeeded;
	Long contentsHeight;
	Long contentsWidth;
};

inline bool ScrollBarAnalyzer::GetVScrollNeeded() const {
	return this->vScrollNeeded;
}

inline bool ScrollBarAnalyzer::GetHScrollNeeded() const {
	return this->hScrollNeeded;
}

inline Long ScrollBarAnalyzer::GetContentsHeight() const {
	return this->contentsHeight;
}

inline Long ScrollBarAnalyzer::GetCOntentsWidth() const {
	return this->contentsWidth;
}

#endif // !_SCROLLBARANALYZER_H
