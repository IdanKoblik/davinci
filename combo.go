package main

import (
	"fmt"
	"strings"
	"time"

	"github.com/gvalkov/golang-evdev"
)

func (vk *VirtualKeyboard) RunCombo(combo string) error {
	fmt.Println("Running combo:", combo)
	parts := strings.Fields(strings.ToUpper(combo))
	var mods []uint16
	var main uint16

	for _, p := range parts {
		p = strings.TrimPrefix(p, "KEY_")
		code, ok := KeyMap[p]
		if !ok {
			fmt.Printf("Warning: key '%s' not found in KeyMap\n", p)
			continue
		}
		if strings.Contains(p, "CTRL") ||
			strings.Contains(p, "SHIFT") ||
			strings.Contains(p, "ALT") ||
			strings.Contains(p, "META") {
			mods = append(mods, code)
		} else {
			main = code
		}
	}

	// Press modifiers
	for _, m := range mods {
		if err := vk.emit(evdev.EV_KEY, m, 1); err != nil {
			return err
		}
		if err := vk.sync(); err != nil {
			return err
		}
		time.Sleep(10 * time.Millisecond)
	}

	// Press main key
	if main != 0 {
		if err := vk.emit(evdev.EV_KEY, main, 1); err != nil {
			return err
		}
		if err := vk.sync(); err != nil {
			return err
		}
		time.Sleep(50 * time.Millisecond)

		// Release main key
		if err := vk.emit(evdev.EV_KEY, main, 0); err != nil {
			return err
		}
		if err := vk.sync(); err != nil {
			return err
		}
		time.Sleep(10 * time.Millisecond)
	}

	// Release modifiers in reverse order
	for i := len(mods) - 1; i >= 0; i-- {
		if err := vk.emit(evdev.EV_KEY, mods[i], 0); err != nil {
			return err
		}
		if err := vk.sync(); err != nil {
			return err
		}
		time.Sleep(10 * time.Millisecond)
	}

	return nil
}