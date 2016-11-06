

#include "snake.h"

#include <iostream>

#include "Draw.h"
#include "Game.h"

const char Snake::SEGMENT_CHAR[] = "O";

Snake::Snake(const unsigned int startingSize, const float minSpeed):
myNumSegmentsResize(0),
mySegments(startingSize),
myDistTravelledSinceMoved(0.0),
myDirection(VEC_RIGHT),
myMinSpeed(minSpeed),
mySpeedStep(1.0)
{
  // Set up the initial position
  Vector segmentPos(5, 5);
  for(unsigned int i = 0; i < size(); ++i)
  {
    mySegments[i] = segmentPos;
    segmentPos -= myDirection;
  }
  mySpeed = calcSpeed();
}

void Snake::tick(Game & game)
{
  myDistTravelledSinceMoved += game.getElapsedTime() * mySpeed;

  if(game.getLastKeypress() == SKEY_UP && myDirection != VEC_DOWN)
    myDirection = VEC_UP;
  else if(game.getLastKeypress() == SKEY_DOWN && myDirection != VEC_UP)
    myDirection = VEC_DOWN;
  else if(game.getLastKeypress() == SKEY_LEFT && myDirection != VEC_RIGHT)
    myDirection = VEC_LEFT;
  else if(game.getLastKeypress() == SKEY_RIGHT && myDirection != VEC_LEFT)
    myDirection = VEC_RIGHT;

  // Get the integer distance travelled, only move if it is more than 0
  const int distTravelled = static_cast<int>(myDistTravelledSinceMoved);
  if(distTravelled > 0)
  {
    for(int i = 0; i < distTravelled; ++i)
      move();
    myDistTravelledSinceMoved -= static_cast<float>(distTravelled);
  }

  if(hasCollidedWithSelf())
    game.triggerGameOver();
}

void Snake::draw()
{
  // Draw each segment
  for(unsigned int i = 0; i < size(); ++i)
  {
    drawChars(mySegments[i], SEGMENT_CHAR);
  }
}

void Snake::move()
{
  const Vector lastSegmentPos = mySegments[size() - 1];
  // Do we need to remove a segment
  if(myNumSegmentsResize < 0)
  {
    mySegments.pop_back();
    ++myNumSegmentsResize;
  }

  // Move body pieces
  for(int i = size() - 1; i > 0; --i)
  {
    mySegments[i] = mySegments[i - 1];
  }
  // Move the head
  mySegments[0] += myDirection;

  // Do we need to add a segment
  if(myNumSegmentsResize > 0)
  {
    mySegments.push_back(lastSegmentPos);
    --myNumSegmentsResize;
  }
  mySpeed = calcSpeed();
}

float Snake::calcSpeed() const
{
  return myMinSpeed + (mySpeedStep * static_cast<float>(size()));
}

bool Snake::hasCollidedWithSelf() const
{
  for(unsigned int i = 1; i < size(); ++i)
  {
    if(mySegments[0] == mySegments[i])
      return true;
  }
  return false;
}

const Vector & Snake::getHeadPos() const
{
  return mySegments[0];
}

unsigned int Snake::size() const
{
  return mySegments.size();
}

void Snake::resize(const int numSegments)
{
  // Protect against having 0 segments
  if(numSegments < 0 && size() + myNumSegmentsResize <= 1)
    return;

  myNumSegmentsResize += numSegments;
}

