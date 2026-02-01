package main

import (
	"encoding/binary"
	"fmt"
	"os"
	"syscall"
	"time"

	"github.com/gvalkov/golang-evdev"
)

const (
	UI_SET_EVBIT  = 0x40045564
	UI_SET_KEYBIT = 0x40045565
	UI_SET_PROPBIT = 0x4004556e
	UI_DEV_CREATE = 0x5501
	UI_DEV_DESTROY = 0x5502
)

type InputID struct {
	Bustype uint16
	Vendor  uint16
	Product uint16
	Version uint16
}

type UInputUserDev struct {
	Name       [80]byte
	ID         InputID
	EffectsMax uint32
	Absmax     [64]int32
	Absmin     [64]int32
	Absfuzz    [64]int32
	Absflat    [64]int32
}

type VirtualKeyboard struct {
	fd *os.File
}

func ioctl(fd uintptr, cmd uintptr, arg uintptr) error {
	_, _, errno := syscall.Syscall(syscall.SYS_IOCTL, fd, cmd, arg)
	if errno != 0 {
		return errno
	}
	return nil
}

func NewVirtualKeyboard() (*VirtualKeyboard, error) {
	fd, err := os.OpenFile(
		"/dev/uinput",
		os.O_WRONLY|syscall.O_NONBLOCK,
		0,
	)
	if err != nil {
		return nil, fmt.Errorf("failed to open /dev/uinput: %v", err)
	}

	if err := ioctl(fd.Fd(), UI_SET_EVBIT, evdev.EV_KEY); err != nil {
		fd.Close()
		return nil, fmt.Errorf("failed to set EV_KEY: %v", err)
	}
	if err := ioctl(fd.Fd(), UI_SET_EVBIT, evdev.EV_SYN); err != nil {
		fd.Close()
		return nil, fmt.Errorf("failed to set EV_SYN: %v", err)
	}

	for _, code := range KeyMap {
		if err := ioctl(fd.Fd(), UI_SET_KEYBIT, uintptr(code)); err != nil {
			fd.Close()
			return nil, fmt.Errorf("failed to set key %d: %v", code, err)
		}
	}

	var dev UInputUserDev
	copy(dev.Name[:], []byte("wacom-virtual-kbd"))
	dev.ID = InputID{
		Bustype: evdev.BUS_USB,
		Vendor:  0x1234,
		Product: 0x5678,
		Version: 1,
	}

	if err := binary.Write(fd, binary.LittleEndian, &dev); err != nil {
		fd.Close()
		return nil, fmt.Errorf("failed to write device info: %v", err)
	}

	if err := ioctl(fd.Fd(), UI_DEV_CREATE, 0); err != nil {
		fd.Close()
		return nil, fmt.Errorf("failed to create device: %v", err)
	}

	time.Sleep(200 * time.Millisecond)

	return &VirtualKeyboard{fd}, nil
}

func (vk *VirtualKeyboard) emit(t, c uint16, v int32) error {
	ev := evdev.InputEvent{
		Type:  t,
		Code:  c,
		Value: v,
	}
	
	ev.Time = syscall.Timeval{
		Sec:  time.Now().Unix(),
		Usec: int64(time.Now().Nanosecond() / 1000),
	}

	err := binary.Write(vk.fd, binary.LittleEndian, &ev)
	if err != nil {
		return fmt.Errorf("failed to write event: %v", err)
	}
	return nil
}

func (vk *VirtualKeyboard) sync() error {
	return vk.emit(evdev.EV_SYN, evdev.SYN_REPORT, 0)
}

func (vk *VirtualKeyboard) Close() error {
	if vk.fd != nil {
		ioctl(vk.fd.Fd(), UI_DEV_DESTROY, 0)
		return vk.fd.Close()
	}
	return nil
}

