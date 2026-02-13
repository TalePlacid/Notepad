#ifndef _GLYPH_H //guard
#define _GLYPH_H
#include <string>
using namespace std;
#include "Position.h"

typedef signed long int Long;

class Visitor;

class Glyph {
public:
	Glyph();
	virtual ~Glyph() = 0;
	Glyph(const Glyph& source);
	Glyph& operator=(const Glyph& source);

	virtual Long Add(Glyph* glyph);
	virtual Long Add(Long index, Glyph* glyph);
	virtual Glyph* GetAt(Long index);
	virtual Glyph* Clone() = 0;
	virtual string MakeString() = 0;
	virtual Long Remove();
	virtual Long Remove(Long index);
	virtual Long Replace(Long index, Glyph* glyph);
	virtual Long First();
	virtual Long Previous();
	virtual Long Next();
	virtual Long Last();
	virtual Long Move(Long index);
	virtual bool IsDummyRow();
	virtual bool IsWordCharacter();
	virtual bool IsMultiByteCharacter();
	virtual void Accept(Visitor& visitor);
	virtual bool Select(bool isSelecting);
	virtual Long ToggleSelection();
	virtual Long ToggleSelection(Long start, Long end);
	virtual Long TruncateAfter(Long index);
	virtual Long TruncateBefore(Long index);
	virtual Long MergeRows(Long index);
	virtual Long SplitRows(Long rowIndex, Long columnIndex, bool IsDummyRow = false);
	virtual Long SelectRange(Long start, Long end, bool isSelecting = true);
	virtual Long AppendFromFront(const Glyph* ohter);
	virtual Long AppendFromRear(const Glyph* other);
	virtual bool IsAboveTopLine(Long index);
	virtual bool IsBelowBottomLine(Long index);
	virtual Long FindPreviousWordStart(Long columnIndex);
	virtual Long FindNextWordStart(Long columnIndex);

	virtual Glyph* operator[](Long index);
	virtual operator char*() const;

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;
	virtual Long GetCurrent() const;
	virtual bool IsSelected() const;
protected:
	bool isSelected;
};

inline Long Glyph::GetCapacity() const {
	return 0;
}

inline Long Glyph::GetLength() const {
	return 0;
}

inline Long Glyph::GetCurrent() const {
	return -1;
}

inline bool Glyph::IsSelected() const {
	return this->isSelected;
}

inline bool Glyph::IsAboveTopLine(Long index) {
	return false;
}

inline bool Glyph::IsBelowBottomLine(Long index) {
	return false;
}

#endif // !_GLYPH_H
