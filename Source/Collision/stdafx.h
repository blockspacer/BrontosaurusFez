#pragma once

#include "../CommonUtilities/GrowingArray.h"
#include "Intersection.h"

#include "SquareCollider.h"
#include "CircleCollider.h"
#include "GroupCollider.h"
#include "PointCollider.h"

#define SAFE_DELETE(ptr) delete ptr; ptr = nullptr
#define self (*this)