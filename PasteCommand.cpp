#include <afxwin.h>
#include <cstring>
using namespace std;
#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ClipboardController.h"
#include "PagingBuffer.h"

#pragma warning(disable:4996)

PasteCommand::PasteCommand(CWnd* parent)
	:Command(parent) {

}

PasteCommand::~PasteCommand() {

}

void PasteCommand::Execute() {
	BOOL isPasted = ((NotepadForm*)(this->parent))->clipboardController->Paste();
	if (isPasted)
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		TCHAR contents[2];
		CString str = ((NotepadForm*)(this->parent))->clipboardController->GetContent();
		Glyph* glyph;
		GlyphFactory glyphFactory;

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long length = strlen((LPCTSTR)str);
		Long i = 0;
		while (i < length)
		{
			contents[0] = str.GetAt(i);
			if (contents[0] & 0x80 || contents[0] == '\r')
			{
				contents[1] = str.GetAt(++i);
			}

			glyph = glyphFactory.Create(contents);
			if (contents[0] != '\r')
			{
				columnIndex = row->Add(columnIndex, glyph);
				pagingBuffer->Add((char*)(*glyph));
			}
			else
			{
				rowIndex = note->Add(rowIndex + 1, glyph);
				pagingBuffer->Add(contents);
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
			i++;
		}

		if (!pagingBuffer->IsAboveBottomLine())
		{
			pagingBuffer->Load();
		}

		((NotepadForm*)(this->parent))->Notify("AdjustScrollBars");
		this->parent->Invalidate();
	}
}