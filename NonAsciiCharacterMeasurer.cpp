#include <afxwin.h>
#include "NonAsciiCharacterMeasurer.h"
#include "NotepadForm.h"

#pragma warning(disable:4996)

NonAsciiCharacterMeasurer::NonAsciiCharacterMeasurer() {

}

NonAsciiCharacterMeasurer::~NonAsciiCharacterMeasurer() {

}

Long NonAsciiCharacterMeasurer::MesureWidth(CWnd* parent, char(*character)) {
	char character_[3];
	character_[0] = character[0];
	character_[1] = character[1];
	character_[2] = '\0';
	
	CDC* cdc = parent->GetDC();
	CFont* oldFont = cdc->SelectObject(((NotepadForm*)parent)->GetDisplayFont());

	Long width = cdc->GetTextExtent(character_).cx;

	cdc->SelectObject(oldFont);
	parent->ReleaseDC(cdc);

	return width;
}
