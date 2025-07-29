#include <afxwin.h>
#include <WinBase.h>
#include <io.h>
#include <fcntl.h>
#include "PagingBuffer.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NoteConverter.h"
#include "SizeCalculator.h"
#include "ByteChecker.h"

#pragma warning(disable:4996)

PagingBuffer::PagingBuffer(CWnd* parent, Long pageSize) {
	this->parent = parent;
	this->pageSize = pageSize;

	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	TCHAR directory[256];
	GetCurrentDirectory(256, directory);
	CString fileName = CString(directory) + CString("\\Note.tmp");

	this->file = fopen((LPCTSTR)fileName, "w+b");
	if (this->file != NULL)
	{
		CString contents(notepadForm->Load(notepadForm->GetPath()));
		fwrite((LPCTSTR)contents, 1, contents.GetLength(), this->file);
		fseek(this->file, 0, SEEK_SET);
	}

	this->isDirty = false;
	this->selectionBeginOffset = -1;
}

PagingBuffer::~PagingBuffer() {
	if (this->file != NULL)
	{
		fclose(this->file);
		TCHAR directory[256];
		GetCurrentDirectory(256, directory);
		CString fileName = CString(directory) + CString("\\Note.tmp");
		remove((LPCTSTR)fileName);
	}
}

void PagingBuffer::Load() {
	//1. ���� ������ ��ġ�� ���Ѵ�.
	Long currentOffset = ftell(this->file);

	//2. ������ �� ���� ���Ѵ�.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
#if 0
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();
	Long loadingRowCount = rowCount * PAGE_MULTIPLE;
#endif
	Long loadingRowCount = PAGE_ROWCOUNT * PAGE_MULTIPLE;

	//3. ������ġ�� ���Ѵ�.
	TCHAR character[2];
	Long newLineNeeded = loadingRowCount / 2 - 1;
	Long newLineCount = 0;
	Long i = currentOffset - 1;
	while (newLineCount < newLineNeeded && i >= 0)
	{
		int ret = fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);

		if (character[0] == '\n')
		{
			newLineCount++;
		}

		i--;
	}

	this->startOffset = 0;
	if (newLineCount >= newLineNeeded)
	{
		this->startOffset = i;
	}
	this->start = this->start.Move(0, 0);

	//4. ���� ��ǥ�� ���Ѵ�.
	newLineNeeded = loadingRowCount;
	newLineCount = 0;
	this->current = this->start;
	i = this->startOffset;
	fseek(this->file, i, SEEK_SET);
	while (i < currentOffset)
	{
		fread(character, 1, 1, this->file);

		if (character[0] == '\r' || character[0] & 0x80)
		{
			i++;
			fread(character + 1, 1, 1, this->file);
		}

		if (character[0] != '\r')
		{
			this->current = this->current.Right();
		}
		else
		{
			this->current = this->current.Down();
			this->current = this->current.Move(this->current.GetRow(), 0);
			newLineCount++;
		}

		i++;
	}

	//5. �� ��ġ�� ���Ѵ�.
	Position previous;
	fseek(this->file, currentOffset, SEEK_SET);
	this->end = this->current;
	size_t flag = fread(character, 1, 1, this->file);
	i = currentOffset + 1;
	Long pageLimit = this->startOffset + this->pageSize;
	while (newLineCount < newLineNeeded && flag > 0 && !feof(this->file) && i < pageLimit)
	{
		if (character[0] == '\r' || character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
			i++;
		}

		if (character[0] != '\r')
		{
			this->end = this->end.Right();
		}
		else
		{
			previous = this->end;
			this->end = this->end.Down();
			this->end = this->end.Move(this->end.GetRow(), 0);
			newLineCount++;
		}

		fread(character, 1, 1, this->file);
		i++;
	}

	if (newLineCount >= newLineNeeded)
	{
		this->endOffset = i - 3;
		this->end = previous;
	}
	else if (i > pageLimit)
	{
		Long j = pageLimit - 1;
		fseek(this->file, j, SEEK_SET);
		fread(character, 1, 1, this->file);
		while (j >= currentOffset && character[0] != '\r')
		{
			j--;
			fseek(this->file, j, SEEK_SET);
			fread(character, 1, 1, this->file);
		}

		this->endOffset = j;
		this->end = previous;
	}
	else if (feof(this->file))
	{
		fseek(this->file, 0, SEEK_END);
		this->endOffset = ftell(this->file);
	}

	//6. �������� �д´�.
	fseek(this->file, this->startOffset, SEEK_SET);
	TCHAR(*contents) = new TCHAR[this->pageSize];
	Long contentsSize = this->endOffset - this->startOffset;
	Long count = fread(contents, 1, contentsSize, this->file);
	contents[count] = '\0';

	//7. ��Ʈ�� ��ȯ�Ѵ�.
	if (((NotepadForm*)(this->parent))->note != NULL)
	{
		delete ((NotepadForm*)(this->parent))->note;
	}

	NoteConverter noteConverter;
	((NotepadForm*)(this->parent))->note = noteConverter.Convert(contents, false);
	this->isDirty = false;

	//8. ��Ʈ���� ���� ��ġ�� �̵��Ѵ�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	//9. ���ý�����ġ�� ǥ�õǾ� ������,
	if (this->selectionBeginOffset >= 0)
	{
		Long j;
		Position selectionBegin;
		//9.1. ���ý�����ġ�� ������ġ���� ���̸�,
		if (this->selectionBeginOffset < currentOffset)
		{
			selectionBegin = this->start;
			//9.1.1. ���ý�����ġ�� ������ġ���� �Ʒ��̸�,
			if (this->selectionBeginOffset > this->startOffset)
			{
				//9.1.1.1. ���ý�����ġ�� ��ǥ�� ���Ѵ�.
				Long i = this->startOffset;
				fseek(this->file, i, SEEK_SET);
				while (i < this->selectionBeginOffset)
				{
					fread(character, 1, 1, this->file);

					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
						i++;
					}

					if (character[0] != '\r')
					{
						selectionBegin = selectionBegin.Right();
					}
					else
					{
						selectionBegin = selectionBegin.Down();
						selectionBegin = selectionBegin.Move(selectionBegin.GetRow(), 0);
					}

					i++;
				}
			}
			
			//9.1.2. ���ý�����ǥ���� ������ǥ���� �����Ѵ�.
			row = note->GetAt(selectionBegin.GetRow());
			i = selectionBegin.GetColumn();
			while (i < row->GetLength())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}

			i = selectionBegin.GetRow() + 1;
			while (i < this->current.GetRow())
			{
				row = note->GetAt(i);
				j = 0;
				while (j < row->GetLength())
				{
					row->GetAt(j)->Select(TRUE);
					j++;
				}
				i++;
			}

			row = note->GetAt(this->current.GetRow());
			i = 0;
			while (i < this->current.GetColumn())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}
		}
		else if (this->selectionBeginOffset > currentOffset) //9.2. ���ý�����ġ�� ������ġ���� �Ʒ��̸�,
		{
			selectionBegin = this->end;
			//9.2.1. ���ý�����ġ�� ����ġ���� ���̸�,
			if (this->selectionBeginOffset < this->endOffset)
			{
				//9.2.1.1. ���ý�����ġ�� ��ǥ�� ���Ѵ�.
				selectionBegin = this->current;
				i = currentOffset;
				fseek(this->file, i, SEEK_SET);
				while (i < selectionBeginOffset)
				{
					fread(character, 1, 1, this->file);
					
					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
						i++;
					}
		
					if (character[0] != '\r')
					{
						selectionBegin = selectionBegin.Right();
					}
					else
					{
						selectionBegin = selectionBegin.Down();
						selectionBegin = selectionBegin.Move(selectionBegin.GetRow(), 0);
					}

					i++;
				}
			}

			//9.2.2. ������ǥ���� ���ý�����ǥ���� �����Ѵ�.
			row = note->GetAt(this->current.GetRow());
			i = this->current.GetColumn();
			while (i < row->GetLength())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}

			i = this->current.GetRow() + 1;
			while (i < selectionBegin.GetRow())
			{
				row = note->GetAt(i);
				j = 0;
				while (j < row->GetLength())
				{
					row->GetAt(j)->Select(TRUE);
					j++;
				}
				i++;
			}

			row = note->GetAt(selectionBegin.GetRow());
			i = 0;
			while (i < selectionBegin.GetColumn())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}
		}
	}

	//10. ���� ��ġ�� �̵��Ѵ�.
	fseek(this->file, currentOffset, SEEK_SET);

	if (contents != NULL)
	{
		delete[] contents;
	}
}

Long PagingBuffer::Add(char(*character)) {
	Long currentOffset = ftell(this->file);

	//1. �� �ӽ������� �����.
	FILE* addedFile = fopen("AddedFile.tmp", "w+b");

	if (addedFile != NULL)
	{
		//2. ���� ũ�⸦ ���Ѵ�.
		Long characterLength = 1;
		ByteChecker byteChecker;
		if (byteChecker.IsLeadByte(character[0]) || character[0] == '\r')
		{
			characterLength = 2;
		}

		//3. �ӽ����Ͽ� ���ڸ� �߰��� ��ü������ �Ű����´�.
		fseek(this->file, 0, SEEK_END);
		Long fileEndOffset = ftell(this->file);
		TCHAR(*contents) = new TCHAR[fileEndOffset + characterLength];

		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, currentOffset, this->file);

		memcpy(contents + currentOffset, character, characterLength);
		
		Long backwardStart = currentOffset + characterLength;
		fseek(this->file, currentOffset, SEEK_SET);
		fread(contents + backwardStart, 1, fileEndOffset - currentOffset, this->file);

		fwrite(contents, 1, fileEndOffset + characterLength - 1, addedFile);

		if (contents != NULL)
		{
			delete[] contents;
		}

		//4. �ӽ����ϰ� ���������� �¹ٲ۴�.
		fclose(addedFile);
		fclose(this->file);
		remove("Note.tmp");
		rename("AddedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		//5. ����¡ ���ۿ��� ��Ʈ���� ���������� �����Ѵ�.
		fseek(this->file, currentOffset + characterLength, SEEK_SET);
		if (character[0] != '\r')
		{
			this->current = this->current.Right();
		}
		else
		{
			this->current = this->current.Down();
			this->current = this->current.Move(this->current.GetRow(), 0);
		}
	}

	return ftell(this->file);
}

Long PagingBuffer::Remove() {
	Long ret = 0;
	Long currentOffset = ftell(this->file);

	//1. �� �ӽ������� �����.
	FILE* removedFile = fopen("RemovedFile.tmp", "w+b");
	if (removedFile != NULL)
	{
		//2. ���� ������ ���̸� ���Ѵ�.
		fseek(this->file, currentOffset - 1, SEEK_SET);
		TCHAR character;
		fread(&character, 1, 1, this->file);

		Long characterLength = 1;
		ByteChecker byteChecker;
		if (!byteChecker.IsASCII(character) || character == '\n')
		{
			characterLength = 2;
		}

		//3. ���� ���ڸ� �����ϰ� ������ �д´�.
		fseek(this->file, 0, SEEK_END);
		Long fileEnd = ftell(this->file);
		TCHAR(*contents) = new TCHAR[fileEnd];

		Long withoutCharacterOffset = currentOffset - characterLength;
		fseek(this->file, 0, SEEK_SET);
		fread(contents, 1, withoutCharacterOffset, this->file);
		fseek(this->file, currentOffset, SEEK_SET);
		fread(contents + withoutCharacterOffset, 1, fileEnd - currentOffset, this->file);
		
		//4. �� ���Ͽ� ������ ����.
		fwrite(contents, 1, fileEnd - characterLength, removedFile);
		
		if (contents != NULL)
		{
			delete[] contents;
		}

		//5. �� ���ϰ� ���������� �¹ٲ۴�.
		fclose(removedFile);
		fclose(this->file);
		remove("Note.tmp");
		rename("RemovedFile.tmp", "Note.tmp");
		this->file = fopen("Note.tmp", "r+b");

		//6. ����¡ ���ۿ��� ��Ʈ���� ���������� �����Ѵ�.
		if (character != '\n')
		{
			this->current = this->current.Left();
		}
		else
		{
			//���� �� ù��° ĭ���� �̵�.
			TCHAR letter[2];
			Long i = currentOffset - 3;
			fseek(this->file, i, SEEK_SET);
			fread(letter, 1, 1, this->file);
			while (i >= 0 && letter[0] != '\n')
			{
				i--;
				fseek(this->file, i, SEEK_SET);
				fread(letter, 1, 1, this->file);
			}

			if (i >= 0)
			{
				i++;
				this->current = this->current.Up();
				this->current = this->current.Move(this->current.GetRow(), 0);
			}
			else
			{
				i = 0;
				this->current = this->current.Move(0, 0);
			}
			fseek(this->file, i, SEEK_SET);

			//���� ĭ���� ����.
			while (i < currentOffset - characterLength)
			{
				fread(letter, 1, 1, this->file);
				if (byteChecker.IsLeadByte(letter[0]))
				{
					fread(letter + 1, 1, 1, this->file);
					i++;
				}

				this->current = this->current.Right();
				i++;
			}
		}
		fseek(this->file, currentOffset - characterLength, SEEK_SET);
		ret = -1;
	}

	return ret;
}

Long PagingBuffer::CountRow(Long offset) {
	Long current = ftell(this->file);

	Long count = 0;
	TCHAR character[2];
	fseek(this->file, 0, SEEK_SET);
	Long i = 0;
	size_t flag = fread(character, 1, 1, this->file);
	while (i < offset && flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r')
		{
			count++;
		}

		i++;
		size_t flag = fread(character, 1, 1, this->file);
	}

	fseek(this->file, current, SEEK_SET);

	return count;
}

bool PagingBuffer::IsAboveBottomLine() {
	bool ret = false;

	Long currentOffset = ftell(this->file);
	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);
	fseek(this->file, currentOffset, SEEK_SET);

	Long bottomLine = this->end.GetRow() / 10 * 8;
	if (this->current.GetRow() < bottomLine || this->endOffset >= fileEnd)
	{
		ret = true;
	}

	return ret;
}

bool PagingBuffer::IsBelowTopLine() {
	bool ret = false;

	Long topLine = this->end.GetRow() / 10 * 4;
	if (this->current.GetRow() > topLine || this->startOffset <= 0)
	{
		ret = true;
	}

	return ret;
}

Position& PagingBuffer::First() {
	Long currentOffset = ftell(this->file);

	TCHAR character;
	Long i = currentOffset - 1;
	int result = fseek(this->file, i, SEEK_SET);
	fread(&character, 1, 1, this->file);
	while (i >= 0 && character != '\n' && result == 0)
	{
		i--;
		result = fseek(this->file, i, SEEK_SET);
		fread(&character, 1, 1, this->file);
	}

	this->current = this->current.Move(this->current.GetRow(), 0);

	return this->current;
}

Position& PagingBuffer::Previous() {
	Long currentOffset = ftell(this->file);

	if (currentOffset > 0)
	{
		fseek(this->file, currentOffset - 1, SEEK_SET);
		TCHAR character;
		fread(&character, 1, 1, this->file);

		ByteChecker byteChecker;
		if (!byteChecker.IsASCII(character))
		{
			fseek(this->file, currentOffset - 2, SEEK_SET);
			this->current = this->current.Left();
		}
		else
		{
			if (character != '\n')
			{
				fseek(this->file, currentOffset - 1, SEEK_SET);
				this->current = this->current.Left();
			}
			else
			{
				fseek(this->file, currentOffset, SEEK_SET);
			}
		}
	}

	return this->current;
}

Position& PagingBuffer::Next() {
	Long currentOffset = ftell(this->file);

	TCHAR character[2];
	size_t flag = fread(character, 1, 1, this->file);
	if (flag > 0 && !feof(this->file))
	{
		if (character[0] == '\r' || character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		if (character[0] != '\r')
		{
			this->current = this->current.Right();
		}
		else
		{
			fseek(this->file, currentOffset, SEEK_SET);
		}
	}

	return this->current;
}

Position& PagingBuffer::Last() {
	Long currentOffset = ftell(this->file);
	Position current = this->current;

	TCHAR character[2];
	size_t flag = fread(character, 1, 1, this->file);
	while (character[0] != '\r' && flag > 0 && !feof(this->file))
	{
		if (character[0] & 0x80)
		{
			flag = fread(character + 1, 1, 1, this->file);
		}

		this->current = this->current.Right();
		flag = fread(character, 1, 1, this->file);
	}

	if (character[0] == '\r')
	{
		fseek(this->file, -1, SEEK_CUR);
	}

	return this->current;
}

Position& PagingBuffer::Move(Long index) {
	Long current = ftell(this->file);

	TCHAR character[2];
	Long i = current - 1;
	int result = fseek(this->file, i, SEEK_SET);
	fread(character, 1, 1, this->file);
	while (i >= 0 && character[0] != '\n' && result == 0)
	{
		i--;
		result = fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);
	}

	if (result != 0)
	{
		fseek(this->file, 0, SEEK_SET);
	}

	i = 0;
	while (i < index)
	{
		fread(character, 1, 1, this->file);
		if (character[0] & 0x80)
		{
			fread(character + 1, 1, 1, this->file);
		}
		i++;
	}

	this->current = this->current.Move(this->current.GetRow(), index);

	return this->current;
}

Position& PagingBuffer::FirstRow() {
	fseek(this->file, 0, SEEK_SET);

	this->current = this->current.Move(0, 0);

	return this->current;
}

Position& PagingBuffer::PreviousRow() {
	Long current = ftell(this->file);

	TCHAR character[2];
	Long count = 0;
	Long i = current - 1;
	while (i >= 0 && count < 2)
	{
		fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);

		if (character[0] == '\n')
		{
			count++;
		}

		i--;
	}

	if (count > 0)
	{
		this->current = this->current.Up();
		this->current = this->current.Move(this->current.GetRow(), 0);
	}
	else
	{
		fseek(this->file, current, SEEK_SET);
	}

	return this->current;
}

Position& PagingBuffer::NextRow() {
	Long current = ftell(this->file);

	TCHAR character;
	size_t flag = fread(&character, 1, 1, this->file);
	while (character != '\n' && flag > 0 && !feof(this->file))
	{
		flag = fread(&character, 1, 1, this->file);
	}

	if (!feof(this->file))
	{
		this->current = this->current.Move(this->current.GetRow() + 1, 0);
	}
	else
	{
		fseek(this->file, current, SEEK_SET);
	}

	return this->current;
}

Position& PagingBuffer::LastRow() {
	//1. ������ ������ �̵��Ѵ�.
	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);

	//2. ���������� �� ������ �̵��Ѵ�.
	TCHAR character[2];
	Long i = fileEnd - 1;
	int result = fseek(this->file, i, SEEK_SET);
	fread(character, 1, 1, this->file);
	while (i >= 0 && character[0] != '\n' && result == 0)
	{
		i--;
		result = fseek(this->file, i, SEEK_SET);
		fread(character, 1, 1, this->file);
	}

	if (result == 0) //3. ������ ���� ù��° ���� �ƴϸ�,
	{
		//3.1. ��� ����Ǵ� �� ���� ���Ѵ�.
		RECT rect;
		GetClientRect(this->parent->GetSafeHwnd(), &rect);
		Long clientAreaHeight = rect.bottom - rect.top;
		Long rowCount = clientAreaHeight / ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight();
		Long loadingRowCount = rowCount * PAGE_MULTIPLE;
		Long newLineNeeded = loadingRowCount / 2 - 1;

		//3.2. ���� ����Ǵ� �� ���� ���Ѵ�.
		Long newLineCount = 0;
		Long offset = i;
		fseek(this->file, offset, SEEK_SET);
		fread(character, 1, 1, this->file);
		while (offset >= 0 && newLineCount < newLineNeeded)
		{
			fseek(this->file, offset, SEEK_SET);
			fread(character, 1, 1, this->file);
			if (character[0] == '\n')
			{
				newLineCount++;
			}
			offset--;
		}

		//3.3. ������ ���� ù��° ��ġ�� �̵��Ѵ�.
		fseek(this->file, i + 1, SEEK_SET);
		this->current = this->current.Move(newLineCount, 0);
	}
	else //4. ������ ���� ù��° ���̶��,
	{
		//4.1. ������ ���� ��ġ�� �̵��Ѵ�.
		fseek(this->file, 0, SEEK_SET);
		this->current = this->current.Move(0, 0);
	}

	Long checkCurrentOffset = ftell(this->file);

	return this->current;
}

Position& PagingBuffer::MoveRow(Long index) {
	//1. ���� ��ġ�� �д´�.
	Long currentOffset = ftell(this->file);

	//2. �̵��� ���� ���� �ٺ��� �����̸�,
	TCHAR character[2];
	Long need;
	Long i;
	Long count = 0;
	if (index < this->current.GetRow())
	{
		//2.1. �̵��� ���� ã�´�.
		need = this->current.GetRow() - index + 1;
		i = currentOffset - 1;
		while (count < need && i >= 0)
		{
			fseek(this->file, i, SEEK_SET);
			fread(character, 1, 1, this->file);

			if (character[0] == '\n')
			{
				count++;
			}
			
			i--;
		}

		//2.2. ã������,
		if (count >= need)
		{  
			//2.2.1. �̵��Ѵ�.
			currentOffset = i + 1;
		}
		else //2.3. ã�� ��������,
		{
			//2.3.1. ó�� ��ġ�� �̵��Ѵ�.
			currentOffset = 0;
			index = 0;
		}

		fseek(this->file, currentOffset, SEEK_SET);
		this->current = this->current.Move(index, 0);
	}
	else if (index > this->current.GetRow()) //3. �̵��� ���� ���� �ٺ��� �����̸�,
	{
		//3.1. �̵��� ���� ã�´�.
		need = index - this->current.GetRow();
		size_t flag = fread(character, 1, 1, this->file);
		while (count < need && flag > 0 && !feof(this->file))
		{
			if (character[0] == '\n')
			{
				count++;
			}

			flag = fread(character, 1, 1, this->file);
		}

		//3.2. ã������,
		if (count >= need && !feof(this->file))
		{
			//3.2.1. �̵��Ѵ�.
			fseek(this->file, -1, SEEK_CUR);
			this->current = this->current.Move(index, 0);
		}
		else //3.3. ã�� ��������,
		{
			//3.3.1. ������ ���� �������� �̵��Ѵ�.
			i = ftell(this->file) - 1;
			fseek(this->file, i, SEEK_SET);
			fread(character, 1, 1, this->file);
			while (i > currentOffset && character[0] != '\n')
			{
				i--;
				fseek(this->file, i, SEEK_SET);
				fread(character, 1, 1, this->file);
			}
			this->current = this->current.Move(this->current.GetRow() + count, 0);

			//3.3.2. ������ ���������� �̵��Ѵ�.
			i = ftell(this->file);

			fseek(this->file, 0, SEEK_END);
			Long fileEnd = ftell(this->file);

			fseek(this->file, i, SEEK_SET);
			flag = fread(character, 1, 1, this->file);
			while (i < fileEnd && flag > 0 && !feof(this->file))
			{
				if (character[0] & 0x80)
				{
					flag = fread(character + 1, 1, 1, this->file);
					i++;
				}

				this->current = this->current.Right();
				i++;
			}
		}
	}

	return this->current;
}

Long PagingBuffer::MarkSelectionBegin() {
	this->selectionBeginOffset = ftell(this->file);

	return this->selectionBeginOffset;
}

Long PagingBuffer::UnmarkSelectionBegin() {
	this->selectionBeginOffset = -1;

	return this->selectionBeginOffset;
}

CString PagingBuffer::MakeSelectedString() {
	CString selectedString("");
	if (this->selectionBeginOffset >= 0)
	{
		Long currentOffset = ftell(this->file);

		Long forwardOffset;
		Long backwardOffset;
		if (currentOffset < this->selectionBeginOffset)
		{
			forwardOffset = currentOffset;
			backwardOffset = this->selectionBeginOffset;
		}
		else if (currentOffset > this->selectionBeginOffset)
		{
			forwardOffset = this->selectionBeginOffset;
			backwardOffset = currentOffset;
		}

		Long length = backwardOffset - forwardOffset;
		TCHAR(*contents) = new TCHAR[length + 1];

		fseek(this->file, forwardOffset, SEEK_SET);
		fread(contents, 1, length, this->file);
		contents[length] = '\0';

		selectedString = CString(contents);

		if (contents != NULL)
		{
			delete[] contents;
		}
		fseek(this->file, currentOffset, SEEK_SET);
	}

	return selectedString;
}

bool PagingBuffer::MarkAsDirty() {
	this->isDirty = true;

	return this->isDirty;
}

Long PagingBuffer::GetCurrentOffset() const {
	return ftell(this->file);
}

Long PagingBuffer::GetFileEnd() const {
	Long current = ftell(this->file);

	fseek(this->file, 0, SEEK_END);
	Long fileEnd = ftell(this->file);

	fseek(this->file, current, SEEK_SET);

	return fileEnd;
}