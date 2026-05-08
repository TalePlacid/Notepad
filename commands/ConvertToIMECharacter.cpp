#include <afxwin.h>
#include "ConvertToIMECharacterCommand.h"
#include "../NotepadForm.h"
#include "../TimerConstants.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../CaretNavigator.h"
#include "../glyphs/GlyphFactory.h"
#include "../IMEController.h"

#pragma warning(disable:4996)

ConvertToIMECharacterCommand::ConvertToIMECharacterCommand(CWnd* parent, const char* character)
	:Command(parent) {
	this->source[0] = '\0';
	this->source[1] = '\0';
	this->replaced[0] = character[0];;
	this->replaced[1] = character[1];
	this->offset = -1;
	this->columnIndex = 0;
}

ConvertToIMECharacterCommand::~ConvertToIMECharacterCommand() {

}

ConvertToIMECharacterCommand::ConvertToIMECharacterCommand(const ConvertToIMECharacterCommand& source)
	:Command(source) {
	this->source[0] = source.source[0];
	this->source[1] = source.source[1];
	this->replaced[0] = source.replaced[0];
	this->replaced[1] = source.replaced[1];
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;
}

ConvertToIMECharacterCommand& ConvertToIMECharacterCommand::operator=(const ConvertToIMECharacterCommand& source) {
	Command::operator=(source);

	this->source[0] = source.source[0];
	this->source[1] = source.source[1];
	this->replaced[0] = source.replaced[0];
	this->replaced[1] = source.replaced[1];
	this->offset = source.offset;
	this->columnIndex = source.columnIndex;

	return *this;
}

void ConvertToIMECharacterCommand::Execute() {
	//1. ÈÄº¸°¡ ¼±ÅÃµÇ¾úÀ¸¸é,
	WCHAR wideCharacter;
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, this->replaced, 2,
		&wideCharacter, 1);
	if (!(wideCharacter >= L'°¡' && wideCharacter <= L'ÆR')
		&& !(wideCharacter >= L'¤¡' && wideCharacter <= L'¤¾'))
	{
		this->isUndoable = true;

		//1. ¼±ÅÃ¹üÀ§°¡ ÀÖ°í, ÇöÀç À§Ä¡°¡ ¼±ÅÃ¹üÀ§ÀÇ ¸Ç ¾ÕÀÌ ¾Æ´Ï¸é, ¸Ç ¾Õ À§Ä¡·Î ÀÌµ¿ÇÑ´Ù.
		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long selectionBeginOffset = pagingBuffer->GetSelectionBeginOffset();
		Long currentOffset = pagingBuffer->GetCurrentOffset();
		if (selectionBeginOffset >= 0 && selectionBeginOffset < currentOffset)
		{
			CaretNavigator caretNavigator(this->parent);
			caretNavigator.MoveTo(selectionBeginOffset);
			caretNavigator.NormalizeColumn(0);
		}

		//2. ¼±ÅÃÀ» Ãë¼ÒÇÑ´Ù.
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		note->Select(FALSE);
		pagingBuffer->UnmarkSelectionBegin();

		//3. ³ëÆ®¿¡¼­ ±³Ã¼ÇÑ´Ù.
		this->offset = pagingBuffer->GetCurrentOffset();

		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		this->columnIndex = row->GetCurrent();

		TCHAR(*character) = (char*)(*row->GetAt(this->columnIndex));
		this->source[0] = character[0];
		this->source[1] = character[1];

		GlyphFactory glyphFactory;
		Glyph* glyph = glyphFactory.Create(this->replaced);
		row->Replace(this->columnIndex, glyph);
		row->Next();

		//4. ÆäÀÌÂ¡ ¹öÆÛ¿¡¼­ ±³Ã¼ÇÑ´Ù.
		pagingBuffer->Replace(this->offset, glyph->MakeString().c_str());
	}
}

void ConvertToIMECharacterCommand::Undo() {
	//1. ¿ø·¡ À§Ä¡·Î ÀÌµ¿ÇÑ´Ù.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. ³ëÆ®¿¡¼­ ±³Ã¼ÇÑ´Ù.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->source, true);
	row->Replace(columnIndex, glyph);
	columnIndex = row->Next();

	//3. ÆäÀÌÂ¡ ¹öÆÛ¿¡¼­ ±³Ã¼ ¹× ¼±ÅÃÇÑ´Ù.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MarkSelectionBegin();
	TCHAR source[3] = { this->source[0], this->source[1], '\0' };
	pagingBuffer->Replace(this->offset, CString(source));
}

void ConvertToIMECharacterCommand::Redo() {
	//1. ¿ø·¡ À§Ä¡·Î ÀÌµ¿ÇÑ´Ù.
	CaretNavigator caretNavigator(this->parent);
	caretNavigator.MoveTo(this->offset);
	caretNavigator.NormalizeColumn(this->columnIndex);

	//2. ³ëÆ®¿¡¼­ ±³Ã¼ÇÑ´Ù.
	Glyph* note = ((NotepadForm*)(this->parent))->note;
	Long rowIndex = note->GetCurrent();
	Glyph* row = note->GetAt(rowIndex);
	Long columnIndex = row->GetCurrent();

	GlyphFactory glyphFactory;
	Glyph* glyph = glyphFactory.Create(this->replaced, true);
	row->Replace(columnIndex, glyph);
	columnIndex = row->Next();

	//3. ÆäÀÌÂ¡ ¹öÆÛ¿¡¼­ ±³Ã¼ ¹× ¼±ÅÃÇÑ´Ù.
	PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
	pagingBuffer->MarkSelectionBegin();
	TCHAR replaced[3] = { this->replaced[0], this->replaced[1], '\0' };
	pagingBuffer->Replace(this->offset, CString(replaced));
}

Command* ConvertToIMECharacterCommand::Clone() {
	return new ConvertToIMECharacterCommand(*this);
}