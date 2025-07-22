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
	//1. 노트에서 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 첫번째 페이지의 첫번째 줄이 아니면,
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (pagingBuffer->GetStartOffset() > 0 || rowIndex > 0)
	{
		//2.1. 기존 줄에서 첫번째 위치까지 반복한다.
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Glyph* character;
		Long originalWidth = 0;
		Long i = columnIndex - 1;
		while (i >= 0)
		{
			//2.1.1. 문자를 읽는다.
			character = row->GetAt(i);

			//2.1.2. 문자가 선택되지 않았다면,
			if (!character->IsSelected())
			{
				//2.1.2.1. 문자를 선택한다.
				character->Select(TRUE);

				//2.1.2.2. 페이징버퍼에서 선택시작위치가 표시되지 않았다면, 표시한다.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//2.1.2.3. 이전 칸으로 이동한다.
				row->Previous();
				pagingBuffer->Previous();
			}
			else //2.1.3. 문자가 선택되어있다면,
			{
				//2.1.3.1. 문자를 선택취소한다.
				character->Select(FALSE);

				//2.1.3.2. 이전 칸으로 이동한다.
				row->Previous();
				pagingBuffer->Previous();

				//2.1.3.3. 페이징 버퍼의 현재 위치가 선택시작위치와 같으면, 표시를 지운다.
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}

			originalWidth += sizeCalculator->GetCharacterWidth((char*)(*character));

			i--;
		}

		//2.2. 노트에서 이전 줄로 이동한다.
		rowIndex = note->Previous();
		pagingBuffer->PreviousRow();

		//2.3. 줄에서 너비와 가장 가까운 위치를 찾는다.
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

		//2.4. 줄에서 끝으로 이동한다.
		columnIndex = row->Last();
		pagingBuffer->Last();

		//2.5. 위치까지 반복한다.
		i = columnIndex - 1;
		while (i >= index)
		{
			//2.5.1. 문자를 읽는다.
			character = row->GetAt(i);

			//2.5.2. 문자가 선택되어 있지 않다면,
			if (!character->IsSelected())
			{
				//2.5.2.1. 문자를 선택한다.
				character->Select(TRUE);

				//2.5.2.2. 페이징 버퍼에서 선택시작위치가 표시되지 않았다면, 표시한다.
				if (pagingBuffer->GetSelectionBeginOffset() < 0)
				{
					pagingBuffer->MarkSelectionBegin();
				}

				//2.5.2.3. 줄에서 이전 칸으로 이동한다.
				row->Previous();
				pagingBuffer->Previous();
			}
			else //2.5.3. 문자가 선택되어 있다면,
			{
				//2.5.3.1. 문자를 선택취소한다.
				character->Select(FALSE);

				//2.5.3.2. 줄에서 이전 칸으로 이동한다.
				row->Previous();
				pagingBuffer->Previous();

				//2.5.3.3. 페이징버퍼의 현재위치가 선택시작위치와 같다면, 표시를 지운다.
				if (pagingBuffer->GetCurrentOffset() == pagingBuffer->GetSelectionBeginOffset())
				{
					pagingBuffer->UnmarkSelectionBegin();
				}
			}

			i--;
		}

		//2.6. 범위를 벗어났다면, 재적재한다.
		if (!pagingBuffer->IsBelowTopLine())
		{
			pagingBuffer->Load();
		}

		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		this->parent->Invalidate();
	}
}