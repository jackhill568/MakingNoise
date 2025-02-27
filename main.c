#include <portaudio.h>
#include <stdio.h>

#define SAMPLE_RATE (44100)

typedef struct {
  float left_phase;
  float right_phase;
} paTestData;

static paTestData data;

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  /* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData *)userData;
  float *out = (float *)outputBuffer;
  unsigned int i;
  (void)inputBuffer; /* Prevent unused variable warning. */

  for (i = 0; i < framesPerBuffer; i++) {
    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase; /* right */
    /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
    data->left_phase += 0.01f;
    /* When signal reaches top, drop back down. */
    if (data->left_phase >= 1.0f)
      data->left_phase -= 2.0f;
    /* higher pitch so we can distinguish left and right. */
    data->right_phase += 0.05f;
    if (data->right_phase >= 1.0f)
      data->right_phase -= 2.0f;
  }
  return 0;
}

int check_error(PaError err) {
  if (err != paNoError) {
    return 1;
  } else {
    return 0;
  }
}

void error_has_happend(PaError err) { // shit
  printf("PortAudio error: %s\n", Pa_GetErrorText(err));
  err = Pa_Terminate();
}

int main(void) {
  PaError err = Pa_Initialize();
  if (check_error(err) == 1) {
    error_has_happend(err);
    return -1;
  }
  PaStream *stream;
  /* Open an audio I/O stream. */
  err = Pa_OpenDefaultStream(
      &stream, 0,       /* no input channels */
      2,                /* stereo output */
      paFloat32,        /* 32 bit floating point output */
      SAMPLE_RATE, 256, /* frames per buffer, i.e. the number
                               of sample frames that PortAudio will
                               request from the callback. Many apps
                               may want to use
                               paFramesPerBufferUnspecified, which
                               tells PortAudio to pick the best,
                               possibly changing, buffer size.*/
      patestCallback,   /* this is your callback function */
      &data);           /*This is a pointer that will be passed to
                                  your callback*/

  err = Pa_StartStream(stream);
  if (check_error(err) == 1) {
    error_has_happend(err);
  }

  Pa_Sleep(1000);

  err = Pa_StopStream(stream);
  if (check_error(err) == 1) {
    error_has_happend(err);
  }

  err = Pa_CloseStream(stream);
  if (check_error(err) == 1) {
    error_has_happend(err);
  }
  Pa_Terminate();
}
