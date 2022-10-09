#pragma once
#include <Windows.h>

class Window
{
public:
    //Initialize the window
	Window();
	
	
	
	//return running state of the window
	bool IsRun();

	RECT GetClientWindowRect();
	RECT GetScreenSize();


	//Events
	virtual void OnCreate() ;
    virtual void OnUpdate() ;
    virtual void OnDestroy();
	virtual void OnFocus();
	virtual void OnKillFocus();
	virtual void OnSize();

	

     //Release the window
	~Window();

protected:

    
	HWND m_hwnd;
	bool m_is_run;
	bool m_is_init = false;

private:

    bool broadcast();

};
