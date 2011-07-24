//
//  UIController.cpp
//  Kepler
//
//  Created by Tom Carden on 7/17/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "UIController.h"
#include "cinder/app/AppCocoaTouch.h"
#include "cinder/gl/gl.h"
#include "OrientationHelper.h"

using namespace ci;
using namespace ci::app;

UIControllerRef UIController::create( AppCocoaTouch *app )
{
    UIControllerRef ref = UIControllerRef( new UIController( app ) );
    ref->mRoot = ref;
    return ref;
}

UIController::UIController( AppCocoaTouch *app ): 
    mApp( app ), 
    mInterfaceSize( 0.0f, 0.0f )
{
    mParent = UINodeRef();     // NULL, we are the parent (crash rather than recurse)
    mRoot = UIControllerRef(); // NULL, will be set in create() because we are the root
    
    cbTouchesBegan = mApp->registerTouchesBegan( this, &UIController::touchesBegan );
    cbTouchesMoved = mApp->registerTouchesMoved( this, &UIController::touchesMoved );
    cbTouchesEnded = mApp->registerTouchesEnded( this, &UIController::touchesEnded );
    
    mInterfaceSize = mApp->getWindowSize();
}

UIController::~UIController()
{
    mApp->unregisterTouchesBegan( cbTouchesBegan );
    mApp->unregisterTouchesMoved( cbTouchesMoved );
    mApp->unregisterTouchesEnded( cbTouchesEnded );
}

bool UIController::touchesBegan( TouchEvent event )
{
    bool consumed = true;
    for (std::vector<TouchEvent::Touch>::const_iterator i = event.getTouches().begin(); i != event.getTouches().end(); i++) {
        consumed = privateTouchBegan(*i) && consumed; // recurses to children
    }    
    return consumed; // only true if all touches were consumed
}

bool UIController::touchesMoved( TouchEvent event )
{
    bool consumed = true;
    for (std::vector<TouchEvent::Touch>::const_iterator i = event.getTouches().begin(); i != event.getTouches().end(); i++) {
        consumed = privateTouchMoved(*i) && consumed; // recurses to children
    }
    return consumed; // only true if all touches were consumed
}

bool UIController::touchesEnded( TouchEvent event )
{
    bool consumed = true;
    for (std::vector<TouchEvent::Touch>::const_iterator i = event.getTouches().begin(); i != event.getTouches().end(); i++) {
        consumed = privateTouchEnded(*i) && consumed; // recurses to children
    }    
    return consumed; // only true if all touches were consumed
}

Matrix44f UIController::getConcatenatedTransform() const
{
    return mTransform;
}

void UIController::draw()
{
    if (mVisible) {
        glPushMatrix();
        glMultMatrixf( getConcatenatedTransform() );    
        // draw children
        for (std::vector<UINodeRef>::const_iterator i = mChildren.begin(); i != mChildren.end(); i++) {
            (*i)->privateDraw();
        }
        // dont' draw self or we'll recurse
        glPopMatrix();
    }
}

void UIController::update()
{
    if (mVisible) {
        // update children
        for (std::vector<UINodeRef>::const_iterator i = mChildren.begin(); i != mChildren.end(); i++) {
            (*i)->privateUpdate();
        }
        // dont' update self or we'll recurse
    }
}
