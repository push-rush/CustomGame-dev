#include "pch.h"
#include "common.h"

using namespace FmodExample;

using namespace Platform;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::System;
using namespace Windows::System::Threading;

App^ App::smInstance;

struct ButtonInfo
{
	VirtualKey virtualKey;
	bool down;
	bool held;
    bool up;
    bool pressed;
};

static ButtonInfo Buttons[BTN_QUIT + 1] =
{	
	{ VirtualKey::Number1, false, false },		//BTN_ACTION1
	{ VirtualKey::Number2, false, false },		//BTN_ACTION2
	{ VirtualKey::Number3, false, false },		//BTN_ACTION3
	{ VirtualKey::Number4, false, false },		//BTN_ACTION4
	{ VirtualKey::Left, false, false },			//BTN_LEFT
	{ VirtualKey::Right, false, false },		//BTN_RIGHT
	{ VirtualKey::Up, false, false },			//BTN_UP
	{ VirtualKey::Down, false, false },			//BTN_DOWN
	{ VirtualKey::Q, false, false },		//BTN_MORE
	{ VirtualKey::Escape, false, false },		//BTN_QUIT
};

App::App()
{
	InitializeComponent();
	smInstance = this;
    mContent0 = "";
    mContent1 = "";
    mCurrentContent = 0;
}

void App::OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs)
{
	if (pArgs->PreviousExecutionState == ApplicationExecutionState::Running)
	{
		Window::Current->Activate();
		return;
	}

	auto grid = ref new Grid();
	grid->Background = ref new SolidColorBrush(Windows::UI::Colors::CornflowerBlue);

	mText = ref new TextBlock();
	mText->Text = "";
	mText->FontSize = 24;	
	mText->Margin = ThicknessHelper::FromUniformLength(50);

	grid->Children->Append(mText);
	
	auto buttonPanel = ref new StackPanel();
    buttonPanel->Margin = Thickness(30);
    buttonPanel->Orientation = Orientation::Horizontal;
    buttonPanel->HorizontalAlignment = HorizontalAlignment::Center;
    buttonPanel->VerticalAlignment = VerticalAlignment::Bottom;


	for (int i = 0; i < BTN_QUIT + 1; i++)
	{
		wchar_t ws[256];	
		_snwprintf_s(ws, 256, L"%S", Common_BtnStr((Common_Button)i));
		auto button = ref new Button();
		button->Content = ref new String(ws);
		button->Click += ref new RoutedEventHandler(this, &App::OnButtonClicked);
		button->Tag = i;
		button->Padding = Thickness(30);
		button->Margin = Thickness(10);
		buttonPanel->Children->Append(button);
	}

	grid->Children->Append(buttonPanel);
	grid->SetRow(buttonPanel, 1);


	auto page = ref new Page();
	page->Content = grid;

	page->KeyDown += ref new KeyEventHandler(this, &App::OnKeyDown);
	page->KeyUp += ref new KeyEventHandler(this, &App::OnKeyUp);

	
	page->Visibility = Visibility::Visible;
	
	Window::Current->Content = page;
	Window::Current->Activate();

    mDispatcher = Window::Current->Dispatcher;

    mDispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent); 
	
	IAsyncAction^ async = ThreadPool::RunAsync(
		ref new WorkItemHandler(
			[](IAsyncAction^ op)
			{
				FMOD_Main();
			}, 
			Platform::CallbackContext::Any), 			
			WorkItemPriority::Normal,
			WorkItemOptions::None);
}

void App::AddText(wchar_t* line)
{
    String^ newline = ref new String(line);
    if (mCurrentContent)
    {
        mContent1 += newline;
    }
    else
    {
        mContent0 += newline;
    }
}

void App::ClearText()
{
    int display = mCurrentContent;
    if (mCurrentContent)
    {
        mContent0 = "";
        mCurrentContent = 0;
    }
    else
    {
        mContent1 = "";
        mCurrentContent = 1;
    }
    mDispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, display]() { mText->Text = display ? mContent1 : mContent0; }));
}

void App::Update()
{		
	for (int i = 0; i <= BTN_QUIT; i++)
	{
        if (!Buttons[i].pressed)
        {
            if (Buttons[i].down)
            {
                Buttons[i].pressed = true;
            }
        }
        else
        {
            Buttons[i].pressed = false;
            Buttons[i].down = false;
        }
    }

}

bool App::KeyPressed(int btn)
{
	return Buttons[btn].pressed;
}

bool App::KeyDown(int btn)
{
	return Buttons[btn].pressed || Buttons[btn].held;
}


void App::OnKeyDown(Object^ sender, KeyRoutedEventArgs^ args)
{
	for (int i = 0; i <= BTN_QUIT; i++)
	{
		if (args->Key == Buttons[i].virtualKey)
		{
			if (args->KeyStatus.WasKeyDown)
			{
				Buttons[i].held = true;
			}
			else
			{
				Buttons[i].down = true;
			}
		}
	}
}

void App::OnKeyUp(Object^ sender, KeyRoutedEventArgs^ args)
{
	for (int i = 0; i <= BTN_QUIT; i++)
	{
		if (args->Key == Buttons[i].virtualKey)
		{
			Buttons[i].up = true;
            Buttons[i].held = false;
		}
	}
}

void App::OnButtonClicked(Object^ sender, RoutedEventArgs^ args)
{
	Buttons[(int)((Button^)sender)->Tag].down = true;
}

