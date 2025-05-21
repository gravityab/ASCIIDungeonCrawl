///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Animation.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)

#pragma once

/// Dungeon Crawl Library Headers
#include "Frame.h"
#include "Time.h"

/// Engine Library Headers
#include "Entity.h"

/// Standard Templicate Library Headers
#include <stdint.h>
#include <vector>

/// Enumeration of all objects with quality
class Animation
{
public:
    /// Constructor for an animation
    Animation();

    /// Constructor for an animation with a static frame rate
    Animation(std::vector<Frame> frames, int fpt, bool repeat, bool animate, bool random);

    /// Constructor for an animation with a random frame rate range
    Animation(std::vector<Frame> frames, int minFpt, int maxFpt, bool repeat, bool animate, bool random);

public:
    /// Write the animation to a console entity
    void WriteData(ConsoleEntity& entity, Time delta, int x, int y, bool& complete);

    /// Assign a list of frames to display at a static rate or cadence
    void SetData(std::vector<Frame> frames, int fpt, bool repeat, bool animate, bool random);

    /// Assign a list of frames to display at a random rate or cadence
    void SetData(std::vector<Frame> frames, int minFpt, int maxFpt, bool repeat, bool animate, bool random);

    /// Set the attribute of a specific frame and image
    void SetAttribute(int frame, int image, uint16_t attribute);

    /// Set the attribute of all images within frame
    void SetAttributes(int image, uint16_t attribute);

    /// Perform a strobe like effect
    void SetStrobe(int image, bool strobe);

    /// Set the frames per tick to a different tate
    void SetFramesPerTick(int fpt);

    /// Whether the animation is currently animating
    bool IsAnimating() const;

    /// Tell the animation to animate or not
    void SetAnimating(bool animate);

    /// Reset the animation data internally
    void Reset();

    /// Create copy using specific attributes
    Animation As(uint16_t att1);
    Animation As(uint16_t att1, uint16_t att2);
    Animation As(uint16_t att1, uint16_t att2, uint16_t att3);

private:
    /// The current time
    Time m_currentTime;

    /// The frame rate of the animation
    Time m_frameRate;

    /// The minimum frame rate
    Time m_minFrameRate;

    /// The maximum frame rate
    Time m_maxFrameRate;

    /// The current frame index to draw
    int m_currentIndex = 0;

    /// The list of frames associated with the animation
    std::vector<Frame> m_frames;

    /// Whether the animation is animating
    bool m_animating = false;

    /// Whether the animation should repeat on completion
    bool m_repeat = false;

    /// Whether the animation is animated at a random frame rate
    bool m_random = false;
};
