const int pinLatch = 2;
const int pinClock = 4;
const int pinData = 6;

int pix[] = {
  0,0,0,0,0,0,1,0,
  0,0,0,0,0,1,1,1,
  0,0,0,0,0,1,0,1,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,1,0,0,
  0,0,0,1,1,1,0,0,
  0,0,0,1,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  1,0,0,0,1,1,0,0,
  1,0,0,0,1,1,0,0,
  1,1,1,1,1,1,0,0,
  0,1,1,1,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,0,0,0,0,0,0,
  1,1,1,0,0,0,0,0,
  1,0,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,0,
  0,0,1,1,1,0,0,0,
  0,0,0,1,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,1,1,0,0,0,1,
  0,0,1,1,0,0,0,1,
  0,0,1,1,1,1,1,1,
  0,0,0,1,1,1,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0
};


const int partCount = 5;
int px[partCount];
int py[partCount];
int pz[partCount];

const int zNear = 100;

void initPart(int i, bool randomZ) {
  px[i] = random(-8 * 200, 7 * 200);
  py[i] = random(-8 * 200, 7 * 200);
  pz[i] = zNear + (randomZ ? random(0, 200) : 100);
}

void starfield() {
  for (int i = 0; i < partCount; i += 1) {
    const int z = pz[i];

    int dx = px[i] / z + 8;
    int dy = py[i] / z + 8;

    writePixel(dx, dy);

    const int nextZ = z - 1;
    const int nextXRatio = px[i] / nextZ;
    const int nextYRatio = py[i] / nextZ;
    if (nextXRatio < -8 || nextXRatio >= 8 || nextYRatio < -8 || nextYRatio >= 8) {
      initPart(i, false);
    } else {
      pz[i] = nextZ;
    }
  }
}

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(pinLatch, OUTPUT);
  pinMode(pinClock, OUTPUT);
  pinMode(pinData, OUTPUT);

  digitalWrite(pinLatch, LOW);
  digitalWrite(pinClock, LOW);

  for (int i = 0; i < partCount; i += 1) {
    initPart(i, true);
  }
}

void writeHalf(int *start) {
  for (int i = 0; i < 16; i++) {
    for (int j=0; j < 8; j++) {
      const int on = start[(15 - i) * 8 + j];
      digitalWrite(pinData, on ? 1 : 0);

      digitalWrite(pinClock, HIGH);
      delayMicroseconds(1);
      digitalWrite(pinClock, LOW);
      delayMicroseconds(1);
    }
  }
}

void clearPixels() {
  for (int i = 0; i < 16 * 16; i += 1) {
    pix[i] = 0;
  }
}

void writePixel(int x, int y) {
  if (x < 8) {
    pix[16 * 8 + x + (15 - y) * 8] = 1;
  } else {
    pix[x - 8 + (15 - y) * 8] = 1;
  }
}

// the loop function runs over and over again forever
void loop() {
  clearPixels();
  starfield();

  writeHalf(pix + 16 * 8);
  writeHalf(pix);

  digitalWrite(pinLatch,HIGH);
  delayMicroseconds(1);
  digitalWrite(pinLatch,LOW);
  delayMicroseconds(1);

  delay(10);
}
