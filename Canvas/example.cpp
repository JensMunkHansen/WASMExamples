#include <emscripten/val.h>

using emscripten::val;

// Use thread_local when you want to retrieve & cache a global JS variable once per thread.
thread_local const val document = val::global("document");

// …

int main() {
  val canvas = document.call<val>("getElementById", "canvas");
  val ctx = canvas.call<val>("getContext", "2d");
  ctx.set("fillStyle", "green");
  ctx.call<void>("fillRect", 10, 10, 150, 100);
}
