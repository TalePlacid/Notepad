#ifndef _FILEPOINTERCALCULATOR_H //guard
#define _FILEPOINTERCALCULATOR_H

typedef signed long int Long;

class PagingBuffer;

class FilePointerCalculator {
public:
	FilePointerCalculator(PagingBuffer* pagingBuffer);
	~FilePointerCalculator();

	Long First(Long offset);
	Long Previous(Long offset);
	Long Next(Long offset);
	Long Last(Long offset);
	Long Move(Long offset, Long index);

	Long FirstRow();
	Long PreviousRow(Long offset);
	Long NextRow(Long offset);
	Long LastRow();
	Long MoveUpRows(Long offset, Long count);
	Long MoveDownRows(Long offset, Long count);

	Long FileEnd();
private:
	PagingBuffer* pagingBuffer;
};

#endif // !_FILEPOINTERCALCULATOR_H
