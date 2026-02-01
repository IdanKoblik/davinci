package main

import "github.com/BurntSushi/toml"

type Profile struct {
	BTN1 string `toml:"BTN_1"`
	BTN2 string `toml:"BTN_2"`
}

type Config struct {
	Vendor  string             `toml:"vendor_id"`
	Product string             `toml:"product_id"`
	Profile map[string]Profile `toml:"profile"`
}

func LoadConfig(path string) (*Config, error) {
	var cfg Config
	_, err := toml.DecodeFile(path, &cfg)
	return &cfg, err
}
