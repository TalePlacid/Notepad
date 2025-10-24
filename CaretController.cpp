#include "CaretController.h"
#include "Caret.h"
#include "SizeCalculator.h"
#include "Glyph.h"
#include "NotepadForm.h"
#include "PagingBuffer.h"

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
	//1. ��Ʈ�� ���۳��̸� ���Ѵ�.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
	Long startRowHeight = pagingBuffer->GetRowStartIndex() * sizeCalculator->GetRowHeight();

	//2. ��Ʈ������ ���̸� ���Ѵ�.
	Long nPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
	Long height = nPos - startRowHeight;
	Long y = rowIndex * sizeCalculator->GetRowHeight() - height;

	//3. ���� ��ġ������ �ʺ� ���Ѵ�.
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

	//4. ĳ�� �ʺ� ���Ѵ�.
	Long caretWidth = 1;
	if (columnIndex < row->GetLength())
	{
		character = row->GetAt(columnIndex);
		if (character->IsMultiByteCharacter())
		{
			caretWidth = sizeCalculator->GetMultiByteWidth();
		}
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
		//1. ���� ĳ���� �ִٸ�, �����.
		if (this->caret != NULL)
		{
			delete this->caret;
			this->caret = NULL;
		}

		//2. ��Ʈ���� ĳ���� ����Ű�� ������ ��ġ�� �д´�.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		//3. ��Ʈ�� ���۳��̸� ���Ѵ�.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		SizeCalculator* sizeCalculator = ((NotepadForm*)(this->parent))->sizeCalculator;
		Long startRowHeight = pagingBuffer->GetRowStartIndex() *sizeCalculator->GetRowHeight();

		//4. ��Ʈ������ ���̸� ���Ѵ�.
		Long nPos = GetScrollPos(this->parent->GetSafeHwnd(), SB_VERT);
		Long height = nPos - startRowHeight;
		Long y = rowIndex * sizeCalculator->GetRowHeight() - height;
		
		//5. ���� ��ġ������ �ʺ� ���Ѵ�.
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

		//6. ĳ�� �ʺ� ���Ѵ�.
		Long caretWidth = 1;
		if (columnIndex < row->GetLength())
		{
			character = row->GetAt(columnIndex);
			if (character->IsMultiByteCharacter())
			{
				caretWidth = sizeCalculator->GetMultiByteWidth();
			}
		}
		this->caret = new Caret(this->parent, x, y, caretWidth, ((NotepadForm*)(this->parent))->sizeCalculator->GetRowHeight());
	}
}