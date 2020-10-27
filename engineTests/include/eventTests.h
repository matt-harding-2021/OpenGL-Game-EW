#pragma once
#include <gtest/gtest.h>

#include "events/event.h"
#include "events/eventDispatcher.h"
#include "events/keyEvents.h"
#include "events/mouseEvents.h"
#include "events/windowEvents.h"

const int width = 1024;
const int height = 720;
Engine::WindowResizeEvent re(width, height);
Engine::WindowCloseEvent ce;
Engine::WindowFocusEvent fe;
Engine::WindowLostFocusEvent lfe;
int WinPosX = 200;
int WinPosY = 250;
Engine::WindowMovedEvent wme(WinPosX, WinPosY);

int keyCode = 55;
int keyRepeatCount = 10;
Engine::KeyPressedEvent kpe(keyCode, keyRepeatCount);
Engine::KeyReleasedEvent kre(keyCode);
Engine::KeyTypedEvent kte(keyCode);

int mouseButton = 1;
float MouPosX = 100;
float MouPosY = 150;
float offsetX = 30;
float offsetY = 30;
Engine::MouseButtonPressedEvent mbpe(mouseButton);
Engine::MouseButtonReleasedEvent mbre(mouseButton);
Engine::MouseMovedEvent mme(MouPosX, MouPosY);
Engine::MouseScrolledEvent mse(offsetX, offsetY);

bool OnResizeTrue(Engine::WindowResizeEvent& e) { return true; }
bool OnResizeFalse(Engine::WindowResizeEvent& e) { return false; }

