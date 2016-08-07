/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SplitterWidget.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
************************************************************************/


#ifndef _SPLITTERWIDGET_H_
#define _SPLITTERWIDGET_H_

#include <wx/splitter.h>
#include "Commands.h"

/*************************************************//**
*
* \brief base splitter class adding support for several events
*
******************************************************/
class SplitterWidget : public wxSplitterWindow, public KeyEventObserver
{
	DECLARE_CLASS( SplitterWidget )
public:
	SplitterWidget( wxWindow *parent, const wxWindowID = -1, const wxPoint &pos = wxDefaultPosition,
		const wxSize size = wxDefaultSize, long style = wxSP_3D  );

	~SplitterWidget();

protected:
	void onMouseRight( wxMouseEvent& );
	void OnSize( wxSizeEvent& );
	void OnPositionChanged( wxSplitterEvent& );
	void OnSplitterDoubleClick(wxSplitterEvent& );
	void OnSplitterSashPosChanged(wxSplitterEvent& );
	void OnKeyDown( wxKeyEvent& );
	void OnSetFocus( wxFocusEvent& );

};

#endif

