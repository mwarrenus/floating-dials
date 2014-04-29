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
	"contrast": 5,
	"taptimeout": 6
    },
    "preferences": {
	"description": "Reload watchface after saving to see changes.",
	"embeded": false,
	"url" : "http://fugounashi.github.io/floating-dials/autoconfig/2.7.0.html",
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
		"type": "enum",
		"choices": {
		  "OFF": "Off",
		  "ON": "On",
		  "TAP": "Tap to toggle On / Off"
		},
		"default": 2
	    },
	    {
		"name":     "taptimeout",
		"title":    "Tap Timeout",
		"type":     "integer",
		"description": "Minutes after tap to automatically turn seconds dial off.  0 to disable.",
		"min": 0,
		"max": 60,
		"default": 5
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
