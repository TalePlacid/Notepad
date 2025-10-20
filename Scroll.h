#ifndef _SCROLL_H //guard
#define _SCROLL_H

typedef signed long int Long;

class Scroll {
public:
	Scroll();
	Scroll(Long min, Long max, Long page, Long pos);
	~Scroll();
	Scroll(const Scroll& source);
	Scroll& operator=(const Scroll& source);

	void ResizeRange(Long max, Long min = 0);
	Long ResizePage(Long page);
	Long Move(Long pos);
	Long LineUp(Long distance);
	Long LineDown(Long distance);
	Long PageUp();
	Long PageDown(); 

	Long GetMin() const;
	Long GetMax() const;
	Long GetPage() const;
	Long GetPos() const;
private:
	Long min;
	Long max;
	Long page;
	Long pos;
};

inline Long Scroll::GetMin() const {
	return this->min;
}

inline Long Scroll::GetMax() const {
	return this->max;
}

inline Long Scroll::GetPage() const {
	return this->page;
}

inline Long Scroll::GetPos() const {
	return this->pos;
}
#endif // !_SCROLL_H
