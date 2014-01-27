#include "src/config.h"

#ifndef TWENTYFOUR
  #define UUID "a713c701-7bb1-4e19-9bef-5af215d6fc51"
#else
  #define UUID "a713c701-7bb1-4e19-9bef-5af215d6fc52"
#endif

{
  "uuid": UUID,
  "shortName": SHORTNAME,
  "longName": LONGNAME,
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
        "menuIcon": true,
        "type": "png",
        "name": "IMAGE_MENU_ICON",
        "file": "images/floating-dials-icon.png"
      }
    ]
  }
}
