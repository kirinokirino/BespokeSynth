/**
    bespoke synth, a software modular synthesizer
    Copyright (C) 2021 Ryan Challinor (contact: awwbees@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/
//
//  IUIControl.h
//  modularSynth
//
//  Created by Ryan Challinor on 1/13/13.
//
//

#ifndef modularSynth_IUIControl_h
#define modularSynth_IUIControl_h

#include "IClickable.h"
#include "SynthGlobals.h"

class FileStreamIn;
class FileStreamOut;
class PatchCableSource;

#define HIDDEN_UICONTROL 9999

enum AnchorDirection
{
   kAnchor_Below,
   kAnchor_Right,
   kAnchor_Right_Padded
};

class IUIControl : public IClickable
{
public:
   IUIControl()
   : mRemoteControlCount(0)
   , mNoHover(false)
   , mShouldSaveState(true)
   {}
   virtual void Delete() { delete this; }
   void AddRemoteController() { ++mRemoteControlCount; }
   void RemoveRemoteController() { --mRemoteControlCount; }
   virtual void SetFromMidiCC(float slider, bool setViaModulator = false) = 0;
   virtual float GetValueForMidiCC(float slider) const { return 0; }
   virtual void SetValue(float value) = 0;
   virtual void SetValueDirect(float value) { SetValue(value); } //override if you need special control here
   virtual float GetValue() const { return 0; }
   virtual float GetMidiValue() const { return 0; }
   virtual int GetNumValues() { return 0; } //the number of distinct values that you can have for this control, zero indicates infinite (like a float slider)
   virtual std::string GetDisplayValue(float val) const { return "unimplemented"; }
   virtual void Init() {}
   virtual void Poll() {}
   virtual void KeyPressed(int key, bool isRepeat) {}
   void StartBeacon() override;
   bool IsPreset();
   virtual void GetRange(float& min, float& max)
   {
      min = 0;
      max = 1;
   }
   virtual bool IsBitmask() { return false; }
   bool TestHover(int x, int y);
   void CheckHover(int x, int y);
   void DrawHover(float x, float y, float w, float h);
   void DrawPatchCableHover();
   virtual bool CanBeTargetedBy(PatchCableSource* source) const;
   virtual bool InvertScrollDirection() { return false; }
   virtual void Double() {}
   virtual void Halve() {}
   virtual void ResetToOriginal() {}
   virtual void Increment(float amount) {}
   void SetNoHover(bool noHover) { mNoHover = noHover; }
   bool GetNoHover() const { return mNoHover; }
   virtual bool AttemptTextInput() { return false; }
   void PositionTo(IUIControl* anchor, AnchorDirection direction);
   void GetColors(ofColor& color, ofColor& textColor);
   bool GetShouldSaveState() const { return mShouldSaveState; }
   void SetShouldSaveState(bool save) { mShouldSaveState = save; }
   virtual bool IsSliderControl() { return true; }
   virtual bool IsButtonControl() { return false; }
   virtual bool IsMouseDown() const { return false; }
   virtual bool IsTextEntry() const { return false; }

   static void SetNewManualHover(int direction);
   static bool WasLastHoverSetViaTab() { return sLastUIHoverWasSetViaTab; }

   virtual void SaveState(FileStreamOut& out) = 0;
   virtual void LoadState(FileStreamIn& in, bool shouldSetValue = true) = 0;

protected:
   virtual ~IUIControl();

   int mRemoteControlCount;
   bool mNoHover;
   bool mShouldSaveState;

   static IUIControl* sLastHoveredUIControl;
   static bool sLastUIHoverWasSetViaTab;
};

#endif
