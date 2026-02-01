package main

import (
	"log"
	"fmt"
	"os"

	"github.com/gvalkov/golang-evdev"
)

func main() {
	home := os.Getenv("HOME")

	configPath := fmt.Sprintf("%s/.config/davinci/config.toml", home)
	cfg, err := LoadConfig(configPath)
	if err != nil {
		log.Fatal(err)
	}

	devPath, err := FindDevice("056a", "037b")
	if err != nil {
		log.Fatal(err)
	}

	stylus, err := evdev.Open(devPath)
	if err != nil {
		log.Fatal(err)
	}

	vk, err := NewVirtualKeyboard()
	if err != nil {
		log.Fatal(err)
	}

	profile := cfg.Profile["default"]

	for {
		events, _ := stylus.Read()
		for _, ev := range events {
			if ev.Type == evdev.EV_KEY && ev.Value == 1 {
				switch ev.Code {
				case evdev.BTN_STYLUS:
					vk.RunCombo(profile.BTN1)
				case evdev.BTN_STYLUS2:
					vk.RunCombo(profile.BTN2)
				}
			}
		}
	}
}

