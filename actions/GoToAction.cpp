#include <afxwin.h>
#include "GoToAction.h"
#include "../GoToForm.h"
#include "../Editor.h"
#include "../NotepadForm.h"
#include "../PagingBuffer.h"
#include "../glyphs/Glyph.h"

#pragma warning(disable:4996)

GoToAction::GoToAction(CWnd* parent)
	:Action(parent) {

}

GoToAction::~GoToAction() {

}

void GoToAction::Perform() {
	NotepadForm* notepadForm = (NotepadForm*)(this->parent);
	if (!notepadForm->IsAutoWrapped())
	{
		//1. 줄 이동 대화상자를 띄운다.
		GoToForm goToForm(notepadForm);
		INT_PTR result = goToForm.DoModal();

		//2. 이동할 줄번호가 입력되었으면, 이동한다.
		if (result == IDOK)
		{
			Long lineNumber = goToForm.GetLineNumber() - 1;
			if (lineNumber >= 0)
			{
				PagingBuffer* pagingBuffer = notepadForm->pagingBuffer;
				Glyph* note = notepadForm->note;

				//2.1. 줄의 첫 칸 위치(columnIndex == 0)로 이동한다.
				Long rowIndex = note->GetCurrent();
				Glyph* row = note->GetAt(rowIndex);
				Long columnIndex = row->GetCurrent();
				Long firstColumnIndex = row->First();
				pagingBuffer->Previous(columnIndex - firstColumnIndex);

				//2.2. 줄 시작번호 + 현재 줄위치 - 이동할 줄번호를 구한다.
				Long difference = pagingBuffer->GetRowStartIndex() + note->GetCurrent() - lineNumber;
				Editor editor(this->parent);

				Long i = 0;
				//2.3. 음수이면 MoveDown()을 횟수만큼 반복한다.
				if (difference < 0)
				{
					difference *= -1;
					while (i < difference)
					{
						editor.MoveDown();
						i++;
					}
				}
				else if (difference > 0) //2.4. 양수이면 MoveUp()을 횟수만큼 반복한다.
				{
					while (i < difference)
					{
						editor.MoveUp();
						i++;
					}
				}
			}
		}
	}
}
