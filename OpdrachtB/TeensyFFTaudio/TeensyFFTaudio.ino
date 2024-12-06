// filename: TeensyFTTaudio.
// Autor: J. v/d Helder
// date 21/05/2021
// program developed for Teensy3.2 on Arduino IDE with the TeensyDruino plugin
// When button SW1 is pressed, the sample table with measured audio data and the FFT is calculated
// When button SW2 is pressed the serial output will be the samples array (when SW3 is switched up), 
// which is filled with a measured audio signal from a microphone or the calculated FFT-values (when SW3 is switched down) 
// adjusted to include SW4
#define ARM_MATH_CM4
#include <arm_math.h>

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION
// These values can be changed to alter the behavior of the spectrum display.
// Nr of bins = FFT_SIZE / 2
// Frequency Resolution (FR) = Fmax / nrOfBins    or   FR = sample_rate / FFT_SIZE
// Fmax = sample_rate /2
// If sampleRate = 9000 and FFT_SIZE = 256, then FR = 35.156 Hz (the bins are separated with FR)
////////////////////////////////////////////////////////////////////////////////

int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.
const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256
const int BUTTON1 = 4;
const int BUTTON2 = 5;
const int SWITCH3 = 6;
const int SWITCH4 = 7;
const int LED1 = 11;
const int LED2 = 12;
const int LED3 = 13;
const int AUDIO_INPUT_PIN = 23; //A9        // Input ADC pin for audio data.
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
// any other changes to the program.
const int MAX_CHARS = 65;              // Max size of the input command buffer
int BeginMicro = 0;
int EindMicro = 0;

IntervalTimer samplingTimer;
float samples[FFT_SIZE * 2];
float timeDomainRaw[FFT_SIZE];
float timeDomainRev[FFT_SIZE];
float freqDomain[FFT_SIZE];
int sampleCounter;
char commandBuffer[MAX_CHARS];
void samplingBegin();
void setup() {
  // Set up serial port.
  Serial.begin(38400);

  // Set up ADC and audio input.
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  pinMode(SWITCH3, INPUT);
  pinMode(SWITCH4, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(AUDIO_INPUT_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);

  // Clear the input command buffer
  memset(commandBuffer, 0, sizeof(commandBuffer));
}

void loop() {
  if (digitalRead(BUTTON1)) {
    digitalWrite(LED1, HIGH);
    // Begin sampling audio
    sampleCounter = 0;
    samplingBegin();
    while(sampleCounter < 512) delay(1);
    digitalWrite(LED1, LOW);
    
    digitalWrite(LED2, HIGH);
    BeginMicro = micros();
    for (int t=0; t<256; t++) timeDomainRaw[t]=samples[2*t]; // Copy the samples into TimeDomain as the FFT uses samples as output
    EindMicro = micros();
    Serial.print(BeginMicro); Serial.print("-"); Serial.print(EindMicro); Serial.print("="); Serial.println(EindMicro-BeginMicro);
    // Calculate FFT if a full sample is available.
    // Run FFT on sample data.
    arm_cfft_radix4_instance_f32 fft_inst;
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);  // samples now contains the real and imaginairy part of the FFT
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, freqDomain, FFT_SIZE);
    digitalWrite(LED2, LOW);

    delay(500);

    //Calculating back from complex frequency Domain to time Domain.
    //digitalWrite(LED3, HIGH);
    //for (int t=0; t<FFT_SIZE; t++) timeDomainRev[t] = 0.0;
    //for (int t=0; t<FFT_SIZE; t++) {
    //  for (int f=30; f<FFT_SIZE-30; f++) {
    //     
    //    timeDomainRev[t] += samples[2*f]*cos(2*PI*t*f/FFT_SIZE)/FFT_SIZE - samples[2*f+1]*sin(2*PI*t*f/FFT_SIZE)/FFT_SIZE;
    //  }
    //}
    //Calculating back from complex frequency Domain to time Domain usinf FFT.
    digitalWrite(LED3, HIGH);
    // Run RevFFT on sample data.
    arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 1, 1);
    arm_cfft_radix4_f32(&fft_inst, samples);  // samples now contains the real and imaginairy part of the FFT
    // Calculate magnitude of complex numbers output by the FFT.
    arm_cmplx_mag_f32(samples, timeDomainRev, FFT_SIZE);
    for (int t=0;t<FFT_SIZE;t++) {
        Serial.print(t); Serial.print("= ");  Serial.println(timeDomainRaw[t]); 
    }
    digitalWrite(LED3, LOW);
  }

  
  if (digitalRead(BUTTON2)) {
    if (digitalRead(SWITCH3)) {
      if (digitalRead(SWITCH4)) {
         for (int t = 1; t < 256; t++) { // print samples
            Serial.print(t); Serial.print("= ");  Serial.println(timeDomainRaw[t]);
         }
      }
      else {
         for (int t = 1; t < 256; t++) { // print samples
            Serial.print(t); Serial.print("= ");  Serial.println(timeDomainRev[t]);
         }
      }
    }
    else {
      for (int f = 1; f < 128; f++) { // print only first half, the others are imaginary
      Serial.print(f); Serial.print("= ");  Serial.println(freqDomain[f]);
      }
    }
    delay(500);
  }
}

////////////////////////////////////////////////////////////////////////////////
// SAMPLING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void samplingCallback() {
  // Read from the ADC and store the sample 
  //BeginMicro = micros();
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_PIN);
  //EindMicro = micros();
  //Serial.print(BeginMicro); Serial.print("-"); Serial.print(EindMicro); Serial.print("="); Serial.println(EindMicro-BeginMicro);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter + 1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE * 2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000 / SAMPLE_RATE_HZ);
}
