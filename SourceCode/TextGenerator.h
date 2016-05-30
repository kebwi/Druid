#ifndef __TEXT_GENERATOR__#define __TEXT_GENERATOR__#include "FloatPoint.h"#include <ATSUnicode.h>#include <vector.h>class TextGenerator{	public:		TextGenerator();		~TextGenerator();				void SelectAndGetFont(MenuRef theMenu, MenuItemIndex theItem);		void SetFontSize(Fixed newFontSize);				bool GetFilterDegenerates();		bool GetGenerateBSplines();				void AddNewControlPointSet();		vector<FloatPoint>* GetPresentGlyphControlPoints();		vector<vector<FloatPoint> > GetGlyphControlPoints();				void ClearGlyphControlPoints();				void RunDialog();				static pascal OSStatus QuadraticLineProc(const Float32Point *pt1, const Float32Point *pt2, void *callBackDataPtr);		static pascal OSStatus QuadraticCurveProc(const Float32Point *pt1, const Float32Point *controlPt, const Float32Point *pt2, void *callBackDataPtr);		static pascal OSStatus QuadraticNewPathProc(void * callBackDataPtr);		static pascal OSStatus QuadraticClosePathProc(void * callBackDataPtr);				void RenderGlyphs(bool ignoreDialogVisibility = false);				enum {		    kMenuCheckMark                      = 0x0012,       // This corresponds to a checkmark in the keyboard font		    kMenuDashMark                       = 0x002D,       // This is just a normal hyphen/minus character		    kMenuNoMark                         = 0x0000        // Null character -- shows up as no mark in a menu		};			private:		enum {			kATSDeletedGlyphcode = 0xFFFF		};				enum {			kATSGlyphInfoAppleReserved    = 0x1FFBFFE8,			kATSGlyphInfoIsAttachment     = (unsigned long)0x80000000,			kATSGlyphInfoIsLTHanger       = 0x40000000,			kATSGlyphInfoIsRBHanger       = 0x20000000,			kATSGlyphInfoTerminatorGlyph  = 0x00080000,			kATSGlyphInfoIsWhiteSpace     = 0x00040000,			kATSGlyphInfoHasImposedWidth  = 0x00000010,			kATSGlyphInfoByteSizeMask     = 0x00000007		};		MenuID					fontMenuID;		MenuRef					fontMenuCurrentMenuRef;		MenuItemIndex			fontMenuCurrentMenuItem;		MenuItemIndex			*fontMenuHierarchicalItems;		ItemCount				numHierarchicalItems;		UInt32					currentFontSizeCommandID;				FMFont                  font;		Fixed					fontSize;				ATSUTextLayout			layout;		ATSUStyle				style;		UniChar					*textString;		UniCharCount			textLength;				struct CurveCallbackData		{		    Float32Point		origin;				// The origin of the current glyph		    float				windowHeight;		// Height of the window (used to flip the y-coordinate for CG drawing)		    Boolean				first;				// Keeps track of which segment is first in a glyph		    Float32Point		current;			// The current pen position (used to filter degenerate cases)		};				struct GlyphRecord		{		    ATSGlyphRef			glyphID;			// The glyphID.  This is simply an index into a table in the font.		    Float32Point		relativeOrigin;		// The origin of this glyph -- relative to the origin of the line.		};				bool filterDegenerates;		bool generateBSplines;				WindowRef window;		bool cancel;				static EventLoopTimerRef sUpdateTextTimer;				vector<vector<FloatPoint> > glyphControlPoints;				OSStatus InstallFontMenu(MenuID menuID);		MenuItemIndex GetFontMenuParentItem(MenuRef inMenu);		Boolean FindAndSelectFont(FMFont iFont);		void BuildFontMenuParentItemArray();				void UpdateATSUIStyle();				void RedrawEditText();		void UniCharToStr255(UniChar* uniCharStr, Str255& str255);		void UniCharToCstring(UniChar* uniCharStr, char* cStr);		void Str255ToUniChar(Str255 str255, UniChar*& uniCharStr);		void CstringToUniChar(char* cStr, UniChar*& uniCharStr);				void GetGlyphIDsAndPositions(ATSUTextLayout iLayout, UniCharArrayOffset iStart, UniCharCount iLength, GlyphRecord **oGlyphRecordArray, ItemCount *oNumGlyphs);				static pascal OSStatus TextGeneratorHandler(EventHandlerCallRef nextHandler, EventRef theEvent, void* userData);		static pascal void EditTextValidationProc(ControlRef control);		static pascal ControlKeyFilterResult ControlKeyFilterProc(ControlRef theControl, SInt16 *keyCode, SInt16 *charCode, EventModifiers *modifiers);				static pascal void UpdateTextTimerHandler(EventLoopTimerRef theTimer, void* userData);};#endif