#ifndef _CONVERTTOIMECHARACTER_H //guard
#define _CONVERTTOIMECHARACTER_H
#include "Command.h"

typedef signed long int Long;

class CWnd;

class ConvertToIMECharacterCommand : public Command {
public:
	ConvertToIMECharacterCommand(CWnd* parent, const char(*character));
	virtual ~ConvertToIMECharacterCommand();
	ConvertToIMECharacterCommand(const ConvertToIMECharacterCommand& source);
	ConvertToIMECharacterCommand& operator=(const ConvertToIMECharacterCommand& source);

	virtual void Execute();
	virtual void Undo();
	virtual void Redo();

	virtual Command* Clone();
	virtual AppID GetID();
private:
	char source[2];
	char replaced[2];
	Long offset;
	Long columnIndex;
};

inline AppID ConvertToIMECharacterCommand::GetID() {
	return AppID::ID_COMMAND_CONVERT_TO_IME_CHARACTER;
}

#endif // !_CONVERTTOIMECHARACTER_H
