//#######################################################################################################
//#################################### Plugin 041: NeoPixel clock #######################################
//#######################################################################################################
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS      60

byte Plugin_041_enabled = 1;
byte Plugin_041_brightness = 64;
byte Plugin_041_marks = 4;
byte Plugin_041_offset = 30;

Adafruit_NeoPixel * Plugin_041_pixels;

#define PLUGIN_041
#define PLUGIN_ID_041         41
#define PLUGIN_NAME_041       "NeoPixel - Clock"
#define PLUGIN_VALUENAME1_041 "Enabled"
#define PLUGIN_VALUENAME2_041 "Brightness"
#define PLUGIN_VALUENAME3_041 "Marks"
boolean Plugin_041(byte function, struct EventStruct *event, String& string)
{
  boolean success = false;

  switch (function)
  {

    case PLUGIN_DEVICE_ADD:
      {
        Device[++deviceCount].Number = PLUGIN_ID_041;
        Device[deviceCount].Type = DEVICE_TYPE_SINGLE;
        Device[deviceCount].Custom = true;
        Device[deviceCount].ValueCount = 3;
        Device[deviceCount].TimerOption = false;
        break;
      }

    case PLUGIN_GET_DEVICENAME:
      {
        string = F(PLUGIN_NAME_041);
        break;
      }

    case PLUGIN_GET_DEVICEVALUENAMES:
      {
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[0], PSTR(PLUGIN_VALUENAME1_041));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[1], PSTR(PLUGIN_VALUENAME2_041));
        strcpy_P(ExtraTaskSettings.TaskDeviceValueNames[2], PSTR(PLUGIN_VALUENAME3_041));
        break;
      }

    case PLUGIN_WEBFORM_LOAD:
      {
        char tmpString[128];
        string += F("<TR><TD>LED pin:<TD>");
        addPinSelect(false, string, "taskdevicepin1", Settings.TaskDevicePin1[event->TaskIndex]);

        sprintf_P(tmpString, PSTR("<TR><TD>LED Offset:<TD><input title=\"Position of the 12 o'clock LED in the strip\" type='text' name='plugin_041_offset' size='3' value='%u'>"), Settings.TaskDevicePluginConfig[event->TaskIndex][3]);
        string += tmpString;
        sprintf_P(tmpString, PSTR("<TR><TD>Clock display enabled:<TD><input title=\"LEDs activation\" type='text' name='plugin_041_enabled' size='3' value='%u'>"), Settings.TaskDevicePluginConfig[event->TaskIndex][0]);
        string += tmpString;
        sprintf_P(tmpString, PSTR("<TR><TD>LED brightness:<TD><input title=\"Brightness level of the H,M,S indicators (0-255)\" type='text' name='plugin_041_brightness' size='3' value='%u'>"), Settings.TaskDevicePluginConfig[event->TaskIndex][1]);
        string += tmpString;
        sprintf_P(tmpString, PSTR("<TR><TD>Hour mark brightness:<TD><input title=\"Brightness level of the hour marks\" type='text' name='plugin_041_marks' size='3' value='%u'>"), Settings.TaskDevicePluginConfig[event->TaskIndex][2]);
        string += tmpString;
        success = true;
        break;
      }

    case PLUGIN_WEBFORM_SAVE:
      {
        String plugin1 = WebServer.arg(F("plugin_041_enabled"));
        if (plugin1.toInt() > 1) Settings.TaskDevicePluginConfig[event->TaskIndex][0] = 1;
        else Settings.TaskDevicePluginConfig[event->TaskIndex][0] = plugin1.toInt();
        String plugin2 = WebServer.arg(F("plugin_041_brightness"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][1] = plugin2.toInt();
        String plugin3 = WebServer.arg(F("plugin_041_marks"));
        Settings.TaskDevicePluginConfig[event->TaskIndex][2] = plugin3.toInt();
        String plugin4 = WebServer.arg(F("plugin_041_offset"));
        if (plugin4.toInt() > 59) Settings.TaskDevicePluginConfig[event->TaskIndex][3] = 0;
        else Settings.TaskDevicePluginConfig[event->TaskIndex][3] = plugin4.toInt();

        Plugin_041_enabled = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        Plugin_041_brightness = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        Plugin_041_marks = Settings.TaskDevicePluginConfig[event->TaskIndex][2];
        Plugin_041_offset = Settings.TaskDevicePluginConfig[event->TaskIndex][3];

        success = true;
        break;
      }

    case PLUGIN_INIT:
      {
        if (!Plugin_041_pixels)
        {
          Plugin_041_pixels = new Adafruit_NeoPixel(NUM_LEDS, Settings.TaskDevicePin1[event->TaskIndex], NEO_GRB + NEO_KHZ800);
          Plugin_041_pixels->begin(); // This initializes the NeoPixel library.
        }
        Plugin_041_enabled = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
        Plugin_041_brightness = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
        Plugin_041_marks = Settings.TaskDevicePluginConfig[event->TaskIndex][2];
        Plugin_041_offset = Settings.TaskDevicePluginConfig[event->TaskIndex][3];

        success = true;
        break;
      }

    case PLUGIN_ONCE_A_SECOND:
      {
        Ceas_update();
        success = true;
        break;
      }

    case PLUGIN_WRITE:
      {
        String tmpString  = string;

        if (tmpString.startsWith("Clock,")) {
          int idx1 = tmpString.indexOf(',');
          int idx2 = tmpString.indexOf(',', idx1 + 1);
          int idx3 = tmpString.indexOf(',', idx2 + 1);
          int idx4 = tmpString.indexOf(',', idx3 + 1);
          String val_Mode = tmpString.substring(idx1 + 1, idx2);
          String val_Bright = tmpString.substring(idx2 + 1, idx3);
          String val_Marks = tmpString.substring(idx3 + 1, idx4);

          if (val_Mode != "") {
            if (val_Mode.toInt() > -1 && val_Mode.toInt() < 2) {
              Settings.TaskDevicePluginConfig[event->TaskIndex][0] = val_Mode.toInt();
              Plugin_041_enabled = Settings.TaskDevicePluginConfig[event->TaskIndex][0];
            }
          }
          if (val_Bright != "") {
            if (val_Bright.toInt() > -1 && val_Bright.toInt() < 256) {
              Settings.TaskDevicePluginConfig[event->TaskIndex][1] = val_Bright.toInt();
              Plugin_041_brightness = Settings.TaskDevicePluginConfig[event->TaskIndex][1];
            }
          }
          if (val_Marks != "") {
            if (val_Marks.toInt() > -1 && val_Marks.toInt() < 256) {
              Settings.TaskDevicePluginConfig[event->TaskIndex][2] = val_Marks.toInt();
              Plugin_041_marks = Settings.TaskDevicePluginConfig[event->TaskIndex][2];
            }
          }
        }
        break;
      }

    case PLUGIN_READ:
      {
        UserVar[event->BaseVarIndex] = Plugin_041_enabled;
        UserVar[event->BaseVarIndex + 1] = Plugin_041_brightness;
        UserVar[event->BaseVarIndex + 2] = Plugin_041_marks;

        success = true;
      }

  }
  return success;
}

void Ceas_update()
{
  resetAndBlack();
  if (Plugin_041_enabled > 0) {
    int Hours = hour();
    int Minutes = minute();
    int Seconds = second();
    timeToStrip(Hours, Minutes, Seconds);
  }
  Plugin_041_pixels->show(); // This sends the updated pixel color to the hardware.
}


void resetAndBlack() {
  for (int i = 0; i < NUM_LEDS; i++) {
    Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(0, 0, 0));
  }
}

void timeToStrip(int hours, int minutes, int seconds) {
  if (hours > 11) hours = hours - 12;
  hours = (hours * 5) + (minutes / 12) + Plugin_041_offset;
  if (hours > 59) hours = hours - 60;
  minutes = minutes + Plugin_041_offset;
  if (minutes > 59) minutes = minutes - 60;
  seconds = seconds + Plugin_041_offset;
  if (seconds > 59) seconds = seconds - 60;
  for (int i = 0; i < NUM_LEDS; i = i + 5) {
    if ((i != hours) && (i != minutes) && (i != seconds)) {
      Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(Plugin_041_marks, Plugin_041_marks, Plugin_041_marks));
    }
  }
  uint32_t currentColor;
  uint8_t r_val, g_val, b_val;
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i == hours) {
      Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(Plugin_041_brightness, 0, 0));
    }
    if (i == minutes) {
      currentColor = Plugin_041_pixels->getPixelColor(i);
      r_val = (uint8_t)(currentColor >> 16);
      Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(r_val, Plugin_041_brightness, 0));
    }
    if (i == seconds) {
      currentColor = Plugin_041_pixels->getPixelColor(i);
      r_val = (uint8_t)(currentColor >> 16);
      g_val = (uint8_t)(currentColor >>  8);
      Plugin_041_pixels->setPixelColor(i, Plugin_041_pixels->Color(r_val, g_val, Plugin_041_brightness));
    }
  }
}

