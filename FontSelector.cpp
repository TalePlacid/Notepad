#include "FontSelector.h"

#pragma warning(disable:4996)

FontSelector::FontSelector() {

}

FontSelector::~FontSelector() {

}

LOGFONT FontSelector::SelectBaseLogFont(CWnd* owner) {
	LOGFONT logFont = { 0, };
	CClientDC dc(owner);

	//±‚∫ª ∆˘∆Æ(∏º¿∫ ∞ÌµÒ, 10pt, ∫∏≈Î Ω∫≈∏¿œ)
	logFont.lfHeight = -MulDiv(10, dc.GetDeviceCaps(LOGPIXELSY), 72);
	logFont.lfWeight = FW_NORMAL;
	logFont.lfCharSet = HANGUL_CHARSET;
	strcpy_s(logFont.lfFaceName, "∏º¿∫ ∞ÌµÒ");

	return logFont;
}

LOGFONT FontSelector::SelectScaledLogFont(LOGFONT baseLogFont, double magnification) {
	double round = 0.5;
	if (baseLogFont.lfHeight < 0)
	{
		round = -0.5;
	}

	baseLogFont.lfHeight = baseLogFont.lfHeight * magnification + round;

	return baseLogFont;
}

