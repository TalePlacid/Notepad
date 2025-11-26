#include <afxwin.h>
#include "ShiftDownAction.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "MarkingHelper.h"
#include "resource.h"

#pragma warning(disable:4996)

ShiftDownAction::ShiftDownAction(CWnd* parent)
	:KeyAction(parent) {

}

ShiftDownAction::~ShiftDownAction() {

}

void ShiftDownAction::Perform() {
	//1. 현재 위치를 읽는다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	//2. 다음 줄이 적재범위에서 벗어났다면 재적재한다.
	if (note->IsBelowBottomLine(rowIndex + 1))
	{
		SendMessage(this->parent->GetSafeHwnd(), WM_COMMAND, (WPARAM)ID_COMMAND_LOADNEXT, 0);
	}

	//3. 현재 위치가 마지막 줄이 아니라면,
	MarkingHelper markingHelper(this->parent);
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	if (rowIndex < note->GetLength() - 1)
	{
		//2.1. 현재 위치까지의 너비를 구한다.
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

		//2.2. 현재 위치가 줄의 끝이 아니라면,
		i = columnIndex;
		while (i < row->GetLength())
		{
			//2.2.1. 문자를 읽는다.
			character = row->GetAt(columnIndex);

			//2.2.2. 문자가 선택되어있지 않다면,
			if (!character->IsSelected())
			{
				//2.2.2.1. 문자를 선택한다.
				character->Select(TRUE);

				//2.2.2.2. 페이징버퍼에서 선택시작위치가 표시되어있지 않으면, 표시한다.
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}

				//2.2.2.3. 줄에서 다음 칸으로 이동한다.
				columnIndex = row->Next();
				pagingBuffer->Next();
			}
			else //2.2.3. 문자가 선택되어 있다면,
			{
				//2.2.3.1. 문자를 선택취소한다.
				character->Select(FALSE);

				//2.2.3.2. 줄에서 다음 칸으로 이동한다.
				columnIndex = row->Next();
				pagingBuffer->Next();

				//2.2.3.3. 페이징버퍼에서 현재위치가 선택시작위치와 같으면, 선택시작위치의 표시를 지운다.
				if (markingHelper.HasReturnedToSelectionBegin())
				{
					markingHelper.Unmark();
				}
			}
			i++;
		}

		//2.3. 노트에서 다음 줄로 이동한다.
		rowIndex = note->Next();
		row = note->GetAt(rowIndex);
		columnIndex = row->First();
		pagingBuffer->NextRow();

		//2.4. 기존 너비와 가장 가까운 위치를 찾는다.
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

		//2.5. 찾은 위치까지 반복한다.
		i = 0;
		while (i < index)
		{
			//2.5.1. 문자를 읽는다.
			character = row->GetAt(i);

			//2.5.2. 문자가 선택되어 있지 않다면,
			if (!character->IsSelected())
			{
				//2.5.2.1. 문자를 선택한다.
				character->Select(TRUE);

				//2.5.2.2. 페이징 버퍼에서 선택시작위치가 표시되어 있지 않다면, 표시한다.
				if (markingHelper.IsUnmarked())
				{
					markingHelper.Mark();
				}

				//2.5.2.3. 다음칸으로 이동한다.
				row->Next();
				pagingBuffer->Next();
			}
			else //2.5.3. 문자가 선택되어 있다면,
			{
				//2.5.3.1. 문자를 선택취소한다.
				character->Select(FALSE);

				//2.5.3.2. 다음 칸으로 이동한다.
				row->Next();
				pagingBuffer->Next();

				//2.5.3.3. 페이징버퍼에서 현재위치랑 선택시작위치가 같으면, 표시를 지운다.
				if (markingHelper.HasReturnedToSelectionBegin())
				{
					markingHelper.Unmark();
				}
			}

			i++;
		}
	}
}