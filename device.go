package main

import (
	"os"
	"fmt"
	"strings"
	"path/filepath"
)

func FindDevice(vendor, product string) (string, error) {
	entries, err := os.ReadDir("/sys/class/input")
	if err != nil {
		return "", err
	}

	for _, e := range entries {
		if !strings.HasPrefix(e.Name(), "event") {
			continue
		}

		symlinkPath := filepath.Join("/sys/class/input", e.Name(), "device")
		realPath, err := filepath.EvalSymlinks(symlinkPath)
		if err != nil {
			continue
		}

		curr := realPath

		for {
			vFile := filepath.Join(curr, "idVendor")
			pFile := filepath.Join(curr, "idProduct")

			v, vErr := os.ReadFile(vFile)
			p, pErr := os.ReadFile(pFile)

			if vErr == nil && pErr == nil {
				if strings.TrimSpace(string(v)) == vendor &&
					strings.TrimSpace(string(p)) == product {
					return "/dev/input/" + e.Name(), nil
				}

				break 
			}

			parent := filepath.Dir(curr)
			if parent == curr || parent == "." || parent == "/" {
				break
			}
			curr = parent
		}
	}

	return "", fmt.Errorf("device not found")
}


