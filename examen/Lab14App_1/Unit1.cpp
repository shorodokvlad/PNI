//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "jpeg.hpp"
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* Sender)
{
    OpenDialog1->Filter = "Bmp files(*.bmp)|*.BMP|JPEG images\(*.jpg)|*.jpg;";
    Image1->Proportional = true;
    if (OpenDialog1->Execute()) {
        if (!FileExists(OpenDialog1->FileName))
			return;
        AnsiString temp = ExtractFileExt(OpenDialog1->FileName);
        AnsiString Ext = temp.LowerCase();
		if (Ext.AnsiPos("jpg") > 0)
		{
            TJPEGImage* myjpeg = new TJPEGImage();
			myjpeg->LoadFromFile(OpenDialog1->FileName);
            myjpeg->DIBNeeded();
            Image1->Picture->Bitmap->Assign(myjpeg);
            delete myjpeg;
        } else if (Ext.AnsiPos("bmp") > 0)
            Image1->Picture->Bitmap->LoadFromFile(OpenDialog1->FileName);
    }

	/// histograma imaginii sursa -> start
    Graphics::TBitmap* source = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
    source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    RGBTRIPLE* pixels;
    int histoSR[256], histoSG[256], histoSB[256];
	for (int i = 0; i < 256; i++) {
		histoSR[i] = 0;
		histoSG[i] = 0;
		histoSB[i] = 0;
	}
    for (int y = 0; y < source->Height; y++) {
        pixels = (RGBTRIPLE*)source->ScanLine[y];
        for (int x = 0; x < source->Width; x++) {
            histoSR[pixels[x].rgbtRed]++;
            histoSG[pixels[x].rgbtGreen]++;
            histoSB[pixels[x].rgbtBlue]++;
        }
    }
    delete source;
    Chart1->Series[0]->Clear();
    Chart1->Series[1]->Clear();
    Chart1->Series[2]->Clear();
    for (int i = 0; i < 256; i++) {
        Chart1->Series[0]->AddY(histoSR[i], "", clRed);
        Chart1->Series[1]->AddY(histoSG[i], "", clGreen);
        Chart1->Series[2]->AddY(histoSB[i], "", clBlue);
    }
    Chart1->Series[0]->Active = true;
	Chart1->Series[1]->Active = true;
	Chart1->Series[2]->Active = true;
    /// histograma imaginii sursa -> stop

}

//---------------------------------------------------------------------------

void __fastcall TForm1::Image1MouseDown(
    TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{

	/// necesar pentru histograma pe linie dar si pentru profilul pe linie -> start
	Graphics::TBitmap* source = new Graphics::TBitmap;
    source->Assign(Image1->Picture->Bitmap);
	source->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

	RGBTRIPLE* pixels = (RGBTRIPLE*)
				 source->ScanLine[Y * (float)source->Height / Image1->Height];
	 /// necesar pentru histograma pe linie dar si pentru profilul pe linie -> stop

	/// histograma pe linie -> start
	int histoSR[256], histoSG[256], histoSB[256];

	memset(histoSR, 0, sizeof(histoSR));
	memset(histoSG, 0, sizeof(histoSG));
	memset(histoSB, 0, sizeof(histoSB));
	///sau
	/*for (int i = 0; i < 256; i++) {
		histoSR[i] = 0;
		histoSG[i] = 0;
		histoSB[i] = 0;
	}  */

	for (int x = 0; x < source->Width; x++) {
        histoSR[pixels[x].rgbtRed]++;
        histoSG[pixels[x].rgbtGreen]++;
        histoSB[pixels[x].rgbtBlue]++;
    }

    Chart2->Series[0]->Clear();
    Chart2->Series[1]->Clear();
    Chart2->Series[2]->Clear();
    for (int i = 0; i < 256; i++) {
        Chart2->Series[0]->AddY(histoSR[i], "", clRed);
        Chart2->Series[1]->AddY(histoSG[i], "", clGreen);
        Chart2->Series[2]->AddY(histoSB[i], "", clBlue);
    }
	Chart2->Series[0]->Active = true;
    Chart2->Series[1]->Active = true;
	Chart2->Series[2]->Active = true;

	/// histograma pe linie -> stop


	/// profilul pe linie -> start
	Chart3->Series[0]->Clear();
    Chart3->Series[1]->Clear();
    Chart3->Series[2]->Clear();
    for (int i = 0; i < source->Width; i++) {
        Chart3->Series[0]->AddY(pixels[i].rgbtRed, "", clRed);
        Chart3->Series[1]->AddY(pixels[i].rgbtGreen, "", clGreen);
        Chart3->Series[2]->AddY(pixels[i].rgbtBlue, "", clBlue);
    }
    Chart3->Series[0]->Active = true;
    Chart3->Series[1]->Active = true;
	Chart3->Series[2]->Active = true;

	/// profilul pe linie -> stop
	delete source;
}
//---------------------------------------------------------------------------

