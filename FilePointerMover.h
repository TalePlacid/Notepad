#ifndef _FILEPOINTERMOVER_H //guard
#define _FILEPOINTERMOVER_H

typedef signed long int Long;

class PagingBuffer;

class FilePointerMover {
public:
	FilePointerMover(PagingBuffer* pagingBuffer);
	~FilePointerMover();

	Long First();
	Long Previous();
	Long Next();
	Long Last();
	Long Move(Long index);

	Long FirstRow();
	Long PreviousRow();
	Long NextRow();
	Long LastRow();
	Long MoveUpRows(Long count);
	Long MoveDownRows(Long count);
private:
	PagingBuffer* pagingBuffer;
};

#endif // !_FILEPOINTERMOVER_H
