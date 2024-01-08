#include "winctx.h"

void WinCtx::get_size(int* w, int* h) {
	glfwGetWindowSize(win, w, h);
}