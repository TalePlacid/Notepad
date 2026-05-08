#ifndef _START_KOREAN_IME_CONVERSION_ACTION_H //guard
#define _START_KOREAN_IME_CONVERSION_ACTION_H
#include <afxwin.h>
#include "Action.h"

class StartKoreanImeConversionAction : public Action {
public:
    StartKoreanImeConversionAction(CWnd* parent);
    virtual ~StartKoreanImeConversionAction();

    virtual void Perform();
    virtual bool NeedScrollBarUpdate();
    virtual bool ShouldKeepSelection();
};

inline bool StartKoreanImeConversionAction::NeedScrollBarUpdate() {
    return false;
}

inline bool StartKoreanImeConversionAction::ShouldKeepSelection() {
    return true;
}

#endif // !_START_KOREAN_IME_CONVERSION_ACTION_H