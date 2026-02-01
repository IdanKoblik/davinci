//go:build ignore
package main

import (
	"bufio"
	"fmt"
	"os"
	"regexp"
	"strings"
)

var re = regexp.MustCompile(`#define\s+(KEY_[A-Z0-9_]+)\s+(\d+)`)

func main() {
	in, err := os.Open("/usr/include/linux/input-event-codes.h")
	if err != nil {
		panic(err)
	}
	defer in.Close()

	out, err := os.Create("keys_gen.go")
	if err != nil {
		panic(err)
	}
	defer out.Close()

	fmt.Fprintln(out, "package main")
	fmt.Fprintln(out, "")
	fmt.Fprintln(out, "var KeyMap = map[string]uint16{")

	sc := bufio.NewScanner(in)
	for sc.Scan() {
		m := re.FindStringSubmatch(sc.Text())
		if m == nil {
			continue
		}
		name := strings.TrimPrefix(m[1], "KEY_")
		fmt.Fprintf(out, "\t%q: %s,\n", name, m[2])
	}

	fmt.Fprintln(out, "}")
}
