#pragma once

#include "App.g.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Core;

namespace FmodExample
{
	ref class App sealed
	{
	private:
		CoreDispatcher^ mDispatcher;
		TextBlock^ mText;
        String^ mContent0;
        String^ mContent1;
        int mCurrentContent;
		static App^ smInstance;
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ pArgs) override;
		void AddText(wchar_t* line);
		void ClearText();

		void Update();

		bool KeyPressed(int btn);
		bool KeyDown(int btn);

		void OnKeyDown(Object^ sender, KeyRoutedEventArgs^ args);
		void OnKeyUp(Object^ sender, KeyRoutedEventArgs^ args);
		void OnButtonClicked(Object^ sender, RoutedEventArgs^ args);

		static App^ Get() { return smInstance; }
	};
}
