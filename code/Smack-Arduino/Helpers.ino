void initOled(){
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
//  oled.display();  // Display what's in the buffer (splashscreen)
//  delay(1000);     // Delay 1000 ms
  oled.clear(PAGE); // Clear the buffer.

}
// Center and print a small title
// This function is quick and dirty. Only works for titles one
// line long.
void printTitle(String title, int font)
{
  int middleX = oled.getLCDWidth() / 2;
  int middleY = oled.getLCDHeight() / 2;
  
//  oled.clear(PAGE);
  oled.setFontType(font);
  // Try to set the cursor in the middle of the screen
  oled.setCursor(middleX - (oled.getFontWidth() * (title.length()/2)),
                 middleY - (oled.getFontWidth() / 2));
  // Print the title:
  oled.print(title);

}
