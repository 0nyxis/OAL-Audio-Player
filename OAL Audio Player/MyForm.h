#pragma once
#include<msclr\marshal_cppstd.h>
#include "Player.h"
#include <filesystem>

namespace OALAudioPlayer {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Description résumée de MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: ajoutez ici le code du constructeur
			//

			init();
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ ResumePauseButton;
	private: System::Windows::Forms::Button^ StopButton;
	protected:

	protected:

	protected:

	private: System::Windows::Forms::Button^ PlayButton;
	private: System::Windows::Forms::ListBox^ listBox1;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ fileButton;
	private: System::Windows::Forms::ToolStripMenuItem^ openButton;
	private: System::Windows::Forms::ToolStripMenuItem^ saveButton;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog;
	private: System::Windows::Forms::TrackBar^ VolumeTrackbar;

	private: System::Windows::Forms::ToolStripMenuItem^ exitButton;
	private: System::Windows::Forms::ToolStripMenuItem^ openListToolStripMenuItem;
	private: System::Windows::Forms::Label^ VolumeLabel;
	private: System::Windows::Forms::Label^ CurrentLabel;
	private: System::Windows::Forms::Label^ SongPlayingLabel;
	private: System::Windows::Forms::TrackBar^ timePlayingTrackbar;
	private: System::Windows::Forms::Label^ TotalTimeLabel;
	private: System::Windows::Forms::Label^ CurrentTimeLabel;

	private: System::Windows::Forms::Label^ label3;











	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ResumePauseButton = (gcnew System::Windows::Forms::Button());
			this->StopButton = (gcnew System::Windows::Forms::Button());
			this->PlayButton = (gcnew System::Windows::Forms::Button());
			this->listBox1 = (gcnew System::Windows::Forms::ListBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileButton = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openButton = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openListToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveButton = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitButton = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->VolumeTrackbar = (gcnew System::Windows::Forms::TrackBar());
			this->VolumeLabel = (gcnew System::Windows::Forms::Label());
			this->CurrentLabel = (gcnew System::Windows::Forms::Label());
			this->SongPlayingLabel = (gcnew System::Windows::Forms::Label());
			this->timePlayingTrackbar = (gcnew System::Windows::Forms::TrackBar());
			this->TotalTimeLabel = (gcnew System::Windows::Forms::Label());
			this->CurrentTimeLabel = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VolumeTrackbar))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->timePlayingTrackbar))->BeginInit();
			this->SuspendLayout();
			// 
			// ResumePauseButton
			// 
			this->ResumePauseButton->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->ResumePauseButton->Location = System::Drawing::Point(557, 261);
			this->ResumePauseButton->Name = L"ResumePauseButton";
			this->ResumePauseButton->Size = System::Drawing::Size(98, 27);
			this->ResumePauseButton->TabIndex = 0;
			this->ResumePauseButton->Text = L"Resume/Pause";
			this->ResumePauseButton->UseVisualStyleBackColor = true;
			this->ResumePauseButton->Click += gcnew System::EventHandler(this, &MyForm::ResumePauseButton_Click);
			// 
			// StopButton
			// 
			this->StopButton->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->StopButton->Location = System::Drawing::Point(661, 261);
			this->StopButton->Name = L"StopButton";
			this->StopButton->Size = System::Drawing::Size(75, 27);
			this->StopButton->TabIndex = 1;
			this->StopButton->Text = L"Stop";
			this->StopButton->UseVisualStyleBackColor = true;
			this->StopButton->Click += gcnew System::EventHandler(this, &MyForm::StopButton_Click);
			// 
			// PlayButton
			// 
			this->PlayButton->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
			this->PlayButton->Location = System::Drawing::Point(476, 261);
			this->PlayButton->Name = L"PlayButton";
			this->PlayButton->Size = System::Drawing::Size(75, 27);
			this->PlayButton->TabIndex = 2;
			this->PlayButton->Text = L"Play";
			this->PlayButton->UseVisualStyleBackColor = true;
			this->PlayButton->Click += gcnew System::EventHandler(this, &MyForm::PlayButton_Click);
			// 
			// listBox1
			// 
			this->listBox1->FormattingEnabled = true;
			this->listBox1->Location = System::Drawing::Point(12, 24);
			this->listBox1->Name = L"listBox1";
			this->listBox1->Size = System::Drawing::Size(391, 329);
			this->listBox1->TabIndex = 3;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileButton });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(819, 24);
			this->menuStrip1->TabIndex = 4;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileButton
			// 
			this->fileButton->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->openButton,
					this->openListToolStripMenuItem, this->saveButton, this->exitButton
			});
			this->fileButton->Name = L"fileButton";
			this->fileButton->Size = System::Drawing::Size(37, 20);
			this->fileButton->Text = L"File";
			this->fileButton->Click += gcnew System::EventHandler(this, &MyForm::toolStripMenuItem1_Click);
			// 
			// openButton
			// 
			this->openButton->Name = L"openButton";
			this->openButton->Size = System::Drawing::Size(121, 22);
			this->openButton->Text = L"Open";
			this->openButton->Click += gcnew System::EventHandler(this, &MyForm::openButton_Click);
			// 
			// openListToolStripMenuItem
			// 
			this->openListToolStripMenuItem->Name = L"openListToolStripMenuItem";
			this->openListToolStripMenuItem->Size = System::Drawing::Size(121, 22);
			this->openListToolStripMenuItem->Text = L"Open list";
			this->openListToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::openListToolStripMenuItem_Click);
			// 
			// saveButton
			// 
			this->saveButton->Name = L"saveButton";
			this->saveButton->Size = System::Drawing::Size(121, 22);
			this->saveButton->Text = L"Save";
			this->saveButton->Click += gcnew System::EventHandler(this, &MyForm::saveButton_Click);
			// 
			// exitButton
			// 
			this->exitButton->Name = L"exitButton";
			this->exitButton->Size = System::Drawing::Size(121, 22);
			this->exitButton->Text = L"Exit";
			this->exitButton->Click += gcnew System::EventHandler(this, &MyForm::exitButton_Click);
			// 
			// saveFileDialog
			// 
			this->saveFileDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveFileDialog1_FileOk);
			// 
			// VolumeTrackbar
			// 
			this->VolumeTrackbar->Location = System::Drawing::Point(542, 308);
			this->VolumeTrackbar->Maximum = 100;
			this->VolumeTrackbar->Name = L"VolumeTrackbar";
			this->VolumeTrackbar->Size = System::Drawing::Size(163, 45);
			this->VolumeTrackbar->TabIndex = 5;
			this->VolumeTrackbar->Value = 100;
			this->VolumeTrackbar->ValueChanged += gcnew System::EventHandler(this, &MyForm::VolumeChanged);
			// 
			// VolumeLabel
			// 
			this->VolumeLabel->AutoSize = true;
			this->VolumeLabel->Location = System::Drawing::Point(494, 317);
			this->VolumeLabel->Name = L"VolumeLabel";
			this->VolumeLabel->Size = System::Drawing::Size(42, 13);
			this->VolumeLabel->TabIndex = 6;
			this->VolumeLabel->Text = L"Volume";
			// 
			// CurrentLabel
			// 
			this->CurrentLabel->AutoSize = true;
			this->CurrentLabel->Location = System::Drawing::Point(564, 70);
			this->CurrentLabel->Name = L"CurrentLabel";
			this->CurrentLabel->Size = System::Drawing::Size(91, 13);
			this->CurrentLabel->TabIndex = 7;
			this->CurrentLabel->Text = L"Currently Playing :";
			// 
			// SongPlayingLabel
			// 
			this->SongPlayingLabel->AutoSize = true;
			this->SongPlayingLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SongPlayingLabel->Location = System::Drawing::Point(441, 98);
			this->SongPlayingLabel->MaximumSize = System::Drawing::Size(330, 0);
			this->SongPlayingLabel->Name = L"SongPlayingLabel";
			this->SongPlayingLabel->Size = System::Drawing::Size(160, 20);
			this->SongPlayingLabel->TabIndex = 8;
			this->SongPlayingLabel->Text = L"No audio file selected";
			// 
			// timePlayingTrackbar
			// 
			this->timePlayingTrackbar->Location = System::Drawing::Point(512, 198);
			this->timePlayingTrackbar->Maximum = 100;
			this->timePlayingTrackbar->Name = L"timePlayingTrackbar";
			this->timePlayingTrackbar->Size = System::Drawing::Size(257, 45);
			this->timePlayingTrackbar->TabIndex = 9;
			// 
			// TotalTimeLabel
			// 
			this->TotalTimeLabel->AutoSize = true;
			this->TotalTimeLabel->Location = System::Drawing::Point(472, 213);
			this->TotalTimeLabel->Name = L"TotalTimeLabel";
			this->TotalTimeLabel->Size = System::Drawing::Size(34, 13);
			this->TotalTimeLabel->TabIndex = 10;
			this->TotalTimeLabel->Text = L"00:00";
			// 
			// CurrentTimeLabel
			// 
			this->CurrentTimeLabel->AutoSize = true;
			this->CurrentTimeLabel->Location = System::Drawing::Point(414, 213);
			this->CurrentTimeLabel->Name = L"CurrentTimeLabel";
			this->CurrentTimeLabel->Size = System::Drawing::Size(34, 13);
			this->CurrentTimeLabel->TabIndex = 11;
			this->CurrentTimeLabel->Text = L"00:00";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(454, 213);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(12, 13);
			this->label3->TabIndex = 12;
			this->label3->Text = L"/";
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(819, 367);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->CurrentTimeLabel);
			this->Controls->Add(this->TotalTimeLabel);
			this->Controls->Add(this->timePlayingTrackbar);
			this->Controls->Add(this->SongPlayingLabel);
			this->Controls->Add(this->CurrentLabel);
			this->Controls->Add(this->VolumeLabel);
			this->Controls->Add(this->VolumeTrackbar);
			this->Controls->Add(this->listBox1);
			this->Controls->Add(this->PlayButton);
			this->Controls->Add(this->StopButton);
			this->Controls->Add(this->ResumePauseButton);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"OAL Audio Player";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VolumeTrackbar))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->timePlayingTrackbar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void toolStripMenuItem1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void saveFileDialog1_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) 
	{

	}
	private: System::Void openButton_Click(System::Object^ sender, System::EventArgs^ e)
	{
		openFileDialog->Filter = "Audio files|*.wav;*.ogg|WAV files|*.wav|OGG files|*.ogg";

		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ path = openFileDialog->FileName;

			listBox1->Items->Add(path);
		}
	}

	private: System::Void Save(String^ path)
	{
		StreamWriter^ writer = gcnew StreamWriter(path);

		for (int i = 0; i < listBox1->Items->Count; i++)
		{
			writer->WriteLine(listBox1->Items[i]);
		}

		writer->Close();
	}
	private: System::Void exitButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		Application::Exit();
	}
	private: System::Void saveButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		saveFileDialog->Filter = "Text file|*.txt";

		if (saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			Save(saveFileDialog->FileName);
		}
	}
	private: System::Void openListToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		openFileDialog->Filter = "Text file|*.txt";

		if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			String^ path = openFileDialog->FileName;

			StreamReader^ reader = gcnew StreamReader(path);

			listBox1->Items->Clear();

			while (!reader->EndOfStream)
			{
				String^ lineContent = reader->ReadLine();

				listBox1->Items->Add(lineContent);
			}
			reader->Close();
		}
	}
	private: System::Void PlayButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		if (listBox1->SelectedItem)
		{
			std::string text = msclr::interop::marshal_as<std::string>(listBox1->SelectedItem->ToString()->Trim());

			String^ title = Path::GetFileName(listBox1->SelectedItem->ToString());

			this->SongPlayingLabel->Text = title; 

			SetFileType(text);

			if (Play(text.c_str()))
			{
				MessageBox::Show("Impossible to read this audio file.");
			}

			int time = GetTotalTime();
			TotalTimeLabel->Text = msclr::interop::marshal_as<String^>(std::to_string(time / 60) + ":" + std::to_string(time % 60));
			timePlayingTrackbar->Maximum = time;
			//timePlayingTrackbar->Value = 0;
		}
		else
		{
			MessageBox::Show("Please select an item.");
		}
	}
	private: System::Void StopButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		Stop();
	}
	private: System::Void ResumePauseButton_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		ResumePause();
	}

	private: System::Void VolumeChanged(System::Object^ sender, System::EventArgs^ e) 
	{

		VolumeChange(VolumeTrackbar->Value);
	}
};
}
