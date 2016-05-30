#include <memory>#include "GWorldWrapper.h"//******************************************************************************//Extern Globals//******************************************************************************//Global DeclarationsPaletteHandle gSrcPalette;//******************************************************************************//Static InitializationsGDHandle GWorldWrapper::sOldGD = NULL;GWorldPtr GWorldWrapper::sOldGW = NULL;//******************************************************************************//Function PrototypesGWorldWrapper::GWorldWrapper(int newColorDepth, Rect theDim, int newColorTableId){	dim = theDim;	colorDepth = newColorDepth;	colorTableId = newColorTableId;		colorTable = colorTableId != -1 ? GetCTable(colorTableId) : NULL;		QDErr err = NewGWorld(&theGWorld, colorDepth, &dim, colorTable, NULL, 0L);	if (err == -108)	//Out of memory		throw std::bad_alloc();		StartUsingGWorld();		theSrcPixMap = GetGWorldPixMap(theGWorld);	theSrcRowBytes = (**theSrcPixMap).rowBytes & 0x3FFF;	/*	try	{		theYLookUp = new int[dim.bottom - dim.top];	}	catch (...)	{		throw;	}	for (int i = 0; i < dim.bottom - dim.top; i++)	//Create the lookup table for jumping directly to a given row.		theYLookUp[i] = i * theSrcRowBytes;	*/	FinishUsingGWorld();		theBitHandle = (BitMapHandle)GetGWorldPixMap(theGWorld);		EraseGWorldRect();	//CopyWorldBits(theWindow->portRect);}GWorldWrapper::~GWorldWrapper(){	DisposeGWorld(theGWorld);	//delete [] theYLookUp;}void GWorldWrapper::SetGWorldWrapperWorld(){	SetGWorld(theGWorld, NULL);}void GWorldWrapper::StartUsingGWorld(){	GetGWorld(&sOldGW, &sOldGD);	bool result = LockPixels(GetGWorldPixMap(theGWorld));	if (!result)	{		SysBeep(1);				//CTabHandle colorTable = colorTableId != -1 ? GetCTable(colorTableId) : NULL;		//UpdateGWorld(&theGWorld, colorDepth, &dim, colorTable, NULL, 0L);	}	SetGWorld(theGWorld, NULL);}void GWorldWrapper::FinishUsingGWorld(){	SetGWorld(sOldGW, sOldGD);	UnlockPixels(GetGWorldPixMap(theGWorld));}//Erase the entire gWorld, provided one existsvoid GWorldWrapper::EraseGWorldRect(){	StartUsingGWorld();		BackColor(whiteColor);	Rect r;	GetPortBounds(theGWorld, &r);	EraseRect(&r);		FinishUsingGWorld();}//Blit gWorld pixels to the screen (only within the confines of whereRectPtr)void GWorldWrapper::CopyWorldBits(Rect whereRectPtr, GrafPtr theWindow){	//GrafPort* oldPortPtr;	//GetPort(&oldPortPtr);	CGrafPtr savedPort; 	GDHandle savedDevice;	GetGWorld(&savedPort, &savedDevice);		SetPort(theWindow);		LockPixels(GetGWorldPixMap(theGWorld));	Rect r;	GetPortBounds(theGWorld, &r);	const BitMap* bm = GetPortBitMapForCopyBits(theWindow);	CopyBits(*theBitHandle, bm, &r, &whereRectPtr, ditherCopy, 0);	UnlockPixels(GetGWorldPixMap(theGWorld));		//SetPort(oldPortPtr);	SetGWorld(savedPort, savedDevice);}void GWorldWrapper::CopyWorldBits(Rect whereRectPtr){	LockPixels(GetGWorldPixMap(theGWorld));	Rect r;	GetPortBounds(theGWorld, &r);	GrafPtr port;	GetPort(&port);	const BitMap* bm = GetPortBitMapForCopyBits(port);	CopyBits(*theBitHandle, bm, &r, &whereRectPtr, ditherCopy, 0);	UnlockPixels(GetGWorldPixMap(theGWorld));}void GWorldWrapper::CopyWorldBits(Rect srcRectPtr, Rect destRectPtr, GrafPtr theWindow, int transferMode){	//GrafPort* oldPortPtr;	//GetPort(&oldPortPtr);	CGrafPtr savedPort; 	GDHandle savedDevice;	GetGWorld(&savedPort, &savedDevice);		SetPort(theWindow);		LockPixels(GetGWorldPixMap(theGWorld));	const BitMap* bm = GetPortBitMapForCopyBits(theWindow);	CopyBits(*theBitHandle, bm, &srcRectPtr, &destRectPtr, transferMode, 0);	UnlockPixels(GetGWorldPixMap(theGWorld));		//SetPort(oldPortPtr);	SetGWorld(savedPort, savedDevice);}void GWorldWrapper::UpdateGWorldFromRect(Rect newDim){	if (newDim.left == dim.left && newDim.top == dim.top && newDim.right == dim.right && newDim.bottom == dim.bottom)		return;		CTabHandle colorTable = colorTableId != -1 ? GetCTable(colorTableId) : NULL;	UpdateGWorld(&theGWorld, colorDepth, &newDim, colorTable, NULL, 0L);		StartUsingGWorld();		theSrcPixMap = GetGWorldPixMap(theGWorld);	theSrcRowBytes = (**theSrcPixMap).rowBytes & 0x3FFF;		//theYLookUp = new int[theWindow->portRect.bottom - theWindow->portRect.top];	//for (int i = 0; i < theWindow->portRect.bottom - theWindow->portRect.top; i++)	//Create the lookup table for jumping directly to a given row.	//	theYLookUp[i] = i * theSrcRowBytes;			FinishUsingGWorld();		theBitHandle = (BitMapHandle)GetGWorldPixMap(theGWorld);		EraseGWorldRect();		dim = newDim;}/*void GWorldWrapper::UpdateGWorldFromWindow(){	UpdateGWorldFromRect(theWindow->portRect);	CopyWorldBits(theWindow->portRect);}void GWorldWrapper::UpdateGWorldPalette(int colorTableId){	colorTable = GetCTable(colorTableId);	UpdateGWorld(&theGWorld, colorDepth, &theWindow->portRect, colorTable, NULL, 0L);	CopyWorldBits(theWindow->portRect);}void GWorldWrapper::AnimateColorTable(bool animateUp, bool fast){	int inc = 1;	if (fast)		inc = 5;		RGBColor firstColors[5];	if (animateUp)	{		for (int i = 0; i < inc; i++)			firstColors[i] = (*colorTable)->ctTable[i].rgb;		for (int i = 0; i < 256 - inc; i++)			(*colorTable)->ctTable[i].rgb = (*colorTable)->ctTable[i + inc].rgb;		for (int i = 256 - inc; i < 256; i++)			(*colorTable)->ctTable[i].rgb = firstColors[i - (256 - inc)];	}	else	{		for (int i = inc - 1; i >= 0; i--)			firstColors[i] = (*colorTable)->ctTable[255 - i].rgb;		for (int i = 255; i >= inc; i--)			(*colorTable)->ctTable[i].rgb = (*colorTable)->ctTable[i - inc].rgb;		for (int i = inc - 1; i >= 0; i--)			(*colorTable)->ctTable[i].rgb = firstColors[inc - 1 - i];	}		CTabChanged(colorTable);	UpdateGWorld(&theGWorld, colorDepth, &theWindow->portRect, colorTable, NULL, 0L);	CopyWorldBits(theWindow->portRect);}*/