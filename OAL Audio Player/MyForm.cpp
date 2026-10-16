#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

namespace OALAudioPlayer
{
	[STAThreadAttribute]
	void Main(array<String^>^ args) {
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		OALAudioPlayer::MyForm form;
		Application::Run(% form);
	}
}