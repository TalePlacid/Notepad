#ifndef _GLYPHFACTORY_H //guard
#define _GLYPHFACTORY_H

typedef signed long int Long;

class Glyph;

class GlyphFactory {
public:
	GlyphFactory();
	virtual ~GlyphFactory();

	virtual Glyph* Create(char(*content), bool isDirty = true);
};

#endif // !_GLYPHFACTORY_H
