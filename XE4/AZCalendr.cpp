
//---------------------------------------------------------------------------

// This software is Copyright (c) 2011 Embarcadero Technologies, Inc. 
// You may only use this software if you are an authorized licensee
// of Delphi, C++Builder or RAD Studio (Embarcadero Products).
// This software is considered a Redistributable as defined under
// the software license agreement that comes with the Embarcadero Products
// and is subject to that software license agreement.

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// C++Builder Sample Components
// Copyright(c) 1998-2011 Embarcadero Technologies, Inc.
//---------------------------------------------------------------------------
#pragma hdrstop
#include  "AZCalendr.h"
#pragma package(smart_init)

__fastcall TAZCalendar::TAZCalendar(TComponent *AOwner) :
  TCustomGrid(AOwner)
{
  FUseCurrentDate = true;
  FixedCols       = 0;
  FixedRows       = 1;
  ColCount        = 7;             
  RowCount        = 7;
  StartOfWeek     = 1;
  ScrollBars      = ssNone;
  FDate           = Date();
  FSundayColor    = clRed;
  (Options >> goRangeSelect) << goDrawFocusSelected;
  UpdateCalendar();
}

__fastcall TAZCalendar::TAZCalendar(HWND Parent): TCustomGrid(Parent) 
{}

void __fastcall TAZCalendar::Change()
{
  if(FOnChange)
    FOnChange(this);
}

void __fastcall TAZCalendar::Click()
{
  System::String TheCellText;

  TheCellText = CellText[Col][Row];
  if(!TheCellText.IsEmpty())
    Day = TheCellText.ToInt();
}


/* Don't have any leap year code available.  Seemed like a straigthforward
   translation, but should be checked.
*/
bool __fastcall TAZCalendar::IsLeapYear(int AYear)
{
  return  ((AYear %   4) == 0) &&
         (((AYear % 100) != 0) ||
          ((AYear % 400) == 0));
};


int __fastcall TAZCalendar::DaysPerMonth(int AYear, int AMonth)
{
  int result;
  const int DaysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  result = DaysInMonth[AMonth-1];
  if ((AMonth == 2) && IsLeapYear(AYear))
    ++result; // leap-year Feb is special

  return result;
};

int __fastcall TAZCalendar::DaysThisMonth()
{
  return DaysPerMonth(Year, Month);
};


void __fastcall TAZCalendar::DrawCell(int ACol, int ARow, const TRect &ARect,
  TGridDrawState /*AState*/)
{
  const System::String TheText = CellText[ACol][ARow];

  if(ACol == 6) {
    Canvas->Font->Color = FSundayColor;
  }
  else {
    Canvas->Font->Color = Font->Color;
  }

  Canvas->TextRect(
    ARect,
    ARect.Left + (ARect.Right - ARect.Left - Canvas->TextWidth(TheText)) / 2,
    ARect.Top + (ARect.Bottom - ARect.Top - Canvas->TextHeight(TheText)) / 2,
    TheText);

  Canvas->Font->Color = Font->Color;
};


System::String __fastcall TAZCalendar::GetCellText(int ACol, int ARow)
{
  int DayNum;
  System::String  result;

  if (!ARow)
    // day names at tops of columns
    //
    result = FormatSettings.ShortDayNames[(StartOfWeek + ACol) % 7];
  else
  {
    DayNum = FMonthOffset + ACol + (ARow - 1) * 7;
    if ((DayNum < 1) || (DayNum > DaysThisMonth()))
      result = "";
    else
      result = IntToStr(DayNum);
  }

  return result;
};

bool __fastcall TAZCalendar::SelectCell(int ACol, int ARow)
{
  bool  result;

  if ((!FUpdating && FReadOnly) || !CellText[ACol][ARow].c_str())
    result = false;
  else
   result = TCustomGrid::SelectCell(ACol, ARow);

  return result;
};

void __fastcall TAZCalendar::SeTCCalendarDate(TDateTime Value)
{
  FDate = Value;
  UpdateCalendar();
  Change();
};

bool __fastcall TAZCalendar::StoreCalendarDate()
{
  return !FUseCurrentDate;
};

int __fastcall  TAZCalendar::GetDateElement(int Index)
{
  Word  AYear, AMonth, ADay;
  int result;

  DecodeDate(FDate, AYear, AMonth, ADay);
  switch(Index)
  {
    case 1:
      result = AYear;break;

    case 2:
      result = AMonth;break;

    case 3:
      result = ADay;break;

    default:
      result = -1;
  };

  return result;
};

#pragma warn -sig
void __fastcall TAZCalendar::SetDateElement(int Index, int Value)
{
  Word  AYear, AMonth, ADay;
  bool  Update  = false;

  if (Value > 0)
  {
    DecodeDate(FDate, AYear, AMonth, ADay);
    switch(Index)
    {
      case 1:
        if (AYear != Value)
        {
          AYear   = Value;
          Update  = true;
        }
        break;

      case 2:
        if ((Value <= 12) && (Value != AMonth))
        {
          AMonth  = Value;
          Update  = true;
        }
        break;

      case 3:
        if ((Value <= DaysThisMonth()) && (Value != ADay))
        {
          ADay    = Value;
          Update  = true;
        }
        break;
    }
    if (Update)
    {
      FDate = EncodeDate(AYear, AMonth, ADay);
      FUseCurrentDate = false;
      UpdateCalendar();
      Change();
    }
  };
};

void __fastcall TAZCalendar::SetStartOfWeek(short Value)
{
  if( (Value > 7) || (Value < 1) )
  {
      FStartOfWeek=1; // bad value sets to 1 (Sunday)
  }
  else
  {
      if (Value != FStartOfWeek)
      {
        FStartOfWeek = Value;
        UpdateCalendar();
      }
  }
};

void __fastcall TAZCalendar::SetUseCurrentDate(bool Value)
{
  if (Value != FUseCurrentDate)
  {
    FUseCurrentDate = Value;
    if (Value)
    {
      FDate = Date(); // use the current date, then
      UpdateCalendar();
    };
  };
};

// Given a value of 1 or -1, moves to Next or Prev month accordingly.
//
void __fastcall TAZCalendar::ChangeMonth(int Delta)
{
  Word  AYear, AMonth, ADay;
  TDateTime NewDate;
  int CurDay;

  DecodeDate(FDate, AYear, AMonth, ADay);
  CurDay = ADay;
  if (Delta > 0)
    ADay = DaysPerMonth(AYear, AMonth);
  else
    ADay = 1;

  NewDate = EncodeDate(AYear, AMonth, ADay);
  NewDate = NewDate + Delta;
  DecodeDate(NewDate, AYear, AMonth, ADay);
  if (DaysPerMonth(AYear, AMonth) > CurDay)
    ADay = CurDay;
  else
    ADay = DaysPerMonth(AYear, AMonth);

  CalendarDate = EncodeDate(AYear, AMonth, ADay);
};
#pragma warn .sig

void __fastcall TAZCalendar::PrevMonth()
{
  ChangeMonth(-1);
}

void __fastcall TAZCalendar::NextMonth()
{
  ChangeMonth(1);
}

void __fastcall TAZCalendar::NextYear()
{
  if (IsLeapYear(Year) && (Month == 2) && (Day == 29))
    Day = 28;
  ++Year;
}

void __fastcall TAZCalendar::PrevYear()
{
  if (IsLeapYear(Year) && (Month == 2) && (Day == 29))
    Day = 28;
  --Year;
}

void __fastcall TAZCalendar::UpdateCalendar()
{
  Word  AYear, AMonth, ADay;
  TDateTime FirstDate;

  FUpdating = True;
  DecodeDate(FDate, AYear, AMonth, ADay);
  FirstDate = EncodeDate(AYear, AMonth, 1);

  // Day of week for 1st of month.
  //
  FMonthOffset = 2 - ((DayOfWeek(FirstDate) - StartOfWeek + 7) % 7);
  if (FMonthOffset == 2)
    FMonthOffset = -5;

  MoveColRow((ADay - FMonthOffset) % 7,
             (ADay - FMonthOffset) / 7 + 1,
             False, False);
    Invalidate();

  FUpdating = False;
};

void __fastcall TAZCalendar::WMSize(TWMSize &Message)
{
  int GridLines;

  GridLines = 6 * GridLineWidth;
  DefaultColWidth   = (Message.Width - GridLines) / 7;
  DefaultRowHeight  = (Message.Height - GridLines) / 7;
}

void __fastcall TAZCalendar::SetSundayColor(TColor c)
{
    if(FSundayColor != c) {
        FSundayColor = c;
        UpdateCalendar();
    }
}
