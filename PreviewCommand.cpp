#include "PreviewCommand.h"
#include "PreviewForm.h"

#pragma warning(disable:4996)

PreviewCommand::PreviewCommand(CWnd* parent)
	:Command(parent) {

}

PreviewCommand::~PreviewCommand() {

}

void PreviewCommand::Execute() {
	PreviewForm previewForm(this->parent);
	previewForm.DoModal();
}