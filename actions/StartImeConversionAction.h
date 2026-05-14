#ifndef _START_IME_CONVERSION_ACTION_H //guard
#define _START_IME_CONVERSION_ACTION_H
#include <afxwin.h>
#include "Action.h"

class StartImeConversionAction : public Action {
public:
    StartImeConversionAction(CWnd* parent);
    virtual ~StartImeConversionAction();

    virtual void Perform();
    virtual bool NeedScrollBarUpdate();
    virtual bool ShouldKeepSelection();
};

inline bool StartImeConversionAction::NeedScrollBarUpdate() {
    return false;
}

inline bool StartImeConversionAction::ShouldKeepSelection() {
    return true;
}

#endif // !_START_IME_CONVERSION_ACTION_H