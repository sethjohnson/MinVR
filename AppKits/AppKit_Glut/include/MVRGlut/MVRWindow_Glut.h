/* ================================================================================

This file is part of the MinVR Open Source Project, which is developed and 
maintained collaboratively by the University of Minnesota's Interactive 
Visualization Lab and the Brown University Visualization Research Lab.

File: MinVR/AppKits/AppKit_Glut/include/MVRGlut/MVRWindow_Glut.H

Original Author(s) of this File: 
    Morgan McGuire and Dan Keefe, 2004-ish, Brown University (originally VRG3D/GlutWindow.h)
    
Author(s) of Significant Updates/Modifications to the File:
    Bret Jackson, 2013, University of Minnesota (adapted to work with MinVR)

-----------------------------------------------------------------------------------
Copyright (c) 2008-2015 Regents of the University of Minnesota and Brown University
Copyright (c) 2004-2008 Brown University
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.

* Neither the name of the University of Minnesota, Brown University, nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
================================================================================ */

/**
 @file GlutWindow.h
 @author Morgan McGuire and Dan Keefe
 */
#ifndef GLUTWINDOW_H
#define GLUTWINDOW_H

#include <G3D/G3DAll.h>

#if G3D_VER < 60300
    #error Requires G3D 6.03
#endif

#define GLUT_API_VERSION 4

#ifdef G3D_OSX
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


/**
 GWindow that uses the glut API http://www.opengl.org/resources/libraries/glut/spec3/spec3.html.

  Not supported by GlutWindow:
  <UL>
   <LI> Joysticks
   <LI> Gamma ramp
   <LI> Window Icons
   <LI> Input capture
  </UL>

 Additionally, Glut does not support all keystrokes (e.g. the key-down event for a modifier
 cannot be detected, scan-codes are not available) so GlutWindow fails to report some keyboard
 events.

 @cite Glut by Mark Kilgard, ported to Windows by Nate Robbins
 */
// The public API of this implementation allows only one window because it does
// not provide a mechanism for switching active windows/contexts.  However,
// internally it has been coded so that this will be a simple change.
class GlutWindow : public GWindow {
private:

    /** Has glutInit been called? (only allowed once per program)*/
    static bool             glutInitialized;

    /** Underlying glut window handle */
    int                     glutWindowHandle;

    int                     _mouseHideCount;

    std::string             _windowTitle;

    Rect2D                  _dimensions;

    GWindow::Settings       settings;

    Queue<GEvent>           eventQueue;

    Vector2                 mouse;

    uint8                   mouseButtons;

    /** Where to send GLUT events */
    static GlutWindow*      currentGlutWindow;

    /** Adds an event to the queue-- is synchronized! */
    void postEvent(GEvent& evt);

    // Glut callbacks:
    static void g_reshape(int width, int height);
    static void g_keyboard(unsigned char c, int x, int y);
    static void g_keyboardspecial(int key, int x, int y);
    static void g_keyboardup(unsigned char c, int x, int y);
    static void g_keyboardspecialup(int key, int x, int y);
    static void g_mousemotion(int x, int y);
    static void g_mousebtn(int b, int s, int x, int y);
    static void g_draw();

public:

    GlutWindow(const GWindow::Settings& s);
    virtual ~GlutWindow();
    virtual void getSettings(GWindow::Settings& settings) const;
    virtual int width() const;
    virtual int height() const;
    virtual Rect2D dimensions() const;
    virtual void setDimensions(const Rect2D& dims);
    virtual void setPosition(int x, int y);
    virtual bool hasFocus() const;
    virtual std::string getAPIVersion() const;
    virtual std::string getAPIName() const;
    virtual void setGammaRamp(const Array<uint16>& gammaRamp);
    virtual void setCaption(const std::string& caption);
    virtual int numJoysticks() const;
    virtual std::string joystickName(unsigned int sticknum);
    virtual std::string caption();
    virtual void setIcon(const GImage& image);
    virtual void swapGLBuffers();
    virtual void notifyResize(int w, int h);
    virtual bool pollEvent(GEvent& e);

    virtual bool requiresMainLoop() const {
        return true;
    }

    virtual void runMainLoop();
    virtual void setRelativeMousePosition(double x, double y);
    virtual void setRelativeMousePosition(const Vector2& p);
    virtual void getRelativeMouseState(Vector2& position, uint8& mouseButtons) const;
    virtual void getRelativeMouseState(int& x, int& y, uint8& mouseButtons) const;
    virtual void getRelativeMouseState(double& x, double& y, uint8& mouseButtons) const;
    virtual void getJoystickState(unsigned int stickNum, Array<float>& axis, Array<bool>& button);
    virtual void setInputCapture(bool c);
    virtual bool inputCapture() const;

    virtual bool mouseVisible() const;
    virtual void setMouseVisible(bool v);
    virtual void incMouseHideCount();
    virtual void decMouseHideCount();

    virtual void getDroppedFilenames(G3DLite::Array<std::string>&);
};


#endif
