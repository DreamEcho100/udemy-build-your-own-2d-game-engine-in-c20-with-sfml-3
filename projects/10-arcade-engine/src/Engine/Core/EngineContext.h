#pragma once

#include "Engine/Manager/ClipboardManager.h"
#include "Engine/Manager/RandomManager.h"
#include "Engine/Manager/SaveManager.h"
#include "Engine/Manager/TimeManager.h"

class EngineContext {

public:
  RandomManager random;
  TimeManager time;
  SaveManager save;
  ClipboardManager clipboard;

  EngineContext();
};
