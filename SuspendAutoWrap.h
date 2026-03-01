#ifndef _SUSPENDAUTOWRAP_H //guard
#define _SUSPENDAUTOWRAP_H

typedef signed long int Long;

class CWnd;

class SuspendAutoWrap {
public:
	SuspendAutoWrap(CWnd* parent);
	~SuspendAutoWrap();
private:
	CWnd* parent;
	Long viewRowIndex;
};

#endif // !_SUSPENDAUTOWRAP_H

