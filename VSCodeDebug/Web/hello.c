#define SOKOL_IMPL
#define SOKOL_GLES3

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

static sg_pass_action pass_action;

#define ENABLE_ASYNC 1

#if ENABLE_ASYNC
#include <emscripten/emscripten.h>
#include <emscripten/threading.h>
#include <pthread.h>
#endif

static void init(void)
{
  sg_setup(&(sg_desc){
    .context = sapp_sgcontext(),
    .logger.func = slog_func,
  });
  pass_action = (sg_pass_action){ .colors[0] = { .load_action = SG_LOADACTION_CLEAR,
                                    .clear_value = { 1.0f, 0.0f, 0.0f, 1.0f } } };
}

#if ENABLE_ASYNC
void* thread_function(void* arg)
{
  printf("Hello from the thread! Argument: %d\n", *(int*)arg);
  return NULL;
}
#endif

static void frame(void)
{
#if ENABLE_ASYNC
  pthread_t thread;
  int thread_arg = 42;

  printf("Main thread: creating a new thread\n");
  if (pthread_create(&thread, NULL, thread_function, &thread_arg) != 0)
  {
    printf("Failed to create thread\n");
  }

  pthread_join(thread, NULL);
  printf("Main thread: thread has finished\n");
#endif
  float g = pass_action.colors[0].clear_value.g + 0.01f;
  pass_action.colors[0].clear_value.g = (g > 1.0f) ? 0.0f : g;
  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
  sg_end_pass();
  sg_commit();
}

static void cleanup(void)
{
  sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[])
{
  (void)argc;
  (void)argv;
  return (sapp_desc){
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .window_title = "clear",
    .icon.sokol_default = true,
    .logger.func = slog_func,
  };
}
