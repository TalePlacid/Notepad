#include <afxwin.h>
#include "EraseCommand.h"
#include "NotepadForm.h"
#include "Glyph.h"
#include "PagingBuffer.h"

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
		Glyph* note = ((NotepadForm*)(this->parent))->note;
		Long rowIndex = note->GetCurrent();
		Glyph* row = note->GetAt(rowIndex);

		PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
		Long columnIndex = row->GetCurrent();
		if (columnIndex > 0)
		{
			row->Remove(columnIndex - 1);
			pagingBuffer->Remove();
		}
		else
		{
			if (rowIndex > 0)
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
				pagingBuffer->Remove();
				previousRow->Move(previousCurrent);
			}
		}
	}
}