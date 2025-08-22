#ifndef _SEARCHRESULT_H //guard
#define _SEARCHRESULT_H

typedef signed long int Long;

class SearchResult {
public:
	SearchResult(Long offset = -1);
	~SearchResult();
	SearchResult(const SearchResult& source);
	SearchResult& operator=(const SearchResult& source);

	SearchResult OffsetBy(Long difference);

	Long GetOffset() const;
private:
	Long offset;
};

inline Long SearchResult::GetOffset() const {
	return this->offset;
}

#endif // !_SEARCHRESULT_H