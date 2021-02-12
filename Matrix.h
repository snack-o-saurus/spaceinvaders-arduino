const uint8_t kMatrixWidth = 32;
const uint8_t kMatrixHeight = 16;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[NUM_LEDS];

class Surface{
  private:
    unsigned long _systime;
    unsigned long _timestamp;
    int _fps;

  public:
    Surface(int fps) {
      setFps(fps);
    }
    // Setter
    void setSystime(unsigned long systime) {
      _systime = systime;
    }
    void setTimestamp(unsigned long timestamp) {
      _timestamp = timestamp;
    }
    void setFps(int fps) {
      _fps = fps;
    }
    // Getter
    unsigned long getSystime() {
      return _systime;
    }
    unsigned long getTimestamp() {
      return _timestamp;
    }
    int getFps() {
      return _fps;
    }
    // Main methods
    void timer() {
      setSystime(millis());
    }
    void refresh() {
      if (getSystime() - getTimestamp() >= (1000 / getFps())) {
        setTimestamp(getSystime());
        FastLED.show();
      }
    }
};

class Paddle {
  private:
    bool _debugMsg;
    int _width;
    int _height;
    int _x;
    int _y;
    CRGB _color;

  public:
    Paddle(int x, int y, int width, int height, CRGB color, bool debugMsg) {
      setX(x);
      setY(y);
      setWidth(width);
      setHeight(height);
      setColor(color);
      setDebugMsg(debugMsg);
    }

    // Setter
    void setX(int x) {
      _x = x;
    }
    void setY(int y) {
      _y = y;
    }
    void setWidth(int width) {
      _width = width;
    }
    void setHeight(int height) {
      _height = height;
    }
    void setColor(CRGB color) {
      _color = color;
    }
    void setDebugMsg(bool debugMsg) {
      _debugMsg = debugMsg;
    }

    // Getter
    int getX() {
      return _x;
    }
    int getY() {
      return _y;
    }
    int getWidth() {
      return _width;
    }
    int getHeight() {
      return _height;
    }
    CRGB getColor() {
      return _color;
    }
    bool getDebugMsg() {
      return _debugMsg;
    }

    // Main methods
    uint16_t xy(uint8_t x, uint8_t y) {
      int i = x + (y * kMatrixWidth);
      if (getDebugMsg()) Serial.println(i);
      return i;
    }

    void draw() {
      int x = getX();
      int y = getY();
      int width = getWidth();
      int height = getHeight();
      CRGB color = getColor();

      for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
          leds[xy(x + w, y + h)] = color;
        }
      }
    }

    void reset() {
      int x = getX();
      int y = getY();
      int width = getWidth();
      int height = getHeight();
      
      for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
          leds[xy(x + w, y + h)] = CRGB(0, 0, 0);
        }
      }
    }

    void move(uint8_t x, uint8_t y) {
      if (x != getX() || y != getY()) {
        reset();
        setX(x);
        setY(y);
        draw();
      }
    }
};

class Ball {
  private:
    int _x;
    int _y;
    int _dirX = 1;
    int _dirY = 1;
    CRGB _color;
    Paddle * _paddle;
    Surface * _surface;
    unsigned long _timestamp;
    unsigned long _interval;

  public:
    Ball(int x, int y, int dirX, int dirY, CRGB color, unsigned long interval) {
      setX(x);
      setY(y);
      setDirX(dirX);
      setDirY(dirY);
      setColor(color);
      setInterval(interval);
    }

    // Setter
    void setX(int x) {
      _x = x;
    }
    void setY(int y) {
      _y = y;
    }
    void setDirX(int dirX) {
      if (dirX == 1 || dirX == -1) {
        _dirX = dirX;
      }
    }
    void setDirY(int dirY) {
      if (dirY == 1 || dirY == -1) {
        _dirY = dirY;
      }
    }
    void setColor(CRGB color) {
      _color = color;
    }
    void setTimestamp(unsigned long timestamp) {
      _timestamp = timestamp;
    }
    void setInterval(unsigned long interval) {
      _interval = interval;
    }

    // Getter
    int getX() {
      return _x;
    }
    int getY() {
      return _y;
    }
    int getDirX() {
      return _dirX;
    }
    int getDirY() {
      return _dirY;
    }
    CRGB getColor() {
      return _color;
    }
    unsigned long getTimestamp() {
      return _timestamp;
    }
    unsigned long getInterval() {
      return _interval;
    }

    // Main methods
    uint16_t xy(uint8_t x, uint8_t y) {
      int i = x + (y * kMatrixWidth);
      return i;
    }
    void checkCollision(int x, int y) {
      int xPaddle = _paddle->getX();
      int yPaddle = _paddle->getY();
      if (x+1 >= xPaddle && x <= xPaddle + _paddle->getWidth() && y+1 >= yPaddle && y <= yPaddle + _paddle->getHeight()) {
        setDirY(-getDirY());
        if (getInterval() > 100) {
          setInterval(getInterval() - 50);
        }
      }
      if (x <= 0 || x >= kMatrixWidth - 1) {
        setDirX(-getDirX());
      }
      if (y <= 0 || y >= kMatrixHeight - 1) {
        setDirY(-getDirY());
      }
    }
    void reset() {
      leds[xy(getX(), getY())] = CRGB(0, 0, 0);
    }
    void draw() {
      leds[xy(getX(), getY())] = getColor();
    }
    void move() {
      if (_surface->getSystime() - getTimestamp() >= getInterval()) {
        setTimestamp(_surface->getSystime());
        int x = getX();
        int y = getY();
        
        checkCollision(x, y);
        reset();
        setX(x + getDirX());
        setY(y + getDirY());
        /*
        Serial.print("X: ");
        Serial.print(x);
        Serial.print("| Y: ");
        Serial.println(y);
        */
        draw();
      }
    }
    void attachCollider(Paddle * paddle) {
      _paddle = paddle;
    }
    void attachSurface(Surface * surface) {
      _surface = surface;
    }
};
