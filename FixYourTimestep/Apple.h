// Simple Apple OpenGL framework

#ifdef __APPLE__

#include <Carbon/Carbon.h>
#include <AGL/agl.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

static AGLContext setupAGL(WindowRef window)
{	
	if ((Ptr) kUnresolvedCFragSymbolAddress == (Ptr) aglChoosePixelFormat)
		return 0;
	
	GLint attributes[] = { AGL_RGBA, AGL_DOUBLEBUFFER, AGL_DEPTH_SIZE, 16, AGL_NONE };	
	
	AGLPixelFormat format = aglChoosePixelFormat(NULL, 0, attributes);
	if (!format) 
		return 0;
	
	AGLContext context = aglCreateContext(format, 0);
	if (!context)
		return 0;

	aglDestroyPixelFormat(format);

	aglSetDrawable(context, GetWindowPort(window));
	
	aglSetCurrentContext(context);
	
	return context;
}

static void cleanupAGL(AGLContext context)
{
	glFlush();
	aglSetDrawable(context, 0);
	aglSetCurrentContext(0);
	aglDestroyContext(context);
}

WindowRef window;
AGLContext context;

static pascal OSErr quitEventHandler(const AppleEvent *appleEvt, AppleEvent *reply, SInt32 refcon)
{
	onQuit();
	return false;
}

bool openDisplay(const char title[], int width, int height)
{
	// create window
	
	Rect rect;
	rect.top = 100;
	rect.left = 100;
	rect.bottom = rect.top + height;
	rect.right = rect.left + width;
	
	// todo: center window and adjust client area to match width/height
	
	OSErr result = CreateNewWindow( kDocumentWindowClass, 
					                (kWindowStandardDocumentAttributes | 
									 kWindowStandardHandlerAttribute) &
									~kWindowResizableAttribute,
					                &rect, &window);
	
	if (result!=noErr)
		return false;

	// set window title

	SetWindowTitleWithCFString(window, CFStringCreateWithCString(0, title, CFStringGetSystemEncoding()));

	// initialize apple opengl
	
	context = setupAGL(window);

	if (!context)
		return false;
	
	// show the window
	
	ShowWindow(window);
		
	// install quit event handler
		
	AEInstallEventHandler(kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(quitEventHandler), 0, false);
	
	return true;
}

void updateDisplay(void)
{
	// process pending events
	
	// todo: process all available events instead of one per update!
	
	EventRecord event;
	
	if (WaitNextEvent(everyEvent, &event, 1, NULL)) 
	{
		switch (event.what) 
		{
			case mouseDown:
			{
				WindowRef window;

				SInt16 part = FindWindow(event.where, &window);
				
				if (window)
					SelectWindow(window);
				
				switch (part) 
				{
					case inMenuBar:
						MenuSelect(event.where);
						break;
				}
			}
			break;
				
			case keyDown:
			case autoKey:
			{
				SInt8 key = event.message & charCodeMask;
 				
				if (key==27)
					onQuit();
			}
			break;
		}
	}
	
	aglSwapBuffers(context);
}

void closeDisplay(void)
{	
    cleanupAGL(context);
	context = 0;
	
	DisposeWindow((WindowPtr) window);
	window = 0;
}

float time()
{
	static UInt64 start = 0;
	
	if (start==0)
	{
		Microseconds((UnsignedWide*)&start);
		return 0.0f;
	}
	
	UInt64 counter = 0;
	Microseconds((UnsignedWide*)&counter);
	return (counter - start) / 1000000.0f;
}

#endif
