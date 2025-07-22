#include <afxwin.h>
#include <WinBase.h>
#include "PagingBuffer_Fixed.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "NoteConverter.h"
#include "SizeCalculator.h"

#pragma warning(disable:4996)

PagingBuffer_Fixed::PagingBuffer_Fixed(CWnd* parent, Long pageSize)
	: PagingBuffer(parent, pageSize) {

}

PagingBuffer_Fixed::~PagingBuffer_Fixed() {

}

void PagingBuffer_Fixed::Load() {
	//1. ���� ������ ��ġ�� ���Ѵ�.
	Long currentOffset = ftell(this->file);

	//2. ������ �� ���� ���Ѵ�.
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	RECT rect;
	GetClientRect(this->parent->GetSafeHwnd(), &rect);
	Long clientAreaHeight = rect.bottom - rect.top;
	Long rowCount = clientAreaHeight / sizeCalculator->GetRowHeight();
	Long loadingRowCount = rowCount * PAGE_MULTIPLE;

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
	((NotepadForm*)(this->parent))->note = noteConverter.Convert(contents);

	//8. ��Ʈ���� ���� ��ġ�� �̵��Ѵ�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->Move(this->current.GetRow());
	Glyph* row = note->GetAt(rowIndex);
	row->Move(this->current.GetColumn());

	//9. ���ý�����ġ�� ǥ�õǾ� ������, 
	if (this->selectionBeginOffset >= 0)
	{
		Long j;

		//9.1 ���ý�����ġ�� ������ġ���� ���̸�,
		if (this->selectionBeginOffset < currentOffset)
		{
			//9.1.1. ���ý�����ġ�� ������ġ���� �Ʒ��̸�,
			Position selectionStart(0, 0);
			if (this->selectionBeginOffset > this->startOffset)
			{
				//9.1.1.1. ���ý�����ǥ�� ���Ѵ�.
				i = this->startOffset;
				while (i < this->selectionBeginOffset)
				{
					fseek(this->file, i, SEEK_SET);
					fread(character, 1, 1, this->file);

					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
					}

					if (character[0] != '\r')
					{
						selectionStart = selectionStart.Right();
					}
					else
					{
						selectionStart = selectionStart.Down();
						selectionStart = selectionStart.Move(selectionStart.GetRow(), 0);
					}

					i++;
				}
			}

			//9.1.2. ��Ʈ���� ���ý�����ǥ���� ������ǥ���� �����Ѵ�.
			i = selectionStart.GetRow();
			if (i < this->current.GetRow())
			{
				row = note->GetAt(i);
				j = selectionStart.GetColumn();
				while (j < row->GetLength())
				{
					row->GetAt(j)->Select(TRUE);
					j++;
				}
			}

			i = selectionStart.GetRow() + 1;
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
		else if (this->selectionBeginOffset > currentOffset) //9.2 ���ý�����ġ�� ������ġ���� �Ʒ��̸�,
		{
			//9.2.1.���ý�����ġ�� ����ġ���� ���̸�,
			Position selectionEnd = this->end;
			if (this->selectionBeginOffset < this->endOffset)
			{
				//9.2.1.1. ���ó���ǥ�� ���Ѵ�.
				i = this->selectionBeginOffset;
				while (i < this->selectionBeginOffset)
				{
					fseek(this->file, i, SEEK_SET);
					fread(character, 1, 1, this->file);

					if (character[0] == '\r' || character[0] & 0x80)
					{
						fread(character + 1, 1, 1, this->file);
					}

					if (character[0] != '\r')
					{
						selectionEnd = selectionEnd.Right();
					}
					else
					{
						selectionEnd = selectionEnd.Down();
						selectionEnd = selectionEnd.Move(selectionEnd.GetRow(), 0);
					}
					i++;
				}
			}

			//9.2.2. ��Ʈ���� ������ǥ���� ���ó���ǥ���� �����Ѵ�.
			row = note->GetAt(this->current.GetRow());
			i = this->current.GetColumn();
			if (i < row->GetLength())
			{
				row->GetAt(i)->Select(TRUE);
				i++;
			}

			i = this->current.GetRow() + 1;
			while (i < selectionEnd.GetRow())
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

			row = note->GetAt(selectionEnd.GetRow());
			i = 0;
			while (i < selectionEnd.GetColumn())
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