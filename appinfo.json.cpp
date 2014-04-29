#include "src/config.h"

{
    "uuid": UUID,
    "shortName": SHORTNAME,
    "longName": LONGNAME,
    CONFIGURABLE
    "versionLabel": VERSION,
    "versionCode": VERSIONCODE,
    "companyName": "Koishi Horology",
    "watchapp": {
	"watchface": true
    },
    "resources": {
	"media": [
	    {
		"menuIcon": true,
		"type": "png",
		"name": "IMAGE_MENU_ICON",
		"file": "images/floating-dials-main-icon.png"
	    }
	]
    },
    "appKeys": {
	"background": 0,
	"hourdialtype": 1,
	"seconds": 2,
	"date": 3,
	"handwidth": 4,
	"contrast": 5
    },
    "preferences": {
	"description": "Reload watchface after saving to see changes.",
	"embeded": false,
	"embededx": true,
	"url" : "http://fugounashi.github.io/floating-dials/autoconfig/2.5.0.html",
	"xurl" : "http://192.168.10.59:8000/autoconfig.html",
	"items": [
	    {
		"name":     "background",
		"title":    "Background Colour",
		"type":     "boolean",
		"off-text": "Black",
		"on-text":  "White",
		"default":  false
	    },
	    {
		"name":     "contrast",
		"title":    "Invert Main Dial",
		"type":     "boolean",
		"off-text": "Off",
		"on-text":  "On",
		"default":  false
	    },
	    {
		"name":     "hourdialtype",
		"title":    "Hour Dial Type",
		"type":     "boolean",
		"off-text": "12h",
		"on-text":  "24h",
		"default":  false
	    },
	    {
		"name":     "seconds",
		"title":    "Seconds Dial",
		"type":     "boolean",
		"off-text": "Off",
		"on-text":  "On",
		"default":  true
	    },
	    {
		"name":     "date",
		"title":    "Day / Date Dial",
		"type":     "boolean",
		"off-text": "Off",
		"on-text":  "On",
		"default":  true
	    },
	    {
		"name":     "handwidth",
		"title":    "Hand Width",
		"type":     "integer",
		"description": "10-35% of hand length.",
		"min": 10,
		"max":  35,
		"default":  20
	    }
	]
    }
}
