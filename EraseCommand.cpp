#include <afxwin.h>
#include "EraseCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

EraseCommand::EraseCommand(CWnd* parent)
	:Command(parent) {

}

EraseCommand::~EraseCommand() {

}

EraseCommand::EraseCommand(const EraseCommand& source)
	:Command(source) {

}

EraseCommand& EraseCommand::operator=(const EraseCommand& source) {
	Command::operator=(source);

	return *this;
}

void EraseCommand::Execute() {
	if (!((NotepadForm*)(this->parent))->IsCompositing())
	{
		Long rowIndex = ((NotepadForm*)(this->parent))->note->GetCurrent();
		Glyph* row = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex);

		Long columnIndex = row->GetCurrent();
		if (columnIndex > -1)
		{
			row->Remove(columnIndex);
		}
		else if (rowIndex > 0)
		{
			Glyph* previousRow = ((NotepadForm*)(this->parent))->note->GetAt(rowIndex - 1);
			Long previousCurrent = previousRow->Last();
			Long i = 0;
			while (i < row->GetLength())
			{
				previousRow->Add(row->GetAt(i)->Clone());
				i++;
			}

			((NotepadForm*)(this->parent))->note->Remove(rowIndex);
			previousRow->Move(previousCurrent);
		}
	}
}