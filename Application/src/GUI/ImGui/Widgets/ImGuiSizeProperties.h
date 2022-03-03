#pragma once

namespace IGWidget {

	struct ButtonSize
	{
		constexpr ButtonSize(float x, float y) : x(x), y(y) {}
		const float x, y;
	};
	static constexpr ButtonSize sg_ButtonSize(150.f, 40.f);
	static constexpr float sg_XPosTextInput = 10.f;

	#define IGWidget_BTN_OFFSET_X(x)           x - 170.f
	#define IGWidget_BTN_OFFSET_Y(x, y)        x - y

	#define IGWidget_X_SIZE_TEXT_INPUT(x)      x - 200.f
	#define IGWidget_Y_OFFSET_TEXT_INPUT(x, y) x - y


//#define X_OFFSET_BTN    ws.x - 170.f
//#define Y_OFFSET_BTN(x) ws.y - x
//
//#define X_SIZE_BTN      150.f
//#define Y_SIZE_BTN      40.f
//
//#define X_POS_TXT_IP    10.f
//#define X_SIZE_TXT_IP   ws.x - 200.f
//#define Y_OFF_TXT_IP(x) ws.y - x
}