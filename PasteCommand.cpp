#include <afxwin.h>
#include <cstring>
using namespace std;
#include "PasteCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "GlyphFactory.h"
#include "ClipboardController.h"

PasteCommand::PasteCommand(CWnd* parent)
	:Command(parent) {

}

PasteCommand::~PasteCommand() {

}

void PasteCommand::Execute() {
	if (((NotepadForm*)(this->parent))->clipboardController->Paste())
	{
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);
		Long columnIndex = row->GetCurrent();

		TCHAR content[2];
		CString str = ((NotepadForm*)(this->parent))->clipboardController->GetContent();
		Glyph* glyph;
		GlyphFactory glyphFactory;

		Long length = strlen((LPCTSTR)str);
		Long i = 0;
		while (i < length)
		{
			content[0] = str.GetAt(i);
			if (content[0] & 0x80 || content[0] == '\r')
			{
				content[1] = str.GetAt(++i);
			}

			glyph = glyphFactory.Create(content);
			if (content[0] != '\r')
			{
				columnIndex = row->Add(columnIndex + 1, glyph);
			}
			else
			{
				rowIndex = note->Add(rowIndex + 1, glyph);
				row = note->GetAt(rowIndex);
				columnIndex = row->GetCurrent();
			}
			i++;
		}
		this->parent->Invalidate();
	}
}