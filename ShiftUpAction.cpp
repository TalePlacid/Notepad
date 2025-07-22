#include <afxwin.h>
#include "ShiftUpAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

ShiftUpAction::ShiftUpAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftUpAction::~ShiftUpAction() {

}

void ShiftUpAction::Perform() {
	//1. ��Ʈ���� ���� ��ġ�� �д´�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. ù��° �������� ù��° ���� �ƴϸ�,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetStartOffset() > 0 || rowIndex > 0)
	{
		//2.1. ���� �ٿ��� ù��° ��ġ���� �ݺ��Ѵ�.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Glyph* character;
		Long originalWidth = 0;
		Long i = columnIndex - 1;
		while (i >= 0)
		{
			//2.1.1. ���ڸ� �д´�.
			character = row->GetAt(i);

			//2.1.2. ���ڰ� ���õ��� �ʾҴٸ�,
			if (!character->IsSelected())
			{
				//2.1.2.1. ���ڸ� �����Ѵ�.
				character->Select(TRUE);

				//2.1.2.2. ����¡���ۿ��� ���ý�����ġ�� ǥ�õ��� �ʾҴٸ�, ǥ���Ѵ�.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//2.1.2.3. ���� ĭ���� �̵��Ѵ�.
				row->Previous();
				pagingBuffer->Previous();
			}
			else //2.1.3. ���ڰ� ���õǾ��ִٸ�,
			{
				//2.1.3.1. ���ڸ� ��������Ѵ�.
				character->Select(FALSE);

				//2.1.3.2. ���� ĭ���� �̵��Ѵ�.
				row->Previous();
				pagingBuffer->Previous();

				//2.1.3.3. ����¡ ������ ���� ��ġ�� ���ý�����ġ�� ������, ǥ�ø� �����.
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}

			originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));

			i--;
		}

		//2.2. ��Ʈ���� ���� �ٷ� �̵��Ѵ�.
		rowIndex = note->Previous();
		pagingBuffer->PreviousRow();

		//2.3. �ٿ��� �ʺ�� ���� ����� ��ġ�� ã�´�.
		row = note->GetAt(rowIndex);
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

		//2.4. �ٿ��� ������ �̵��Ѵ�.
		columnIndex = row->Last();
		pagingBuffer->Last();

		//2.5. ��ġ���� �ݺ��Ѵ�.
		i = columnIndex - 1;
		while (i >= index)
		{
			//2.5.1. ���ڸ� �д´�.
			character = row->GetAt(i);

			//2.5.2. ���ڰ� ���õǾ� ���� �ʴٸ�,
			if (!character->IsSelected())
			{
				//2.5.2.1. ���ڸ� �����Ѵ�.
				character->Select(TRUE);

				//2.5.2.2. ����¡ ���ۿ��� ���ý�����ġ�� ǥ�õ��� �ʾҴٸ�, ǥ���Ѵ�.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//2.5.2.3. �ٿ��� ���� ĭ���� �̵��Ѵ�.
				row->Previous();
				pagingBuffer->Previous();
			}
			else //2.5.3. ���ڰ� ���õǾ� �ִٸ�,
			{
				//2.5.3.1. ���ڸ� ��������Ѵ�.
				character->Select(FALSE);

				//2.5.3.2. �ٿ��� ���� ĭ���� �̵��Ѵ�.
				row->Previous();
				pagingBuffer->Previous();

				//2.5.3.3. ����¡������ ������ġ�� ���ý�����ġ�� ���ٸ�, ǥ�ø� �����.
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}

			i--;
		}

		//2.6. ������ ����ٸ�, �������Ѵ�.
		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
		}

		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		this->parent->Invalidate();
	}
}