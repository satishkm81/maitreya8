/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfTools.cpp
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

#include "PdfTools.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "FileConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "PdfBase.h"
#include "PdfPainter.h"
#include "PrintoutConfig.h"
#include "PrintoutSheetCreator.h"
#include "SheetConfig.h"
#include "SheetWriter.h"

#include <wx/choicdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/log.h>
#ifdef __WXMSW__
#include <wx/mimetype.h>
#endif

extern Config *config;

class PdfSheetWriter : public GenericSheetWriter
{
	DECLARE_CLASS( PdfSheetWriter )
public:

	/*****************************************************
	**
	**   PdfSheetWriter   ---   Constructor
	**
	******************************************************/
	PdfSheetWriter( Sheet *sheet, SheetConfig *sheetcfg, WriterConfig *writercfg )
		: GenericSheetWriter( sheet, sheetcfg, writercfg, config->colors )
	{
		pcfg = config->print;
		Horoscope *h = new Horoscope();
		pdfdoc = new BasePdfDocument( h, pcfg );

		//const double pw = pdf->GetPageWidth() - pdf->GetLeftMargin() - pdf->GetRightMargin();
		printf( "PDF width %f left %f right %f\n", pdfdoc->GetPageWidth(), pdfdoc->GetLeftMargin(), pdfdoc->GetRightMargin());

		table_cell_delta_x = 7;
		//table_cell_delta_y = 7;
		table_cell_delta_y = 4;
		table_widget_extra_y = 20;
		table_cell_align_delta = 3;
		table_empty_col_width = 10;

		doCenterAll = true;
		//doCenterAll = false;
	}

	/*****************************************************
	**
	**   PdfSheetWriter   ---   Destructor
	**
	******************************************************/
	~PdfSheetWriter()
	{
		delete pdfdoc;
	}

	/*****************************************************
	**
	**   PdfSheetWriter   ---   doPrint
	**
	******************************************************/
	bool doPrint( wxString filename )
	{
		bool ret = true;

		if ( filename.IsEmpty())
		{
			filename = wxFileName::CreateTempFileName( wxT( "testpage" ));
		}

		pdfdoc->SetCompression( pcfg->compressPdf );
		pdfdoc->AddPage();

		printf( "START PdfSheetWriter::doPrint\n" );

		contentRect.x = pdfdoc->GetLeftMargin();
		contentRect.y = pdfdoc->GetTopMargin();
		contentRect.width = pdfdoc->GetPageWidth() - pdfdoc->GetLeftMargin() - pdfdoc->GetRightMargin();
		contentRect.height = 100000;

		Painter *painter = new PdfPainter( pdfdoc );

		preformat( painter, contentRect.width );
		drawSheet( painter, contentRect );
		delete painter;

		pdfdoc->SaveAsFile( filename );

		printf( "ENDE PdfSheetWriter::doPrint\n" );
		return ret;
	}

protected:

	/*****************************************************
	**
	**   PdfSheetWriter   ---   handlePageBreak
	**
	******************************************************/
	void handlePageBreak( SheetItem *item0 )
	{
		printf( "PdfSheetWriter::handlePageBreak\n" );
		if ( item0->rect.y + item0->rect.height > pdfdoc->GetPageHeight() - pdfdoc->GetTopMargin() )
		{
			bool dobreak = true;

			// try to decrease size
			//if ( item0->type == WiWidget )
			if ( false )
			{
				SheetWidgetItem *wi = (SheetWidgetItem*)item0;

				const double oversize = item0->rect.y + item0->rect.height - pdfdoc->GetPageHeight() + pdfdoc->GetTopMargin();
				const double osp = oversize / item0->rect.height;
				printf( "Try to decrease widget item height is %f oversize is %f percentage %f\n", item0->rect.height, oversize, osp * 100.0 );
				if ( osp < wi->minDecreaseRatio )
				{
					printf( "(1) item oversize -> shift wi->rect.height %fi wi->rect.width %f\n", wi->rect.height, wi->rect.width );
					const double decreaseFactor = 1 - osp;
					wi->rect.height *= decreaseFactor;
					wi->rect.width *= decreaseFactor;
					wi->rect.x += .5 * xrightmax - .5 * wi->rect.width;
					printf( "(2) item oversize -> shift wi->rect.height %fi wi->rect.width %f\n", wi->rect.height, wi->rect.width );
					dobreak = false;
				}
			}

			double y0 = item0->rect.y + item0->rect.height;
			if ( dobreak )
			{
				pdfdoc->AddPage();
				//y0 = contentRect.y;
				y0 = pdfdoc->GetTopMargin();
			}

			// shift all items after item0
			list<SheetItem*>::iterator iter, iter0 = sheet->items.end();
			for( iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
			{
				if ( *iter == item0 )
				{
					iter0 = iter;
					break;
				}
			}

			for( iter = iter0; iter != sheet->items.end(); iter++ )
			{
				SheetItem *item = (SheetItem*)*iter;

				// TODO translate method on sheet item
				item->rect.y = y0;
				y0 += item->rect.height;
			}

			/*
			bool doit = false;
			for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
			{
				if ( *iter == item0 )
				{
					doit = true;

					// item0 hasn't be shifted if it was decrease 
					if ( ! dobreak ) continue;
				}
				if ( ! doit ) continue;

				SheetItem *item = (SheetItem*)*iter;
				item->rect.y = y0;
				y0 += item->rect.height;
			}
		*/
		}
	}

	/*****************************************************
	**
	**   PdfSheetWriter   ---   getFontIdForItem
	**
	******************************************************/
	FONT_ID getFontIdForItem( const SHEET_TEXT_ITEM_SUBTYPE &type )
	{
		printf( "PdfSheetWriter   ---   getFontIdForItem %d\n", (int)type );
		switch( type )
		{
			case WitHeader:
				return FONT_PDF_HEADER;
			break;
			case WitParagraph:
			case WitLine:
				return FONT_PDF_DEFAULT;
			break;
			case WitTableCell:
				return FONT_PDF_TABLE_CONTENTS;
			break;
			case WitTableHeader:
				return FONT_PDF_TABLE_HEADER;
			break;
			default:
				assert( false );
			break;
		}
	}


	PdfDocumentConfig *pcfg;
	BasePdfDocument *pdfdoc;
};

IMPLEMENT_CLASS( PdfSheetWriter, GenericSheetWriter )

/*****************************************************
**
**   PdfTool   ---   Constructor
**
******************************************************/
PdfTool::PdfTool( PdfDocumentConfig *cfg )
{
	if ( cfg ) pcfg = new PdfDocumentConfig( *cfg );
	else pcfg = new PdfDocumentConfig( *config->print );
}

/*****************************************************
**
**   PdfTool   ---   Destructor
**
******************************************************/
PdfTool::~PdfTool()
{
	delete pcfg;
}

/*****************************************************
**
**   PdfTool   ---   printTestpage
**
******************************************************/
void PdfTool::printTestpage()
{
	Horoscope *h = new Horoscope;
	h->update();
	wxString filename = wxFileName::CreateTempFileName( wxT( "testpage" ));
	printHoroscope( h, filename );
	delete h;
}

/*****************************************************
**
**   PdfTool   ---   printTestpage
**
******************************************************/
void PdfTool::printTestpage( Horoscope *horoscope, ChartProperties *props )
{
	Sheet *sheet = new Sheet;
	SheetConfigLoader *sloader = SheetConfigLoader::get();
	wxString filename = wxFileName::CreateTempFileName( wxT( "testpage" ));

	PrintoutSheetCreator c( horoscope, props, sheet );
	c.write( pcfg->defaultPrintout );

	PdfSheetWriter *pdfwriter = new PdfSheetWriter( sheet, sloader->getConfig( pcfg->sheetStyle ), config->writer );
	pdfwriter->doPrint( filename );

	if ( pcfg->launchPdfViewer ) launchBrowser( filename );
	else
	{
		doMessageBox( (wxWindow*)NULL, wxString::Format( _( "Chart printed to file %s." ), filename.c_str() ), wxOK | wxCENTRE );
	}
}

/*****************************************************
**
**   PdfTool   ---   doSheetExport
**
******************************************************/
bool PdfTool::doSheetExport( Sheet *sheet )
{
  SheetConfigLoader *loader = SheetConfigLoader::get();
	SheetConfig *sheetcfg = loader->getConfig( config->view->sheetStyle );

	//wxString filename = wxFileName::CreateTempFileName( wxT( "testpage" ));
	wxString filename;
	const bool b = selectFilename( filename, wxT( "out" ) );
	if ( ! b ) return false;

	PdfSheetWriter *pdfwriter = new PdfSheetWriter( sheet, sheetcfg, config->writer );
	pdfwriter->doPrint( filename );

	if ( pcfg->launchPdfViewer ) launchBrowser( filename );
	else
	{
		doMessageBox( (wxWindow*)NULL, wxString::Format( _( "Chart printed to file %s." ), filename.c_str() ), wxOK | wxCENTRE );
	}
	delete pdfwriter;
	return true;
}

/*****************************************************
**
**   PdfTool   ---   selectPrintoutId
**
******************************************************/
bool PdfTool::selectPrintoutId( int &type )
{
	PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
	wxSingleChoiceDialog dialog( 0, _( "Select Printout" ), GUI_APP_NAME, loader->getNamesAsArray() );
	dialog.SetSelection( pcfg->defaultPrintout );
	if ( IS_VALID_SIZE( config->viewprefs->sizes.sPrintoutSelectionDialog ))
	{
		dialog.SetSize( config->viewprefs->sizes.sPrintoutSelectionDialog );
	}
	if ( dialog.ShowModal() == wxID_OK )
	{
		config->viewprefs->sizes.sPrintoutSelectionDialog = dialog.GetSize();
		type = dialog.GetSelection();
		return true;
	}
	//return pcfg->defaultPrintout;
	return false;
}

/*****************************************************
**
**   PdfTool   ---   selectFilename
**
******************************************************/
bool PdfTool::selectFilename( wxString &fullname, wxString basename )
{
	fullname.Clear();

	if ( basename.IsEmpty()) basename = wxT( "unknown" );
	wxString filename;
	filename << basename << wxT( ".pdf" );
	const static wxString filetypes = wxT( "Portable Document Format (*.pdf)|*.pdf|All files (*)| *.*" );

	int style = wxFD_SAVE;
	if ( pcfg->askOverwrite )
	{
		printf( "GUNNAR\n" );
		style |= wxFD_OVERWRITE_PROMPT;
	}
	else
	{
		printf( "GUNNAR2\n" );
	}

	wxFileDialog exportFileDialog( NULL, _("Save Document" ), config->viewprefs->defExportPath, filename, filetypes, style, wxDefaultPosition );
	if ( exportFileDialog.ShowModal() == wxID_OK )
	{
		fullname << exportFileDialog.GetDirectory() << wxFileName::GetPathSeparator() << exportFileDialog.GetFilename();
	}
	else
	{
		return false;
	}
	config->viewprefs->defExportPath = exportFileDialog.GetDirectory();

	printf( "FULLNAME %s\n", str2char( fullname ));

	return true;
}

/*****************************************************
**
**   PdfTool   ---   printHoroscope
**
******************************************************/
void PdfTool::printHoroscope( Horoscope *horoscope, const bool doSelect )
{
	wxString filename;

	if ( doSelect )
	{
		if ( ! selectPrintoutId( pcfg->defaultPrintout )) return;
		if ( ! selectFilename( filename, horoscope->getHName())) return;
	}
	else
	{
		if ( horoscope->getHName().IsEmpty())
		{
			horoscope->setHName( wxT( "unknown" ));
		}
		filename = config->viewprefs->defExportPath;
		if ( ! filename.IsEmpty()) filename << wxFileName::GetPathSeparator();
		filename << horoscope->getHName() << wxT( ".pdf" );
	}
	printHoroscope( horoscope, filename );
}

/*****************************************************
**
**   PdfTool   ---   printHoroscope
**
******************************************************/
void PdfTool::printHoroscope( Horoscope *horoscope, wxString filename )
{
	ChartProperties *props = new ChartProperties;
	props->setWesternSkin( config->print->wGraphicSkin );
	props->setVedicSkin( config->print->vGraphicSkin );
	//printf( "VEdicChart skin %d\n", props->getVedicSkin());

	PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
	PrintoutConfig *theconfig = loader->getConfig( pcfg->defaultPrintout );
	props->setVedic( theconfig->vedic );
	printf( "VEDIC %d vierwer %s\n", theconfig->vedic, str2char( pcfg->pdfViewerCommand ) );

	Sheet *sheet = new Sheet;
	SheetConfigLoader *sloader = SheetConfigLoader::get();

	PrintoutSheetCreator c( horoscope, props, sheet );
	c.write( pcfg->defaultPrintout );

	PdfSheetWriter *pdfwriter = new PdfSheetWriter( sheet, sloader->getConfig( pcfg->sheetStyle ), config->writer );
	pdfwriter->doPrint( filename );

	finishPrint( filename );

	delete pdfwriter;
	delete props;
	delete sheet;
}

/*****************************************************
**
**   PdfTool   ---   finishPrint
**
******************************************************/
bool PdfTool::finishPrint( wxString filename )
{
	if ( pcfg->launchPdfViewer ) return launchBrowser( filename );

	//else
	doMessageBox( (wxWindow*)NULL, wxString::Format( _( "Chart printed to file %s." ), filename.c_str() ), wxOK | wxCENTRE );
	return false;
}

/*****************************************************
**
**   PdfTool   ---   launchBrowser
**
******************************************************/
bool PdfTool::launchBrowser( wxString filename )
{
	wxString command;

#ifdef __WXMSW__
	wxFileType *filetype = wxMimeTypesManager().GetFileTypeFromExtension( wxT( "pdf" ));
	if ( filetype != 0 )
	{
		command = filetype->GetOpenCommand( filename );
	}
	else
	{
		doMessageBox( 0, _( "Cannot launch pdf viewer." ), wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
#else

	if ( pcfg->pdfViewerCommand.IsEmpty())
	{
		doMessageBox( 0,
			_( "Pdf viewer command is empty. Please configure the command in the configuration dialog." ),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}

	// wxmac has generic open command
#ifndef __WXMAC__
	else if ( ! wxFileName::FileExists( pcfg->pdfViewerCommand ))
	{
		doMessageBox( 0,
			wxString::Format( _( "Pdf viewer file \"%s\" does not exist. Please configure the command in the configuration dialog." ),
				pcfg->pdfViewerCommand.c_str()),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
#endif

	command << pcfg->pdfViewerCommand << wxT( " \"" ) << filename << wxT( "\"" );
#endif
	long r = wxExecute( command );
	if ( r == -1 )
	{
		doMessageBox( 0,
			wxString::Format( _( "Can't launch pdf viewer, command is \"%s\" (error code %ld)" ), command.c_str(), r ),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
	return true;
}


