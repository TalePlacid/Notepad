#include <afxwin.h>
#include <imm.h>
#include "StartImeConversionAction.h"
#include "../NotepadForm.h"
#include "../glyphs/Glyph.h"
#include "../PagingBuffer.h"
#include "../IMEs/IMEAdapter.h"
#include "../ByteChecker.h"

#pragma warning(disable:4996)
#pragma comment(lib, "imm32.lib")

StartImeConversionAction::StartImeConversionAction(CWnd* parent)
    :Action(parent) {

}

StartImeConversionAction::~StartImeConversionAction() {

}

void StartImeConversionAction::Perform() {
    //1. ÇÑ±Û ÈÄº¸¸¦ Ã£´Â´Ù.
    PagingBuffer* pagingBuffer = ((NotepadForm*)(this->parent))->pagingBuffer;
    Glyph* note = ((NotepadForm*)(this->parent))->note;
    Long rowIndex = note->GetCurrent();
    Glyph* row = note->GetAt(rowIndex);
    Long columnIndex = row->GetCurrent();
    
    TCHAR candidate[3] = { '\0', '\0', '\0' };
    if (pagingBuffer->GetSelectionBeginOffset() >= 0)
    {
        CString selected = pagingBuffer->MakeSelectedString();
        TCHAR firstSelected[2] = { '\0', '\0'};
        firstSelected[0] = selected.GetAt(0);
        if (!ByteChecker::IsASCII(firstSelected))
        {
            firstSelected[1] = selected.GetAt(1);
        } 

        WCHAR wideCharacter;
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, firstSelected, 2,
            &wideCharacter, 1);

        if (wideCharacter >= L'°¡' && wideCharacter <= L'ÆR'
            || wideCharacter >= L'¤¡' && wideCharacter <= L'¤¾')
        {
            candidate[0] = firstSelected[0];
            candidate[1] = firstSelected[1];
        }
    }
    else if (columnIndex < row->GetLength() && row->GetAt(columnIndex)->IsMultiByteCharacter())
    {
        char(*character) = (char*)(*row->GetAt(columnIndex));
        candidate[0] = character[0];
        candidate[1] = character[1];
    }

    //2. Ã£¾Ò°í, ÈÄº¸¸®½ºÆ®°¡ Á¸ÀçÇÑ´Ù¸é,
    if (candidate[0] != '\0')
    {
        //2.1. º¯È¯Áß Ç¥½Ã¸¦ ÇÑ´Ù.
        IMEAdapter* imeAdapter = ((NotepadForm*)(this->parent))->imeAdapter;
        imeAdapter->BeginWaitingForComposition();

        //2.2. IME¾î´ðÅÍ¿¡¼­ ¼³Á¤ÇÑ´Ù.
        imeAdapter->RecordSource(candidate);
        imeAdapter->ApplyConversionOptions(candidate);
    }
}
