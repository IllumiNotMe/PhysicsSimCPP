#pragma once

using namespace System::Windows::Controls;
using namespace System::Windows::Documents;

ref class ConsoleHandler
{
public:
	ConsoleHandler(TextBox ^);
	void Write(System::String ^);

	void WriteLine(System::String ^);
	void WriteLine(double num) { WriteLine(num.ToString()); }
	void WriteLine(int num) { WriteLine(num.ToString()); }

private:
	TextBox ^ textBox;
	FlowDocument ^ document;
	Paragraph ^ mainParagraph;
};

