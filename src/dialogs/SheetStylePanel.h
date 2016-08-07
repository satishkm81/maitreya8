/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/SheetStylePanel.h
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

#ifndef SHEETSTYLEPANEL_H_
#define SHEETSTYLEPANEL_H_

#include "ConfigPanel.h"
#include<vector>

using namespace std;

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxButton;
class wxFlexGridSizer;
class wxNotebook;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

class ChartProperties;
class SheetWidget;
class Horoscope;
class SheetConfig;

/*************************************************//**
*
* \brief ConfigPanel for color parameters
*
******************************************************/
class SheetStylePanel: public ConfigPanel
{
	DECLARE_CLASS( SheetStylePanel )

public:
    // begin wxGlade: SheetStylePanel::ids
    // end wxGlade

	SheetStylePanel( wxWindow* );
	~SheetStylePanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: SheetStylePanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnSelectStyle ( wxCommandEvent& );
	void model2gui( const uint &sel );
	void OnManageStyles( wxCommandEvent& );
	void populateStyleChoice();
	SheetConfig *getSheetConfig( const int& );

	void OnButtonBrush( wxCommandEvent& );
	void OnButtonPen( wxCommandEvent& );
	void OnCommand( wxCommandEvent& );

	void OnTableColorPickerEvent( wxColourPickerEvent& );

	virtual void updateUi();

	ChartProperties *props;
	Horoscope *horoscope;
	int selected;
	vector<SheetConfig*> v;

protected:
    // begin wxGlade: SheetStylePanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_shadows_staticbox;
    wxStaticBox* sizer_table_border_staticbox;
    wxStaticBox* sizer_table_cells_staticbox;
    wxStaticBox* sizer_table_headers_staticbox;
    wxStaticBox* sizer_selected_staticbox;
    wxStaticBox* sizer_sheetstyle_staticbox;
    wxStaticText* label_style;
    wxChoice* choice_styles;
    wxButton* button_manage_styles;
    wxStaticLine* static_line_1;
    wxStaticText* label_sheet_textcolor;
    wxColourPickerCtrl* button_sheet_textcolor;
    wxStaticText* label_sheet_brush;
    wxButton* button_sheet_brush;
    wxStaticText* label_selection_mode;
    wxChoice* choice_selection_mode;
    wxStaticText* label_selected_brush;
    wxButton* button_selected_brush;
    wxPanel* notebook_pane_sheet;
    wxStaticLine* static_line_2;
    wxCheckBox* check_use_header_colors;
    wxStaticText* label_header_textcolor;
    wxColourPickerCtrl* button_header_textcolor;
    wxStaticText* label_header_brush;
    wxButton* button_header_brush;
    wxCheckBox* check_use_cell_colors;
    wxStaticText* label_cell_textcolor;
    wxColourPickerCtrl* button_cell_textcolor;
    wxStaticText* label_cell_bgmode;
    wxChoice* choice_cell_bgmode;
    wxStaticText* label_all_row_bgcolor;
    wxButton* button_all_cell_brush;
    wxStaticText* label_even_row_bgcolor;
    wxButton* button_even_cell_brush;
    wxStaticText* label_odd_row_bgcolor;
    wxButton* button_odd_cell_brush;
    wxPanel* notebook_pane_table;
    wxStaticLine* static_line_3;
    wxCheckBox* check_draw_border;
    wxStaticText* label_outer_border;
    wxChoice* choice_outer_border;
    wxButton* button_outer_border_pen;
    wxStaticText* label_header_border;
    wxChoice* choice_header_border;
    wxButton* button_header_border_pen;
    wxStaticText* label_cell_border;
    wxChoice* choice_cell_border;
    wxButton* button_cell_border_pen;
    wxCheckBox* check_draw_shadow;
    wxStaticText* label_cell_bgcolor_copy;
    wxSpinCtrl* spin_shadow_width;
    wxStaticText* label_cell_bgcolor_copy_1;
    wxButton* button_shadow_brush;
    wxPanel* notebook_pane_border;
    wxNotebook* notebook_style;
    wxFlexGridSizer* grid_tablestyles;
    SheetWidget* preview;
    wxPanel* panel_preview;
    // end wxGlade
}; // wxGlade: end class


#endif // SHEETSTYLEPANEL_H_
