#include "CaretController.h"
#include "Caret.h"
#include "NotepadForm.h"
#include "glyphs/Glyph.h"
#include "SizeCalculator.h"
#include "PagingBuffer.h"
#include "ScrollController.h"

#pragma warning(disable:4996)

CaretController::CaretController(CWnd* parent) {
	this->parent = parent;
	this->caret = NULL;
}

CaretController::~CaretController() {
	if (this->caret != NULL)
	{
		delete this->caret;
	}
}

Caret* CaretController::Create() {
	//1. 노트의 시작높이를 구한다.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long startRowHeight = pagingBuffer->GetRowStartIndex() * sizeCalculator->GetRowHeight();

	//2. 노트에서의 높이를 구한다.
	Long nPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	Long height = nPos - startRowHeight;
	Long y = rowIndex * sizeCalculator->GetRowHeight() - height;

	//3. 현재 위치까지의 너비를 구한다.
	Glyph* character = NULL;
	Long width = 0;
	Long i = 0;
	while (i < columnIndex)
	{
		character = row->GetAt(i);
		width += sizeCalculator->GetCharacterWidth((char*)(*character));
		i++;
	}

	nPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_HORZ);
	Long x = width - nPos;

	//4. 캐럿 너비를 구한다.
	Long caretWidth = 1;
	if (columnIndex < row->GetLength() && pagingBuffer->GetSelectionBeginOffset() < 0)
	{
		character = row->GetAt(columnIndex);
		if (character->IsMultiByteCharacter())
		{
			caretWidth = sizeCalculator->GetMultiByteWidth();
		}
	}

	if (this->caret != NULL)
	{
		delete this->caret;
		this->caret = NULL;
	}
	this->caret = new Caret(this->parent, x, y, caretWidth, ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight());

	return this->caret;
}

Caret* CaretController::Destroy() {
	if (this->caret != NULL)
	{
		delete this->caret;
		this->caret = NULL;
	}

	return this->caret;
}

void CaretController::Update(Subject *subject, string interest) {
	if (interest == "ChangeCaret")
	{
		this->Create();
#if 0
		//1. 기존 캐럿이 있다면, 지운다.
		if (this->caret != NULL)
		{
			delete this->caret;
			this->caret = NULL;
		}

		//2. 노트에서 캐럿이 가리키는 문자의 위치를 읽는다.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//3. 캐럿의 y좌표를 구한다.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		ScrollController* scrollController = ((NotepadForm*)(this->parent))->scrollController;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;

		Long startRowIndex = pagingBuffer->GetRowStartIndex();
		Long rowHeight = sizeCalculator->GetRowHeight();
		Long y = (startRowIndex + rowIndex) * rowHeight - scrollController->GetVScroll().GetPos();

		//4. 캐럿의 x좌표를 구한다.
		Long width = 0;
		Long i = 0;
		while (i < columnIndex && i < row->GetLength())
		{
			width += sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(i)));
			i++;
		}
		Long x = width - scrollController->GetHScroll().GetPos();

		//5. 캐럿 너비를 구한다.
		Long caretWidth = 1;
		if (columnIndex < row->GetLength())
		{
			if (row->GetAt(columnIndex)->IsMultiByteCharacter())
			{
				caretWidth = sizeCalculator->GetCharacterWidth((char*)(*row->GetAt(columnIndex)));
			}
		}

		//6. 새 캐럿을 만든다.
		this->caret = new Caret(this->parent, x, y, caretWidth, rowHeight);
#endif
	}
}