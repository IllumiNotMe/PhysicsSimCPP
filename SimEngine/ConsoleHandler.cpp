#include "stdafx.h"
#include "ConsoleHandler.h"


ConsoleHandler::ConsoleHandler(TextBox ^ console) :
	textBox(console)
{
	document = gcnew FlowDocument();
	mainParagraph = gcnew Paragraph();

	document->Blocks->Add(mainParagraph);
}

void ConsoleHandler::Write(System::String ^ strOut)
{
	textBox->AppendText(strOut);
	textBox->ScrollToLine(textBox->LineCount - 1);
}

void ConsoleHandler::WriteLine(System::String ^ strOut)
{
	Write(strOut + "\n");
}