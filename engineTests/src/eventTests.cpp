#include "eventTests.h"

TEST(Events, ResizeConstructor) {
	EXPECT_EQ(re.getWidth(), width);
}
TEST(Events, MovedConstructor) {
	EXPECT_EQ(wme.getXPos(), WinPosX);
}

TEST(Events, KeyPressedConstructor) {
	EXPECT_EQ(kpe.GetRepeatCount(), keyRepeatCount);
}
TEST(Events, KeyReleasedConstructor) {
	EXPECT_EQ(kre.GetKeyCode(), keyCode);
}
TEST(Events, KeyTypedConstructor) {
	EXPECT_EQ(kte.GetKeyCode(), keyCode);
}

TEST(Events, MouseButtonPressedConstructor) {
	EXPECT_EQ(mbpe.getButton(), mouseButton);
}
TEST(Events, MouseButtonReleasedConstructor) {
	EXPECT_EQ(mbre.getButton(), mouseButton);
}
TEST(Events, MouseMovedConstructor) {
	EXPECT_EQ(mme.getX(), MouPosX);
}
TEST(Events, MouseScrolledConstructor) {
	EXPECT_EQ(mse.getYOffset(), offsetY);
}

TEST(Events, DispatchTrue) {
	Engine::EventDispatcher dispatcher(re);
	bool result = dispatcher.dispatch<Engine::WindowResizeEvent>(std::bind(OnResizeTrue, std::placeholders::_1));
	EXPECT_TRUE(result);
}
TEST(Events, DispatchFalse) {
	Engine::EventDispatcher dispatcher(ce);
	bool result = dispatcher.dispatch<Engine::WindowResizeEvent>(std::bind(OnResizeTrue, std::placeholders::_1));
	EXPECT_FALSE(result);
}
TEST(Events, DispatchHandleTrue) {
	Engine::EventDispatcher dispatcher(re);
	dispatcher.dispatch<Engine::WindowResizeEvent>(std::bind(OnResizeTrue, std::placeholders::_1));
	bool result = re.handled();
	EXPECT_TRUE(result);
}
TEST(Events, DispatchHandleFalse) {
	Engine::EventDispatcher dispatcher(re);
	dispatcher.dispatch<Engine::WindowResizeEvent>(std::bind(OnResizeFalse, std::placeholders::_1));
	bool result = re.handled();
	EXPECT_FALSE(result);
}