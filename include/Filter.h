//
//  Filter.h
//  Kepler
//
//  Created by Tom Carden on 6/4/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "CinderIPod.h"

// pure virtual "interface" class, Filters must implement tests for PlaylistRefs and TrackRefs
class Filter {
public:
    Filter() {}
    virtual ~Filter() {}
    virtual bool test(ci::ipod::PlaylistRef artistOrAlbum) const = 0;
    virtual bool test(ci::ipod::TrackRef track) const = 0;
};