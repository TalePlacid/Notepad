#ifndef _PREVIEWPAGINATOR_H
#define _PREVIEWPAGINATOR_H

typedef signed long int Long;

class CWnd;

class PreviewPaginator {
public:
	PreviewPaginator(CWnd* parent);
	~PreviewPaginator();

	void Paginate();
	Long First();
	Long Previous();
	Long Next();
	Long Last();

	Long GetCurrent() const;
	Long GetPageCount() const;
	Long GetRowCountPerPage() const;
private:
	CWnd* parent;
	Long current;
	Long pageCount;
	Long rowCountPerPage;
};

inline Long PreviewPaginator::GetCurrent() const {
	return this->current;
}

inline Long PreviewPaginator::GetPageCount() const {
	return this->pageCount;
}

inline Long PreviewPaginator::GetRowCountPerPage() const {
	return this->rowCountPerPage;
}

#endif // !_PREVIEWPAGINATOR_H

