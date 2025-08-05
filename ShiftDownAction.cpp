#include <afxwin.h>
#include "ShiftDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ShiftDownAction::ShiftDownAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftDownAction::~ShiftDownAction() {

}

void ShiftDownAction::Perform() {
	//1. ���� ��ġ�� �д´�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. ���� ��ġ�� ������ �������� ���������� �ƴ϶��,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetEndOffset() < pagingBuffer->GetFileEndOffset() || rowIndex < note->GetLength() - 1)
	{
		//2.1. ���� ��ġ������ �ʺ� ���Ѵ�.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Glyph* character;
		Long originalWidth = 0;
		Long i = 0;
		while (i < columnIndex)
		{
			character = row->GetAt(i);
			originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		//2.2. ���� ��ġ�� ���� ���� �ƴ϶��,
		i = columnIndex;
		while (i < row->GetLength())
		{
			//2.2.1. ���ڸ� �д´�.
			character = row->GetAt(columnIndex);

			//2.2.2. ���ڰ� ���õǾ����� �ʴٸ�,
			if (!character->IsSelected())
			{
				//2.2.2.1. ���ڸ� �����Ѵ�.
				character->Select(TRUE);

				//2.2.2.2. ����¡���ۿ��� ���ý�����ġ�� ǥ�õǾ����� ������, ǥ���Ѵ�.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//2.2.2.3. �ٿ��� ���� ĭ���� �̵��Ѵ�.
				columnIndex = row->Next();
				pagingBuffer->Next();
			}
			else //2.2.3. ���ڰ� ���õǾ� �ִٸ�,
			{
				//2.2.3.1. ���ڸ� ��������Ѵ�.
				character->Select(FALSE);

				//2.2.3.2. �ٿ��� ���� ĭ���� �̵��Ѵ�.
				columnIndex = row->Next();
				pagingBuffer->Next();

				//2.2.3.3. ����¡���ۿ��� ������ġ�� ���ý�����ġ�� ������, ���ý�����ġ�� ǥ�ø� �����.
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}
			i++;
		}

		//2.3. ��Ʈ���� ���� �ٷ� �̵��Ѵ�.
		rowIndex = note->Next();
		pagingBuffer->NextRow();

		//2.4. �ٿ��� ù��° ĭ���� �̵��Ѵ�.
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		pagingBuffer->First();

		//2.5. ���� �ʺ�� ���� ����� ��ġ�� ã�´�.
		Long previousWidth = 0;
		Long width = 0;
		i = 0;
		while (i < row->GetLength() && width < originalWidth)
		{
			character = row->GetAt(i);
			previousWidth = width;
			width += sizeCalculator->GetCharacterWidth((char*)(*character));
			i++;
		}

		Long index = i;
		if (width - originalWidth > originalWidth - previousWidth)
		{
			index--;
		}

		//2.6. ã�� ��ġ���� �ݺ��Ѵ�.
		i = 0;
		while (i < index)
		{
			//2.6.1. ���ڸ� �д´�.
			character = row->GetAt(i);

			//2.6.2. ���ڰ� ���õǾ� ���� �ʴٸ�,
			if (!character->IsSelected())
			{
				//2.6.2.1. ���ڸ� �����Ѵ�.
				character->Select(TRUE);

				//2.6.2.2. ����¡ ���ۿ��� ���ý�����ġ�� ǥ�õǾ� ���� �ʴٸ�, ǥ���Ѵ�.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//2.6.2.3. ����ĭ���� �̵��Ѵ�.
				row->Next();
				pagingBuffer->Next();
			}
			else //2.6.3. ���ڰ� ���õǾ� �ִٸ�,
			{
				//2.6.3.1. ���ڸ� ��������Ѵ�.
				character->Select(FALSE);

				//2.6.3.2. ���� ĭ���� �̵��Ѵ�.
				row->Next();
				pagingBuffer->Next();

				//2.6.3.3. ����¡���ۿ��� ������ġ�� ���ý�����ġ�� ������, ǥ�ø� �����.
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}

			i++;
		}

		//2.7. ������ �������, �������Ѵ�.
		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}

		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		this->parent->Invalidate();
	}
}