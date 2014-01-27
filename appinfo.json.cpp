#include "src/config.h"

#ifndef TWENTYFOUR
  #define UUID "a713c701-7bb1-4e19-9bef-5af215d6fc51"
  #define APP_NAME "Floating Dials"
#else
  #define UUID "a713c701-7bb1-4e19-9bef-5af215d6fc52"
  #define APP_NAME "Floating Dials (24h)"
#endif

{
  "uuid": UUID,
  "shortName": APP_NAME,
  "longName": APP_NAME,
  "versionLabel": VERSION,
  "versionCode": VERSIONCODE,
  "companyName": "Koishi Horology",
  "watchapp": {
    "watchface": true
  },
  "appKeys": {},
  "resources": {
    "media": [
      {
        "type": "png",
        "name": "IMAGE_MENU_ICON",
        "file": "images/floating-dials-icon.png"
      }
    ]
  }
}
