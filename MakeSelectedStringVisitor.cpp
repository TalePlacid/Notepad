#include "MakeSelectedStringVisitor.h"
#include "NotepadForm.h"
#include "Glyph.h"

#pragma warning(disable:4996)

MakeSelectedStringVisitor::MakeSelectedStringVisitor(CWnd* parent)
	:Visitor(parent), selectedString("") {
	this->hasSelectedCharacter = FALSE;
	this->rowIndex = 0;
}

MakeSelectedStringVisitor::~MakeSelectedStringVisitor() {

}

void MakeSelectedStringVisitor::VisitRow(Glyph* row) {
	if ((this->rowIndex < ((NotepadForm*)(this->parent))->note->GetLength() - 1) && this->hasSelectedCharacter)
	{
		this->selectedString += CString("\r\n");
	}
	this->hasSelectedCharacter = FALSE;
	(this->rowIndex)++;
}

void MakeSelectedStringVisitor::VisitCharacter(Glyph* character) {
	if (character->IsSelected())
	{
		this->selectedString += CString(character->MakeString().c_str());
		this->hasSelectedCharacter = TRUE;
	}
}

