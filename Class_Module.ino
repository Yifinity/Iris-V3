/*
 * Module Class for drawing the screen Menu
 */

class Module{
  private:
    // Module Size
    uint16_t rectHeight = 110;
    uint16_t rectWidth = 98;
    uint16_t rectRadius = 15; 
    uint16_t moduleColor; 

    uint16_t selectedWidth = rectWidth - 10;
    uint16_t selectedHeight = rectHeight - 10;

    //Module Y Positions
    int unselectedY = 75;
    int selectedY = 80;

    // X Positions
    int leftPos = 9;
    int selectedPos = 111;
    int centerPos = 116;
    int rightPos = 214;

    // Icon Variables
    int iconSize = 68; 
    int leftIconX = 24; 
    int centerIconX = 126; 
    int rightIconX = 229;
    int iconY = 96;
    const unsigned char* moduleIcon; 

  public: 
    Module(uint16_t color, const unsigned char* icon){ 
      moduleColor = color; 
      moduleIcon = icon; 

    }

    void draw(int position){
      switch(position){
        case 0:
          tft.fillRoundRect(leftPos, unselectedY, rectWidth, rectHeight, rectRadius, moduleColor);
          tft.drawBitmap(leftIconX, iconY, moduleIcon, iconSize, iconSize, ST77XX_WHITE);

        case 1:
          tft.fillRoundRect(selectedPos, unselectedY, rectWidth, rectHeight, rectRadius, ST77XX_WHITE);
          tft.fillRoundRect(centerPos, selectedY, selectedWidth, selectedHeight, rectRadius, moduleColor);
          
          tft.drawBitmap(centerIconX, iconY, moduleIcon, iconSize, iconSize, ST77XX_WHITE);

        case 2:
          tft.fillRoundRect(rightPos, unselectedY, rectWidth, rectHeight, rectRadius, moduleColor);
          
          tft.drawBitmap(rightIconX, iconY, moduleIcon, iconSize, iconSize, ST77XX_WHITE);
      }
    }
};