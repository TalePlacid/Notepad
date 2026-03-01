#ifndef _FILESTREAMADAPTER_H //guard
#define _FILESTREAMADAPTER_H

typedef signed long int Long;

class FileStreamAdapter {
public:
	FileStreamAdapter();
	~FileStreamAdapter();

	Long Read(const char(*path), char* (*bytes), Long& count);
	void Write(const char(*path), char(*bytes), Long count);
};

#endif // !_FILESTREAMADAPTER_H

