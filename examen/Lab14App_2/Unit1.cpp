//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <jpeg.hpp>
#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;

TPoint pt[100];
int cnt = 0;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject* Sender)
{
    OpenDialog1->Filter = "Bmp files(*.bmp)|*.BMP|JPEG images\(*.jpg)|*.jpg;";
    Image1->Proportional = true;
    Image2->Proportional = true;
    Image3->Proportional = true;

     /// ** de completat *///

    Graphics::TBitmap* imgs = new Graphics::TBitmap;
    imgs->Assign(Image1->Picture->Bitmap);
    imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;
    Graphics::TBitmap* imgd = new Graphics::TBitmap;
    imgd->Width = imgs->Width;
    imgd->Height = imgs->Height;
    imgd->PixelFormat = imgs->PixelFormat;

	 /// ** de completat *///
}

void __fastcall TForm1::Button7Click(TObject* Sender)
{
	Graphics::TBitmap* imgs = new Graphics::TBitmap;
    imgs->Assign(Image1->Picture->Bitmap);
    imgs->PixelFormat = Image1->Picture->Bitmap->PixelFormat;

    Graphics::TBitmap* imgd = new Graphics::TBitmap;
    imgd->Width = imgs->Width;
    imgd->Height = imgs->Height;
    imgd->PixelFormat = imgs->PixelFormat;
	 /// ** de completat *///
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject* Sender)
{
	 /// ** de completat *///
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
	 /// ** de completat *///
}

void __fastcall TForm1::Image1MouseDown(
	TObject* Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	 /// ** de completat *///
}

//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject* Sender)
{
	/// ** de completat *///
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject* Sender)
{
	 /// ** de completat *///
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject* Sender)
{
	 /// ** de completat *///
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject* Sender)
{
	 /// ** de completat *///
}
//---------------------------------------------------------------------------

