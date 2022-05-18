#pragma once
#include <Windows.h>
class Window
{
public:
	Window();
	//Initialize The Window
	bool init();
	//Pick the even message sof the window from the operating system and dispatch them to the window procedure we defined
	bool broadcast();
	//Release The Window
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	//Virtual keyword is highly suggested, as it will ensure anything is tidied up on destroy
	virtual ~Window();


	//EVENTS
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy() = 0;


protected:
	HWND m_hwnd;
	bool m_is_run;
};

